#pragma once

#include "headers.h"

namespace PTAD
{
  class GameOver
  {
    public:
      static void load(uint32_t hash);
      static void setup();
      static void update();
    private:
      static DataPack::PackedFile screen;
  };
};
