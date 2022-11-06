#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

extern uint32_t cameraX;
extern uint32_t cameraY;

DataPack *PTAD::dataFile;
uint32_t PTAD::frameCount = 0;
uint32_t PTAD::fpsStart = 0;
uint32_t PTAD::fps = 0;
int32_t PTAD::globalCounter = 0;
uint8_t PTAD::buttonsPreviousState = 0;
uint8_t PTAD::buttonsJustPressed = 0;
uint8_t PTAD::battleMonsterID = 255;
uint8_t PTAD::battleBG = 0;
uint8_t PTAD::battleBGM = 0;
uint8_t PTAD::darkness = 0;
uint8_t __attribute__((section (".bss"))) __attribute__ ((aligned)) PTAD::memory[PTAD::MEMORY_SIZE];
uint8_t *PTAD::tiles = PTAD::memory + PTAD::MEMORY_TILESET_OFFSET;
#ifndef POK_SIM
uint8_t *PTAD::tilemapBG = (uint8_t*)PTAD::TILEMAPBG_MEMORY_ADDRESS;
uint8_t *PTAD::tilemapFG = (uint8_t*)PTAD::TILEMAPFG_MEMORY_ADDRESS;
#else
uint8_t PTAD::tilemapBG[PTAD::TILEMAP_MEMORY_SIZE];
uint8_t PTAD::tilemapFG[PTAD::TILEMAP_MEMORY_SIZE];
#endif
const uint8_t PTAD::volumeLevels[8] = {0,32,64,96,128,160,192,224};

bool PTAD::justPressed(uint8_t mask)
{
  if ((buttonsJustPressed & mask) != 0)
    return true;
  return false;
}

void PTAD::loadTileset(uint8_t tilesetID)
{
  DataPack::PackedFile file;
  dataFile->getPackedFile(PTAD::Resources::tilesets[tilesetID], &file);
  dataFile->readBytes(&file, tiles, PTAD::MEMORY_TILESET_SIZE);
}

void PTAD::drawTile(uint8_t x, uint8_t y, uint8_t tileID, bool bg)
{
  if (bg)
    tilemapBG[y * tilemapWidth + x] = tileID;
  else
    tilemapFG[y * tilemapWidth + x] = tileID;
}

void PTAD::drawTiles(const uint8_t *array, int numTiles, int x, int y, bool bg)
{
  if (y < 0) return;
  if (y > 22) return;
  if (x < 0)
  {
    array -= x;
    numTiles += x;
    x = 0;
  }
  if (x + numTiles >= 29)
    numTiles = 29 - x;
  uint8_t *tile = ((bg) ? tilemapBG:tilemapFG) + y * tilemapWidth + x;
  while (numTiles-- > 0)
    *tile++ = *array++;
  //PD::drawTile(x++, y, tileArray + *array++ * 66 + 2);
}

void PTAD::clearTiles(int32_t screenX, int32_t screenY)
{
  uint8_t *pTile = tilemapBG;
  for (int y = 0; y < tilemapHeight; ++y)
  {
    int tileY = (screenY + 128) / 8 % 2;
    for (int x = 0; x < tilemapWidth; ++x)
    {
      int tileX = (screenX + 128) / 8 % 2;
      *pTile++ = ((x + tileX) % 2) + (((y + tileY) % 2) * 16);
      //PD::drawTile(x, y, tiles + (((x + tileX) % 2) * 66) + (((y + tileY) % 2) * 16 * 66 + 2));
    }
  }
  memset(tilemapFG, 0xFF, tilemapWidth * tilemapHeight);
}

void PTAD::tileFillerBG(uint8_t *line, uint32_t y, bool skip) noexcept
{
  uint8_t *pTileIndex = tilemapBG + (y + cameraY) / 8 * tilemapWidth;
  uint32_t tileX = cameraX;
  uint32_t tileY = ((y + cameraY) % 8) * 8;
  for (uint32_t i = 0; i < screenWidth;)
  {
    int iter = std::min(8 - tileX, screenWidth - i);
    pixelCopySolid(line+i, tiles + tileY + *pTileIndex * 64 + tileX, iter);
    ++pTileIndex;
    i += iter;
    tileX = 0;
  }
}

void PTAD::tileFillerFG(uint8_t *line, uint32_t y, bool skip) noexcept
{
  uint8_t *pTileIndex = tilemapFG + (y + cameraY) / 8 * tilemapWidth;
  uint32_t tileX = cameraX;
  uint32_t tileY = ((y + cameraY) % 8) * 8;
  for (uint32_t i = 0; i < screenWidth;)
  {
    int iter = std::min(8 - tileX, screenWidth - i);
    if (*pTileIndex != 255)
      pixelCopy(line+i, tiles + tileY + *pTileIndex * 64 + tileX, iter);
    ++pTileIndex;
    i += iter;
    tileX = 0;
  }
  if (darkness == 1)
  {
    uint8_t *pixel = line;
    for (uint8_t i = 0; i < screenWidth; ++i, ++pixel)
      *pixel = (*pixel & 7) / 4;
  }
  else if (darkness == 2)
  {
    uint8_t *pixel = line;
    if (PTAD::Game::player.dir == DIR_UP)
    {
      uint8_t y2 = y + 33;
      for (uint8_t i = 0; i < screenWidth; ++i, ++pixel)
      {
        if (i < y2 || i >= screenWidth - y2)
          *pixel = (*pixel & 0xF8) + ((*pixel & 7) / 2);
      }
    }
    else if (PTAD::Game::player.dir == DIR_RIGHT)
    {
      uint8_t y2 = y + 29;
      for (uint8_t i = 0; i < screenWidth; ++i, ++pixel)
      {
        if (i < y2 || i < screenWidth - y2)
          *pixel = (*pixel & 0xF8) + ((*pixel & 7) / 2);
      }
    }
    else if (PTAD::Game::player.dir == DIR_DOWN)
    {
      uint8_t y2 = y + 30;
      for (uint8_t i = 0; i < screenWidth; ++i, ++pixel)
      {
        if (i < screenWidth - y2 || i > y2)
          *pixel = (*pixel & 0xF8) + ((*pixel & 7) / 2);
      }
    }
    else if (PTAD::Game::player.dir == DIR_LEFT)
    {
      uint8_t y2 = y + 29;
      for (uint8_t i = 0; i < screenWidth; ++i, ++pixel)
      {
        if (i > y2 || i > screenWidth - y2)
          *pixel = (*pixel & 0xF8) + ((*pixel & 7) / 2);
      }
    }
  }
  else if (darkness == 3)
  {
    uint8_t *pixel = line;
    for (uint8_t i = 0; i < screenWidth; ++i, ++pixel)
      *pixel = (*pixel & 0xF8) + ((*pixel & 7) / 2);
  }
}

void PTAD::setup_data_16(uint16_t data)
{
#ifndef POK_SIM
  SET_MASK_P2;
  LPC_GPIO_PORT->MPIN[2] = (data<<3); // write bits to port
  CLR_MASK_P2;
#endif
}

void PTAD::write_command_16(uint16_t data)
{
  #ifndef POK_SIM
  CLR_CS; // select lcd
  CLR_CD; // clear CD = command
  SET_RD; // RD high, do not read
  setup_data_16(data); // function that inputs the data into the relevant bus lines
  CLR_WR_SLOW;  // WR low
  SET_WR;  // WR low, then high = write strobe
  SET_CS; // de-select lcd
  #endif
}

void PTAD::write_data_16(uint16_t data)
{
#ifndef POK_SIM
  CLR_CS;
  SET_CD;
  SET_RD;
  setup_data_16(data);
  CLR_WR;
  SET_WR;
  SET_CS;
#endif
}

void PTAD::fillRegion(uint32_t count, uint16_t color)
{
#ifndef POK_SIM
  write_command(0x22); // write data to DRAM
  setup_data_16(color);
  CLR_CS_SET_CD_RD_WR;
  while (count--)
  {
    CLR_WR;
    SET_WR;
  }
#endif
}
