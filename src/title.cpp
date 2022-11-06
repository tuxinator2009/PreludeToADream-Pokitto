#include <cmath>
#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

const uint8_t PTAD::Title::txtContinue[] = {PTAD::FONT_C, PTAD::FONT_o, PTAD::FONT_n, PTAD::FONT_t, PTAD::FONT_i, PTAD::FONT_n, PTAD::FONT_u, PTAD::FONT_e};
const uint8_t PTAD::Title::txtNewGame[] = {PTAD::FONT_N, PTAD::FONT_e, PTAD::FONT_w, PTAD::FONT_SPACE, PTAD::FONT_G, PTAD::FONT_a, PTAD::FONT_m, PTAD::FONT_e};
uint8_t *PTAD::Title::waveBuffer = PTAD::memory + PTAD::MEMORY_TITLE_WAVE_BUFFER_OFFSET;
uint8_t *PTAD::Title::menuBuffer = PTAD::memory + PTAD::MEMORY_TITLE_MENU_BUFFER_OFFSET;
int8_t *PTAD::Title::waveOffset = (int8_t*)(PTAD::memory + PTAD::MEMORY_TITLE_WAVE_OFFSET);

void PTAD::Title::setup()
{
  DataPack::PackedFile file;
  uint8_t scanline[220];
  PTAD::dataFile->getPackedFile(DataPack::hash("/screens/title.png"), &file);
  Pokitto::lcdPrepareRefresh();
  for (int y = 0; y < 176; ++y)
  {
    PTAD::dataFile->readBytes(&file, (void*)scanline, 220);
    flushLine(PD::palette, scanline);
  }
  for (uint8_t i = 0; i < PTAD::MEMORY_TITLE_WAVE_SIZE; ++i)
    waveOffset[i] = sin(PC::getTime() / 250.0 + i);
  file.seek(220 * waveStart);
  PTAD::dataFile->readBytes(&file, (void*)waveBuffer, PTAD::MEMORY_TITLE_WAVE_BUFFER_SIZE);
  file.seek(220 * menuStart);
  PTAD::dataFile->readBytes(&file, (void*)menuBuffer, PTAD::MEMORY_TITLE_MENU_BUFFER_SIZE);
  PD::setTASRowMask(rowMask);
  PTAD::Ui::clear();
  PTAD::Ui::drawFrame(9, 17, 18, 20);
  PTAD::Ui::drawText(txtContinue, 8, 10, 18);
  PTAD::Ui::drawText(txtNewGame, 8, 10, 19);
  PTAD::Music::playMusic(PTAD::Resources::music_mainTheme, 0);
}

void PTAD::Title::update()
{
  for (uint8_t i = 0; i < PTAD::MEMORY_TITLE_WAVE_SIZE; ++i)
    waveOffset[i] = sin(PC::getTime() / 250.0 + i / 8.0) * 4;
  if (PTAD::justPressed(PTAD::BTN_MASK_UP) || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
    PTAD::globalCounter ^= 1;
  }
  if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PC::initRandom();
    PD::setTASRowMask(0xFFFFFFFF);
    PTAD::Music::playSFX(PTAD::Resources::sfx_select);
    if (PTAD::globalCounter == 0) //Continue
    {
      PTAD::Menu::setup(PTAD::Menu::State::LoadMenu);
      PTAD::Game::state = PTAD::Game::State::MenuInit;
    }
    else //New Game
    {
      PTAD::Menu::setup(PTAD::Menu::State::NameEntryMenu);
      PTAD::Game::state = PTAD::Game::State::MenuInit;
    }
  }
  if (PTAD::Game::state == PTAD::Game::State::Title)
  {
    PTAD::Ui::cursorX = 62;
    PTAD::Ui::cursorY = 144 + PTAD::globalCounter * 8;
  }
}

void PTAD::Title::lineFiller(uint8_t *line, uint32_t y, bool skip) noexcept
{
  if (y >= waveStart && y <= waveEnd)
  {
    memset(line, 0, 220);
    if (waveOffset[y - waveStart] < 0)
      pixelCopySolid(line, waveBuffer + (y - waveStart) * 220 - waveOffset[y - waveStart], 220 + waveOffset[y - waveStart]);
    else
      pixelCopySolid(line + waveOffset[y - waveStart], waveBuffer + (y - waveStart) * 220, 220 - waveOffset[y - waveStart]);
  }
  else if (y >= menuStart && y <= menuEnd)
    pixelCopySolid(line, menuBuffer + (y - menuStart) * 220, 220);
}
