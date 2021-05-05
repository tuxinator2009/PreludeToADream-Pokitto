#include "src/headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

void init()
{
	PD::loadRGBPalette(miloslav);
  Pokitto::lcdFillSurface(0);
  PD::setTASRowMask(0);
	PD::persistence = true;
	PD::invisiblecolor = 0;
  PTAD::Game::setup();
  PB::pollButtons();
  while (PB::buttons_state != 0)
    PB::pollButtons();
  if (Pokitto::discrete_vol == 7)
  {
    Pokitto::discrete_vol = 6;
    Audio::setVolume(Pokitto::discrete_vol_levels[Pokitto::discrete_vol]);
  }
}

void update()
{
	PB::pollButtons();
	PTAD::buttonsJustPressed = ~PTAD::buttonsPreviousState & PB::buttons_state;
	PTAD::buttonsPreviousState = PB::buttons_state;
	PTAD::Game::update();
}
