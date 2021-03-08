#include <cmath>
#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

DataPack::PackedFile PTAD::GameOver::screen;

void PTAD::GameOver::load(uint32_t hash)
{
  PTAD::dataFile->getPackedFile(hash, &screen);
}

void PTAD::GameOver::setup()
{
  uint8_t scanline[220];
  PD::loadRGBPalette(miloslav);
  PD::setTASRowMask(0);
  Pokitto::lcdPrepareRefresh();
  for (int y = 0; y < 176; ++y)
  {
    PTAD::dataFile->readBytes(&screen, (void*)scanline, 220);
    flushLine(PD::palette, scanline);
  }
}

void PTAD::GameOver::update()
{
  if (PTAD::justPressed(BTN_MASK_A) || PTAD::justPressed(BTN_MASK_B))
    PTAD::Game::state = PTAD::Game::State::TitleInit;
}
