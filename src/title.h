#pragma once

#include "headers.h"

namespace PTAD
{
  class Title
  {
    public:
      static void setup();
      static void update();
      static void lineFiller(uint8_t *line, uint32_t y, bool skip) noexcept;
    private:
      static const uint8_t txtContinue[];
      static const uint8_t txtNewGame[];
      static uint8_t *waveBuffer;
      static uint8_t *menuBuffer;
      static int8_t *waveOffset;
      static constexpr uint32_t rowMask = (1 << 9) + (1 << 10) + (1 << 11) + (1 << 12) + (1 << 13) + (1 << 15) + (1 << 16) + (1 << 17) + (1 << 18);
      static constexpr uint8_t waveStart = 79;
      static constexpr uint8_t waveEnd = 123;
      static constexpr uint8_t menuStart = 133;
      static constexpr uint8_t menuEnd = 168;
  };
};
