#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

extern const uint8_t *tilemap[];

/*const uint8_t PTAD::Dialog::messages[] =
{
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x30,0x3B,0x34,0x47,0x0A,0x34,0x36,0x47,0x3C,0x42,0x41,0x0A,0x37,0x42,0x0A,0x4C,0x42,0x48,0x0A,0x46,0x38,0x38,0x3E,0x05,
  0x22,0x2D,0x04,0x2C,0x0A,0x1A,0x0A,0x2C,0x2E,0x2B,0x29,0x2B,0x22,0x2C,0x1E,0x0A,0x1A,0x2D,0x2D,0x1A,0x1C,0x24,0x00,0x0A,
  0x1C,0x2B,0x22,0x2D,0x22,0x1C,0x1A,0x25,0x0A,0x21,0x22,0x2D,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x3A,0x42,0x3F,0x37,0x0A,0x38,0x34,0x45,0x41,0x38,0x37,0x02,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x38,0x4B,0x43,0x38,0x45,0x3C,0x38,0x41,0x36,0x38,0x0A,0x3A,0x34,0x3C,0x41,0x38,0x37,0x02,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x28,0x2E,0x0A,0x25,0x1E,0x2F,0x1E,0x25,0x1E,0x1D,0x0A,0x2E,0x29,0x00,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x25,0x38,0x34,0x45,0x41,0x38,0x37,0x0A,0x46,0x3E,0x3C,0x3F,0x3F,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x47,0x42,0x42,0x3E,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x37,0x34,0x40,0x34,0x3A,0x38,0x02,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x20,0x38,0x3B,0x38,0x41,0x41,0x34,0x0A,0x34,0x4A,0x34,0x3C,0x47,0x46,0x0A,0x4C,0x42,0x48,0x02,0x02,0x02,0x02,0x02,0x02,
  0x32,0x42,0x48,0x0A,0x46,0x47,0x45,0x3C,0x3E,0x38,0x0A,0x4C,0x42,0x48,0x45,0x0A,0x42,0x43,0x43,0x42,0x41,0x38,0x41,0x47,
  0x32,0x42,0x48,0x0A,0x41,0x34,0x45,0x45,0x42,0x4A,0x3F,0x4C,0x0A,0x38,0x46,0x36,0x34,0x43,0x38,0x37,0x02,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x39,0x34,0x3C,0x3F,0x38,0x37,0x0A,0x47,0x42,0x0A,0x45,0x48,0x41,0x0A,0x34,0x4A,0x34,0x4C,0x02,0x0A,
  0x2D,0x3B,0x38,0x45,0x38,0x04,0x46,0x0A,0x41,0x42,0x4A,0x3B,0x38,0x45,0x38,0x0A,0x47,0x42,0x0A,0x45,0x48,0x41,0x02,0x0A,
  0x32,0x42,0x48,0x0A,0x37,0x38,0x34,0x3F,0x47,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x26,0x22,0x2C,0x2C,0x00,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x29,0x42,0x3C,0x46,0x42,0x41,0x0A,0x37,0x38,0x34,0x3F,0x46,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x2D,0x3B,0x3C,0x46,0x0A,0x3C,0x46,0x0A,0x47,0x3B,0x38,0x0A,0x37,0x3C,0x34,0x3F,0x42,0x3A,0x0A,0x46,0x43,0x38,0x38,0x37,
  0x32,0x28,0x2E,0x04,0x2B,0x1E,0x0A,0x1B,0x25,0x22,0x27,0x1D,0x1E,0x1D,0x0A,0x1B,0x32,0x0A,0x2B,0x1A,0x20,0x1E,0x00,0x0A,
  0x32,0x42,0x48,0x0A,0x48,0x46,0x38,0x37,0x0A,0x34,0x0A,0x22,0x2D,0x1E,0x26,0x27,0x1A,0x26,0x1E,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x48,0x46,0x38,0x37,0x0A,0x2C,0x24,0x22,0x25,0x27,0x1A,0x26,0x1E,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x36,0x34,0x46,0x47,0x0A,0x2C,0x29,0x1E,0x25,0x27,0x1A,0x26,0x1E,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x2D,0x3B,0x38,0x0A,0x38,0x41,0x38,0x40,0x4C,0x0A,0x3C,0x46,0x0A,0x43,0x42,0x3C,0x46,0x42,0x41,0x38,0x37,0x0A,0x0A,0x0A,
  0x2D,0x3B,0x38,0x0A,0x43,0x42,0x3C,0x46,0x42,0x41,0x0A,0x37,0x3C,0x37,0x41,0x04,0x47,0x0A,0x3B,0x42,0x3F,0x37,0x0A,0x0A,
  0x2D,0x3B,0x38,0x0A,0x38,0x41,0x38,0x40,0x4C,0x0A,0x40,0x42,0x49,0x38,0x46,0x0A,0x46,0x3F,0x42,0x4A,0x38,0x45,0x0A,0x0A,
  0x2D,0x3B,0x38,0x0A,0x46,0x3F,0x42,0x4A,0x41,0x38,0x46,0x46,0x0A,0x37,0x3C,0x37,0x41,0x04,0x47,0x0A,0x4A,0x42,0x45,0x3E,
  0x32,0x42,0x48,0x0A,0x45,0x38,0x36,0x42,0x49,0x38,0x45,0x38,0x37,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x39,0x38,0x38,0x3F,0x0A,0x49,0x38,0x45,0x4C,0x0A,0x45,0x38,0x39,0x45,0x38,0x46,0x3B,0x38,0x37,0x00,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x39,0x42,0x36,0x48,0x46,0x38,0x37,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x40,0x42,0x45,0x38,0x0A,0x39,0x42,0x36,0x48,0x46,0x38,0x37,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x46,0x48,0x43,0x38,0x45,0x0A,0x39,0x42,0x36,0x48,0x46,0x38,0x37,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x39,0x48,0x45,0x3C,0x42,0x48,0x46,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x40,0x42,0x45,0x38,0x0A,0x39,0x48,0x45,0x3C,0x42,0x48,0x46,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x46,0x48,0x43,0x38,0x45,0x0A,0x39,0x48,0x45,0x3C,0x42,0x48,0x46,
  0x32,0x42,0x48,0x0A,0x39,0x38,0x38,0x3F,0x0A,0x3F,0x38,0x46,0x46,0x0A,0x39,0x48,0x45,0x3C,0x42,0x48,0x46,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x04,0x45,0x38,0x0A,0x40,0x42,0x49,0x38,0x40,0x38,0x41,0x47,0x0A,0x3C,0x46,0x0A,0x39,0x34,0x46,0x47,0x00,
  0x32,0x42,0x48,0x0A,0x45,0x38,0x36,0x42,0x49,0x38,0x45,0x0A,0x39,0x45,0x42,0x40,0x0A,0x43,0x42,0x3C,0x46,0x42,0x41,0x02,
  0x32,0x42,0x48,0x0A,0x36,0x34,0x41,0x0A,0x40,0x42,0x49,0x38,0x0A,0x41,0x42,0x45,0x40,0x34,0x3F,0x3F,0x4C,0x02,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x46,0x47,0x45,0x42,0x41,0x3A,0x38,0x45,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x47,0x42,0x48,0x3A,0x3B,0x38,0x45,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x46,0x4A,0x3C,0x39,0x47,0x38,0x45,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x32,0x42,0x48,0x0A,0x35,0x38,0x36,0x42,0x40,0x38,0x0A,0x40,0x42,0x45,0x38,0x0A,0x43,0x42,0x4A,0x38,0x45,0x39,0x48,0x3F
};
const uint8_t PTAD::Dialog::messageSizes[] = {24,24,23,13,13,19,15,14,9,8,24,24,21,23,23,10,5,13,24,23,11,9,9,21,22,22,24,14,24,18,23,24,18,23,24,21,24,24,22,19,18,18,24};*/

//DataPack::PackedFile PTAD::Dialog::messagesFile;
uint8_t PTAD::Dialog::buffer[MESSAGES_SIZE];
uint8_t PTAD::Dialog::counter = 0;
uint8_t PTAD::Dialog::messageSize = 0;
uint8_t PTAD::Dialog::messageSpeed = 0;

void PTAD::Dialog::setup()
{
//	PTAD::dataFile->getPackedFile(DataPack::hash("messages.dat"), &messagesFile);
}

void PTAD::Dialog::showWindow()
{
	PTAD::Ui::drawFrame(1, 15, 26, 20);
}

void PTAD::Dialog::shiftTextUp()
{
	for (int y = 16; y <= 18; ++y)
		PTAD::Ui::shiftUp(2, y, 24);
	PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
}

void PTAD::Dialog::bufferText(const uint8_t *array, uint8_t numChars)
{
	while (numChars-- > 0)
		buffer[messageSize++] = *array++;
}

void PTAD::Dialog::bufferNumber(int32_t value, int32_t start)
{
  if (value < 0)
  {
    buffer[messageSize++] = PTAD::FONT_MINUS;
    value *= -1;
  }
	while (start > 0)
	{
		if (value >= start || start == 1)
      buffer[messageSize++] = PTAD::FONT_0 + (value / start) % 10;
		start /= 10;
	}
	buffer[messageSize++] = PTAD::FONT_SPACE;
}

void PTAD::Dialog::bufferCharacter(uint8_t character)
{
  buffer[messageSize++] = character;
}

void PTAD::Dialog::trimBuffer()
{
  do
  {
    --messageSize;
  } while (buffer[messageSize] == PTAD::FONT_SPACE);
  ++messageSize;
}

void PTAD::Dialog::addMessage(uint8_t id)
{
  for (uint8_t i = 0; i < PTAD::Resources::messageSizes[id]; ++i)
    buffer[messageSize++] = PTAD::Resources::messages[id * 24 + i];
}

void PTAD::Dialog::beginMessage(bool shiftUp)
{
  memset(buffer + messageSize, PTAD::FONT_SPACE, MESSAGES_SIZE - messageSize);
  if (shiftUp)
    shiftTextUp();
  else
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
  PTAD::Ui::dialogCursorX = 15;
  PTAD::Ui::dialogCursorY = 153;
  PTAD::Ui::dialogCursor = PTAD::Ui::DIALOG_CURSOR_DRAW;
	counter = messageSize * messageSpeed;
	if (messageSpeed == 0)
  {
    PTAD::Ui::drawText(buffer, messageSize, 2, 19);
    PTAD::Ui::dialogCursorX += messageSize * 8;
    messageSize = 0;
    PTAD::Ui::dialogCursor |= PTAD::Ui::DIALOG_CURSOR_BLINK;
  }
}

bool PTAD::Dialog::updateMessage(bool skippable)
{
  int bufferPos = messageSize - (counter / messageSpeed);
	if (counter == 0)
		return false;
	--counter;
  if (bufferPos != messageSize - (counter / messageSpeed))
  {
    if (buffer[bufferPos] != PTAD::FONT_SPACE)
      PTAD::Music::playSFX(PTAD::Resources::sfx_dialog, false);
    PTAD::Ui::dialogCursorX += 8;
  }
	if (skippable && (PB::bBtn() || PB::cBtn() || PTAD::justPressed(PTAD::BTN_MASK_A)))
  {
    PTAD::Ui::dialogCursorX = 15 + messageSize * 8;
		counter = 0;
  }
	PTAD::Ui::drawText(buffer, messageSize - (counter / messageSpeed), 2, 19);
	if (counter == 0)
  {
    memset(buffer, PTAD::FONT_SPACE, sizeof(buffer));
    messageSize = 0;
    PTAD::Ui::dialogCursor |= PTAD::Ui::DIALOG_CURSOR_BLINK;
  }
	return true;
}

void PTAD::Dialog::stopMessage()
{
  memset(buffer, PTAD::FONT_SPACE, sizeof(buffer));
  PTAD::Ui::dialogCursor = 0;
  messageSize = 0;
  counter = 0;
}

void PTAD::Dialog::setMessageSpeed(uint8_t speed)
{
  messageSpeed = speed;
}

uint8_t PTAD::Dialog::getMessageSpeed()
{
  return messageSpeed;
}
