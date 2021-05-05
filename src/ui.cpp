#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

extern uint32_t cameraX;
extern uint32_t cameraY;

int32_t PTAD::Ui::cursorX = 0;
int32_t PTAD::Ui::cursorY = -16;
uint8_t PTAD::Ui::dialogCursorX = 0;
uint8_t PTAD::Ui::dialogCursorY = 255;
uint8_t PTAD::Ui::bgColor = 0;
uint8_t PTAD::Ui::fgColor = 7;
uint8_t PTAD::Ui::dialogCursor = 0;
uint8_t PTAD::Ui::uimap[PTAD::UIMAP_MEMORY_SIZE];

void PTAD::Ui::clear()
{
	memset(uimap, 255, PTAD::UIMAP_MEMORY_SIZE);
  dialogCursorX = 0;
  dialogCursorY = 255;
}

void PTAD::Ui::fill(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t character)
{
  for (uint8_t y = startY; y <= endY; ++y)
    fillCharacter(character, endX - startX + 1, startX, y);
}

void PTAD::Ui::drawFrame(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY)
{
	uimap[startY * PTAD::tilemapWidth + startX] = PTAD::FONT_FRAME_UL;
	uimap[startY * PTAD::tilemapWidth + endX] = PTAD::FONT_FRAME_UR;
	uimap[endY * PTAD::tilemapWidth + startX] = PTAD::FONT_FRAME_BL;
	uimap[endY * PTAD::tilemapWidth + endX] = PTAD::FONT_FRAME_BR;
	for (uint8_t x = startX + 1; x < endX; ++x)
	{
		uimap[startY * PTAD::tilemapWidth + x] = PTAD::FONT_FRAME_U;
		uimap[endY * PTAD::tilemapWidth + x] = PTAD::FONT_FRAME_B;
	}
	for (uint8_t y = startY + 1; y < endY; ++y)
	{
		uimap[y * PTAD::tilemapWidth + startX] = PTAD::FONT_FRAME_L;
		uimap[y * PTAD::tilemapWidth + endX] = PTAD::FONT_FRAME_R;
	}
	for (uint8_t y = startY + 1; y < endY; ++y)
		fillCharacter(PTAD::FONT_FRAME_C, endX - startX - 1, startX + 1, y);
}

void PTAD::Ui::fixFrameHeader(uint8_t startX, uint8_t endX, uint8_t y)
{
  uint8_t *ui = uimap + y * PTAD::tilemapWidth;
  for (uint8_t x = startX; x <= endX; ++x)
  {
    if (ui[x] < PTAD::FONT_FRAME_UL || (ui[x] > PTAD::FONT_FRAME_UR && ui[x] < PTAD::FONT_FRAME_UL_L))
    {
      if (ui[x - 1] == PTAD::FONT_FRAME_UL)
        ui[x - 1] = PTAD::FONT_FRAME_UL_L;
      else if (ui[x - 1] == PTAD::FONT_FRAME_U)
        ui[x - 1] = PTAD::FONT_FRAME_U_L;
      else if (ui[x - 1] == PTAD::FONT_FRAME_U_R)
        ui[x - 1] = PTAD::FONT_FRAME_U_LR;
      if (ui[x + 1] == PTAD::FONT_FRAME_UR)
        ui[x + 1] = PTAD::FONT_FRAME_UR_R;
      else if (ui[x + 1] == PTAD::FONT_FRAME_U)
        ui[x + 1] = PTAD::FONT_FRAME_U_R;
    }
  }
}

void PTAD::Ui::drawText(const uint8_t *array, uint8_t numChars, uint8_t x, uint8_t y)
{
	uint8_t *p = uimap + y * PTAD::tilemapWidth + x;
	while (numChars-- > 0)
		*p++ = *array++;
}

void PTAD::Ui::drawNumber(uint32_t value, uint8_t x, uint8_t y, uint32_t start)
{
	uint8_t *p = uimap + y * PTAD::tilemapWidth + x;
	while (start > 0)
	{
		if (value >= start || start == 1)
			*p++ = PTAD::FONT_0 + (value / start) % 10;
		else
			*p++ = PTAD::FONT_SPACE;
		start /= 10;
	}
}

void PTAD::Ui::drawCharacter(uint8_t character, uint8_t x, uint8_t y)
{
	uimap[y * PTAD::tilemapWidth + x] = character;
}

void PTAD::Ui::fillCharacter(uint8_t character, uint8_t numChars, uint8_t x, uint8_t y)
{
	uint8_t *p = uimap + y * PTAD::tilemapWidth + x;
	while (numChars-- > 0)
		*p++ = character;
}

void PTAD::Ui::shiftUp(uint8_t x, uint8_t y, uint8_t numChars)
{
	uint32_t index1 = y * PTAD::tilemapWidth + x;
	uint32_t index2 = y * PTAD::tilemapWidth + PTAD::tilemapWidth + x;
	while (numChars-- > 0)
		uimap[index1++] = uimap[index2++];
}

void PTAD::Ui::backupSection(uint8_t *buffer, uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY)
{
  for (uint8_t y = startY; y <= endY; ++y)
  {
    for (uint8_t x = startX; x <= endX; ++x)
      *buffer++ = uimap[y * PTAD::tilemapWidth + x];
  }
}

void PTAD::Ui::restoreBackup(const uint8_t *buffer, uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY)
{
  for (uint8_t y = startY; y <= endY; ++y)
  {
    for (uint8_t x = startX; x <= endX; ++x)
      uimap[y * PTAD::tilemapWidth + x] = *buffer++;
  }
}

uint8_t PTAD::Ui::getCharacter(uint8_t x, uint8_t y)
{
  return uimap[y * PTAD::tilemapWidth + x];
}

void PTAD::Ui::lineFiller(uint8_t *line, uint32_t y, bool skip) noexcept
{
	uint8_t mask = 1 << ((y + cameraY) % 8);
	uint32_t tileIndex = (y + cameraY) / 8 * PTAD::tilemapWidth;
	uint32_t tileX = cameraX;
  int32_t finalX, finalY;
	for (uint32_t i = 0; i < PTAD::screenWidth;)
	{
		int iter = std::min(8 - tileX, screenWidth - i);
		uint8_t tile = uimap[tileIndex++];
		if (tile != 255)
		{
			for (uint32_t j = 0; j < iter; ++j)
        line[i + j] = (PTAD::font[tile * 8 + j + tileX] & mask) ? fgColor:bgColor;
		}
		i += iter;
		tileX = 0;
	}
	finalX = cursorX - cameraX + 2;
  finalY = cursorY - cameraY;
  if (y >= finalY && y < finalY + 16)
    pixelCopy(line + finalX, PTAD::cursor + (y - finalY) * 16, 16);
  else if (y >= finalY + 16)
    cursorY = -16;
  finalX = dialogCursorX - cameraX + 2;
  finalY = dialogCursorY - cameraY;
  if (y >= finalY && y < finalY + 6 && (dialogCursor & DIALOG_CURSOR_DRAW) != 0)
    line[finalX] = fgColor;
}
