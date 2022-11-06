#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

#ifdef POK_SIM
PTAD::BattleAnimation::AnimationData PTAD::BattleAnimation::_animation;
PTAD::BattleAnimation::AnimationData *PTAD::BattleAnimation::animation = &PTAD::BattleAnimation::_animation;
#else
PTAD::BattleAnimation::AnimationData *PTAD::BattleAnimation::animation = (PTAD::BattleAnimation::AnimationData*)PTAD::BATTLE_ANIMATION_MEMORY_ADDRESS;
#endif

uint8_t *PTAD::BattleAnimation::sprites = PTAD::memory + PTAD::MEMORY_BATTLE_ANIMATIONS_OFFSET;
uint8_t PTAD::BattleAnimation::currentFrame = 255;
uint8_t PTAD::BattleAnimation::nextFrame = 255;

void PTAD::BattleAnimation::beginAnimation(uint32_t animationHash)
{
  DataPack::PackedFile file;
  PTAD::dataFile->getPackedFile(animationHash, &file);
  PTAD::dataFile->readBytes(&file, (void*)animation, sizeof(AnimationData));
  PTAD::dataFile->getPackedFile(DataPack::hash("/animations/animations.png"), &file);
  for (int i = 0; i < 8; ++i)
  {
    file.seek((uint32_t)animation->sprites[i] * 1024);
    PTAD::dataFile->readBytes(&file, (void*)sprites + i * 1024, 1024);
  }
  currentFrame = 0;
  nextFrame = 0;
}

void PTAD::BattleAnimation::renderFrame()
{
  for (int i = 0; i < 8; ++i)
  {
    AnimationFrame *frame = &animation->frames[currentFrame];
    bool flipped = (frame->value2[i] & SPRITE_FLIPPED) != 0;
    bool mirrored = (frame->value2[i] & SPRITE_MIRRORED) != 0;
    if (frame->effect[i] == EFFECT_SPRITE)
      PD::drawSpriteBitmap((int)frame->value3[i] - 32, (int)frame->value4[i] - 32, 32, 32, sprites + (uint16_t)frame->value1[i] * 1024, flipped, mirrored);
    else if (nextFrame == 0 && frame->effect[i] != EFFECT_NONE)
    {
      if (frame->effect[i] == EFFECT_PLAY_SFX)
        PTAD::Music::playSFX(frame->value1[i]);
      else if (frame->effect[i] == EFFECT_SHAKE_SCREEN)
      {
        PTAD::Battle::shakeScreen = frame->value1[i];
        PTAD::Battle::shakeRate = frame->value2[i];
      }
      else if (frame->effect[i] == EFFECT_FLASH_UI)
      {
        PTAD::Battle::flashUi = frame->value1[i];
        PTAD::Ui::fgColor = frame->value2[i];
      }
      else if (frame->effect[i] == EFFECT_FLASH_BATTLER)
        PTAD::Battle::flashBattlerSprite(frame->value1[i], frame->value2[i], frame->value3[i], frame->value4[i]);
      else if (frame->effect[i] == EFFECT_FLASH_PLAYER)
        PTAD::Battle::flashPlayerSprite(frame->value1[i], frame->value2[i], frame->value3[i], frame->value4[i]);
    }
  }
  ++nextFrame;
  if (nextFrame == 3 || PB::cBtn())
  {
    nextFrame = 0;
    ++currentFrame;
    if (isNullFrame())
      currentFrame = 255;
  }
}

bool PTAD::BattleAnimation::isAnimationPlaying()
{
  if (currentFrame < 51)
    return true;
  return false;
}

bool PTAD::BattleAnimation::isNullFrame()
{
  if (currentFrame >= 51)
    return true;
  for (int i = 0; i < 8; ++i)
  {
    if (animation->frames[currentFrame].effect[i] != EFFECT_NONE)
      return false;
  }
  return true;
}
