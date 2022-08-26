/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2022 Justin (tuxinator2009) Davis                                *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

#include <QByteArray>
#include <QString>
#include <QList>
#include "battleanimation.h"
#include "globals.h"
#include "xmlParser.h"

BattleAnimation::BattleAnimation(QString name)
{
  fileName = name;
  for (int i = 0; i < 8; ++i)
    sprites[i] = 255;
  frames += QList<Effect*>();
}

BattleAnimation::~BattleAnimation()
{
  while (numFrames() > 0)
    deleteFrame(0);
}

void BattleAnimation::loadAnimation()
{
  XMLNode animationNode = XMLNode::openFileHelper(QString("%1/animations/%2.xml").arg(Globals::datadir).arg(fileName).toLocal8Bit().data(), "ptad-animation");
  XMLNode frameNode, effectNode;
  QStringList spriteIDs = QString(animationNode.getAttribute("sprites")).split(',');
  for (int i = 0; i < 8; ++i)
    sprites[i] = spriteIDs[i].toInt();
  while (numFrames() > 0)
    deleteFrame(0);
  for (int i = 0; i < animationNode.nChildNode("frame"); ++i)
  {
    QList<Effect*> effects;
    frameNode = animationNode.getChildNode("frame", i);
    for (int j = 0; j < frameNode.nChildNode(); ++j)
    {
      effectNode = frameNode.getChildNode(j);
      if (strcmp(effectNode.getName(), "sprite") == 0)
      {
        EffectSprite *sprite = new EffectSprite;
        sprite->effect = EFFECT_SPRITE;
        sprite->spriteID = atoi(effectNode.getAttribute("spriteID"));
        sprite->x = atoi(effectNode.getAttribute("x"));
        sprite->y = atoi(effectNode.getAttribute("y"));
        sprite->flipped = strcmp(effectNode.getAttribute("flipped"), "true") == 0;
        sprite->mirrored = strcmp(effectNode.getAttribute("mirrored"), "true") == 0;
        effects += sprite;
      }
      else if (strcmp(effectNode.getName(), "playSFX") == 0)
      {
        EffectPlaySFX *playSFX = new EffectPlaySFX;
        playSFX->effect = EFFECT_PLAY_SFX;
        playSFX->sfx = effectNode.getAttribute("sfx");
        effects += playSFX;
      }
      else if (strcmp(effectNode.getName(), "shakeScreen") == 0)
      {
        EffectShakeScreen *shakeScreen = new EffectShakeScreen;
        shakeScreen->effect = EFFECT_SHAKE_SCREEN;
        shakeScreen->duration = atoi(effectNode.getAttribute("duration"));
        shakeScreen->rate = atoi(effectNode.getAttribute("rate"));
        effects += shakeScreen;
      }
      else if (strcmp(effectNode.getName(), "flashUI") == 0)
      {
        EffectFlashUI *flashUI = new EffectFlashUI;
        flashUI->effect = EFFECT_FLASH_UI;
        flashUI->duration = atoi(effectNode.getAttribute("duration"));
        flashUI->color = atoi(effectNode.getAttribute("color"));
        effects += flashUI;
      }
      else if (strcmp(effectNode.getName(), "flashBattler") == 0)
      {
        EffectFlashBattler *flashBattler = new EffectFlashBattler;
        flashBattler->effect = EFFECT_FLASH_BATTLER;
        flashBattler->duration = atoi(effectNode.getAttribute("duration"));
        flashBattler->color1 = atoi(effectNode.getAttribute("color1"));
        flashBattler->color2 = atoi(effectNode.getAttribute("color2"));
        flashBattler->color3 = atoi(effectNode.getAttribute("color3"));
        effects += flashBattler;
      }
      else if (strcmp(effectNode.getName(), "flashPlayer") == 0)
      {
        EffectFlashPlayer *flashPlayer = new EffectFlashPlayer;
        flashPlayer->effect = EFFECT_FLASH_PLAYER;
        flashPlayer->duration = atoi(effectNode.getAttribute("duration"));
        flashPlayer->color1 = atoi(effectNode.getAttribute("color1"));
        flashPlayer->color2 = atoi(effectNode.getAttribute("color2"));
        flashPlayer->color3 = atoi(effectNode.getAttribute("color3"));
        effects += flashPlayer;
      }
    }
    frames += effects;
  }
}

void BattleAnimation::saveAnimation()
{
  XMLNode animationNode = XMLNode::createXMLTopNode("ptad-animation");
  XMLNode frameNode, effectNode;
  QStringList spriteList;
  for (int i = 0; i < 8; ++i)
    spriteList += QString::number(sprites[i]);
  animationNode.addAttribute("sprites", spriteList.join(',').toLocal8Bit().data());
  for (auto frame : frames)
  {
    frameNode = animationNode.addChild("frame");
    for (auto effect : frame)
    {
      if (effect->effect == EFFECT_SPRITE)
      {
        EffectSprite *sprite = static_cast<EffectSprite*>(effect);
        effectNode = frameNode.addChild("sprite");
        effectNode.addAttribute("spriteID", QString::number(sprite->spriteID).toLocal8Bit().data());
        effectNode.addAttribute("flipped", (sprite->flipped) ? "true":"false");
        effectNode.addAttribute("mirrored", (sprite->mirrored) ? "true":"false");
        effectNode.addAttribute("x", QString::number(sprite->x).toLocal8Bit().data());
        effectNode.addAttribute("y", QString::number(sprite->y).toLocal8Bit().data());
      }
      else if (effect->effect == EFFECT_PLAY_SFX)
      {
        EffectPlaySFX *playSFX = static_cast<EffectPlaySFX*>(effect);
        effectNode = frameNode.addChild("playSFX");
        effectNode.addAttribute("sfx", playSFX->sfx.toLocal8Bit().data());
      }
      else if (effect->effect == EFFECT_SHAKE_SCREEN)
      {
        EffectShakeScreen *shakeScreen = static_cast<EffectShakeScreen*>(effect);
        effectNode = frameNode.addChild("shakeScreen");
        effectNode.addAttribute("duration", QString::number(shakeScreen->duration).toLocal8Bit().data());
        effectNode.addAttribute("rate", QString::number(shakeScreen->rate).toLocal8Bit().data());
      }
      else if (effect->effect == EFFECT_FLASH_UI)
      {
        EffectFlashUI *flashUI = static_cast<EffectFlashUI*>(effect);
        effectNode = frameNode.addChild("flashUI");
        effectNode.addAttribute("duration", QString::number(flashUI->duration).toLocal8Bit().data());
        effectNode.addAttribute("color", QString::number(flashUI->color).toLocal8Bit().data());
      }
      else if (effect->effect == EFFECT_FLASH_BATTLER)
      {
        EffectFlashBattler *flashBattler = static_cast<EffectFlashBattler*>(effect);
        effectNode = frameNode.addChild("flashBattler");
        effectNode.addAttribute("duration", QString::number(flashBattler->duration).toLocal8Bit().data());
        effectNode.addAttribute("color1", QString::number(flashBattler->color1).toLocal8Bit().data());
        effectNode.addAttribute("color2", QString::number(flashBattler->color2).toLocal8Bit().data());
        effectNode.addAttribute("color3", QString::number(flashBattler->color3).toLocal8Bit().data());
      }
      else if (effect->effect == EFFECT_FLASH_PLAYER)
      {
        EffectFlashPlayer *flashPlayer = static_cast<EffectFlashPlayer*>(effect);
        effectNode = frameNode.addChild("flashPlayer");
        effectNode.addAttribute("duration", QString::number(flashPlayer->duration).toLocal8Bit().data());
        effectNode.addAttribute("color1", QString::number(flashPlayer->color1).toLocal8Bit().data());
        effectNode.addAttribute("color2", QString::number(flashPlayer->color2).toLocal8Bit().data());
        effectNode.addAttribute("color3", QString::number(flashPlayer->color3).toLocal8Bit().data());
      }
    }
  }
  animationNode.writeToFile(QString("%1/animations/%2.xml").arg(Globals::datadir).arg(fileName).toLocal8Bit().data());
}

QByteArray BattleAnimation::compileAnimation()
{
  QByteArray bytes;
  for (int i = 0; i < 8; ++i)
    bytes += sprites[i];
  for (auto frame : frames)
  {
    AnimationFrame data = {{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}};
    int i = 0;
    for (auto effect : frame)
    {
      data.effect[i] = effect->effect;
      if (effect->effect == EFFECT_SPRITE)
      {
        EffectSprite *sprite = static_cast<EffectSprite*>(effect);
        data.value1[i] = sprite->spriteID;
        data.value2[i] = ((sprite->flipped) ? SPRITE_FLIPPED:0) | ((sprite->mirrored) ? SPRITE_MIRRORED:0);
        data.value3[i] = sprite->x;
        data.value4[i] = sprite->y;
      }
      else if (effect->effect == EFFECT_PLAY_SFX)
      {
        EffectPlaySFX *playSFX = static_cast<EffectPlaySFX*>(effect);
        data.value1[i] = Globals::sfx.indexOf(playSFX->sfx);
      }
      else if (effect->effect == EFFECT_SHAKE_SCREEN)
      {
        EffectShakeScreen *shakeScreen = static_cast<EffectShakeScreen*>(effect);
        data.value1[i] = shakeScreen->duration;
        data.value2[i] = shakeScreen->rate;
      }
      else if (effect->effect == EFFECT_FLASH_UI)
      {
        EffectFlashUI *flashUI = static_cast<EffectFlashUI*>(effect);
        data.value1[i] = flashUI->duration;
        data.value2[i] = flashUI->color;
      }
      else if (effect->effect == EFFECT_FLASH_BATTLER)
      {
        EffectFlashBattler *flashBattler = static_cast<EffectFlashBattler*>(effect);
        data.value1[i] = flashBattler->duration;
        data.value2[i] = flashBattler->color1;
        data.value3[i] = flashBattler->color2;
        data.value4[i] = flashBattler->color3;
      }
      else if (effect->effect == EFFECT_FLASH_PLAYER)
      {
        EffectFlashPlayer *flashPlayer = static_cast<EffectFlashPlayer*>(effect);
        data.value1[i] = flashPlayer->duration;
        data.value2[i] = flashPlayer->color1;
        data.value3[i] = flashPlayer->color2;
        data.value4[i] = flashPlayer->color3;
      }
      ++i;
    }
    bytes += QByteArray::fromRawData(reinterpret_cast<const char*>(&data), sizeof(AnimationFrame));
  }
  for (int i = frames.count(); i < 51; ++i)
    bytes.append(40, 0);
  return bytes;
}
