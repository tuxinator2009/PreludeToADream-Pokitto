#pragma once

#include "headers.h"

namespace PTAD
{
	class Ui
	{
		public:
      static constexpr uint8_t DIALOG_CURSOR_DRAW = 1;
      static constexpr uint8_t DIALOG_CURSOR_BLINK = 2;
      static void clear();
      static void fill(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t character);
			static void drawFrame(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY);
      static void fixFrameHeader(uint8_t startX, uint8_t endX, uint8_t y);
			static void drawText(const uint8_t *array, uint8_t numChars, uint8_t x, uint8_t y);
			static void drawNumber(uint32_t value, uint8_t x, uint8_t y, uint32_t start);
			static void drawCharacter(uint8_t character, uint8_t x, uint8_t y);
			static void fillCharacter(uint8_t character, uint8_t numChars, uint8_t x, uint8_t y);
			static void shiftUp(uint8_t x, uint8_t y, uint8_t numChars);
      static void backupSection(uint8_t *buffer, uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY);
      static void restoreBackup(const uint8_t *buffer, uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY);
      static uint8_t getCharacter(uint8_t x, uint8_t y);
      static void lineFiller(uint8_t *line, uint32_t y, bool skip) noexcept;
      static int32_t cursorX;
      static int32_t cursorY;
      static uint8_t dialogCursorX;
      static uint8_t dialogCursorY;
			static uint8_t bgColor;
      static uint8_t fgColor;
      static uint8_t dialogCursor;
		private:
      static uint8_t uimap[PTAD::UIMAP_MEMORY_SIZE];
      static constexpr uint8_t PLAYER_STATUS_X = 22;
      static constexpr uint8_t PLAYER_STATUS_Y = 88;
      static constexpr uint8_t ENEMY_STATUS_X = 126;
      static constexpr uint8_t ENEMY_STATUS_Y = 48;
	};
}
