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
}

void update()
{
	PB::pollButtons();
	PTAD::buttonsJustPressed = ~PTAD::buttonsPreviousState & PB::buttons_state;
	PTAD::buttonsPreviousState = PB::buttons_state;
	PTAD::Game::update();
}
