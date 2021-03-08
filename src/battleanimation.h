#pragma once

#include "headers.h"

namespace PTAD
{
  class BattleAnimation
  {
    public:
      struct AnimationFrame
      {
        uint8_t effect[8];
        uint8_t value1[8];
        uint8_t value2[8];
        uint8_t value3[8];
        uint8_t value4[8];
      };
      //effect=SPRITE: x, y, spriteID, flipped/mirrored
      //effect=SFX: sfxID, N/A, N/A, N/A
      //effect=SHAKE: duration, rate, N/A, N/A
      //effect=FLASH_UI: duration, color, N/A, N/A
      //effect=FLASH_BATTLER: duration, color1, color2, color3
      //effect=FLASH_PLAYER: duration, color1, color2, color3
      struct AnimationData
      {
        uint8_t sprites[8];
        AnimationFrame frames[51];
      };
      static void beginAnimation(uint32_t animationHash);
      static void renderFrame();
      static bool isAnimationPlaying();
    private:
      static bool isNullFrame();
      #ifdef POK_SIM
      static AnimationData _animation;
      #endif
      static AnimationData *animation;
      static const uint32_t animations[];
      static uint8_t *sprites;
      static uint8_t currentFrame;
      static uint8_t nextFrame;
      static constexpr uint8_t SPRITE_FLIPPED = 1;
      static constexpr uint8_t SPRITE_MIRRORED = 2;
      static constexpr uint8_t EFFECT_NONE = 0;
      static constexpr uint8_t EFFECT_SPRITE = 1;
      static constexpr uint8_t EFFECT_PLAY_SFX = 2;
      static constexpr uint8_t EFFECT_SHAKE_SCREEN = 3;
      static constexpr uint8_t EFFECT_FLASH_UI = 4;
      static constexpr uint8_t EFFECT_FLASH_BATTLER = 5;
      static constexpr uint8_t EFFECT_FLASH_PLAYER = 6;
  };
};
