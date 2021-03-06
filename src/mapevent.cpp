#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

bool (*const PTAD::MapEvent::execEvent[]) () =
{
  PTAD::MapEvent::hideScreen,          //0x00 ( 0)
  PTAD::MapEvent::showScreen,          //0x01 ( 1)
  PTAD::MapEvent::teleport,            //0x02 ( 2)
  PTAD::MapEvent::showDialog,          //0x03 ( 3)
  PTAD::MapEvent::hideDialog,          //0x04 ( 4)
  PTAD::MapEvent::bufferMessage,       //0x05 ( 5)
  PTAD::MapEvent::bufferValue,         //0x06 ( 6)
  PTAD::MapEvent::bufferCharacter,     //0x07 ( 7)
  PTAD::MapEvent::showMessage,         //0x08 ( 8)
  PTAD::MapEvent::showOneLiner,        //0x09 ( 9)
  PTAD::MapEvent::showShopMenu,        //0x0A (10)
  PTAD::MapEvent::showGold,            //0x0B (11)
  PTAD::MapEvent::hideGold,            //0x0C (12)
  PTAD::MapEvent::waitButtons,         //0x0D (13)
  PTAD::MapEvent::waitFrames,          //0x0E (14)
  PTAD::MapEvent::battle,              //0x0F (15)
  PTAD::MapEvent::shakeScreen,         //0x10 (16)
  PTAD::MapEvent::cutscene,            //0x11 (17)
  PTAD::MapEvent::jump,                //0x12 (18)
  PTAD::MapEvent::jumpIfSwitch,        //0x13 (19)
  PTAD::MapEvent::jumpIfVariable,      //0x14 (20)
  PTAD::MapEvent::jumpIfHasItem,       //0x15 (21)
  PTAD::MapEvent::jumpIfExited,        //0x16 (22)
  PTAD::MapEvent::jumpIfYesNo,         //0x17 (23)
  PTAD::MapEvent::jumpIfValue,         //0x18 (24)
  PTAD::MapEvent::playSoundEffect,     //0x19 (25)
  PTAD::MapEvent::playMusic,           //0x1A (26)
  PTAD::MapEvent::pauseMusic,          //0x1B (27)
  PTAD::MapEvent::resumeMusic,         //0x1C (28)
  PTAD::MapEvent::waitMusic,           //0x1D (29)
  PTAD::MapEvent::waitSoundEffect,     //0x1E (30)
  PTAD::MapEvent::turnSwitchOn,        //0x1F (31)
  PTAD::MapEvent::turnSwitchOff,       //0x20 (32)
  PTAD::MapEvent::toggleSwitch,        //0x21 (33)
  PTAD::MapEvent::turnSwitchRangeOn,   //0x22 (34)
  PTAD::MapEvent::turnSwitchRangeOff,  //0x23 (35)
  PTAD::MapEvent::toggleSwitchRange,   //0x24 (36)
  PTAD::MapEvent::changeVariable,      //0x25 (37)
  PTAD::MapEvent::changeSprite,        //0x26 (38)
  PTAD::MapEvent::changeEventLocation, //0x27 (39)
  PTAD::MapEvent::changeEventFlags,    //0x28 (40)
  PTAD::MapEvent::changePassability,   //0x29 (41)
  PTAD::MapEvent::givePlayerItem,      //0x2A (42)
  PTAD::MapEvent::givePlayerGold,      //0x2B (43)
  PTAD::MapEvent::heal,                //0x2C (44)
  PTAD::MapEvent::showImage,           //0x2D (45)
  PTAD::MapEvent::hideImage,           //0x2E (46)
  PTAD::MapEvent::movePlayer,          //0x2F (47)
  PTAD::MapEvent::endEventProcessing   //0x30 (48)
};

const uint32_t PTAD::MapEvent::wipeDownMasks[21] = {0xFFFFF, 0xFFFFE, 0xFFFFC, 0xFFFF8, 0xFFFF0, 0xFFFE0, 0xFFFC0, 0xFFF80, 0xFFF00, 0xFFE00, 0xFFC00, 0xFF800, 0xFF000, 0xFE000, 0xFC000, 0xF8000, 0xF0000, 0xE0000, 0xC0000, 0x80000, 0x0};
const uint32_t PTAD::MapEvent::wipeUpMasks[21] = {0xFFFFF, 0x7FFFF, 0x3FFFF, 0x1FFFF, 0xFFFF, 0x7FFF, 0x3FFF, 0x1FFF, 0xFFF, 0x7FF, 0x3FF, 0x1FF, 0xFF, 0x7F, 0x3F, 0x1F, 0xF, 0x7, 0x3, 0x1, 0x0};
const uint32_t PTAD::MapEvent::wipeInMasks[11] = {0xFFFFF, 0x7FFFE, 0x3FFFC, 0x1FFF8, 0xFFF0, 0x7FE0, 0x3FC0, 0x1F80, 0xF00, 0x600, 0x0};
const uint8_t PTAD::MapEvent::yesNoUI[2][8] = {0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x08,0x09,0x32,0x38,0x46,0x0A,0x27,0x42,0x0B};
const uint8_t PTAD::MapEvent::goldUI[2][7] = {0x60,0x20,0x42,0x3F,0x37,0x62,0x08,0x09,0x0A,0x0A,0x0A,0x0A,0x0A,0x0B};
DataPack::PackedFile PTAD::MapEvent::eventFile;
uint32_t PTAD::MapEvent::eventFileHash = 0;
uint32_t PTAD::MapEvent::currentBufferPos = 0;
int32_t PTAD::MapEvent::eventPos = 0;
int32_t PTAD::MapEvent::currentEvent = 0;
uint8_t PTAD::MapEvent::counters[4] = {0, 0, 0, 0};
#ifndef POK_SIM
uint8_t *PTAD::MapEvent::eventBuffer = (uint8_t*)PTAD::EVENT_BUFFER_MEMORY_ADDRESS;
#else
uint8_t PTAD::MapEvent::eventBuffer[PTAD::EVENT_BUFFER_MEMORY_SIZE];
#endif
bool PTAD::MapEvent::atEnd = true;

void PTAD::MapEvent::setup(uint32_t hash)
{
  if (hash == eventFileHash)
    return;
  PTAD::dataFile->getPackedFile(hash, &eventFile);
  eventFileHash = hash;
}

void PTAD::MapEvent::begin(uint32_t offset)
{
  eventFile.seek(0);
  currentBufferPos = 0;
  PTAD::dataFile->readBytes(&eventFile, eventBuffer, PTAD::EVENT_BUFFER_MEMORY_SIZE);
  eventPos = offset;
  atEnd = false;
  resetCounters();
}

bool PTAD::MapEvent::update()
{
  if (atEnd)
    return true;
  do
  {
    currentEvent = eventPos;
  } while (execEvent[nextByte()]());
  return atEnd;
}

void PTAD::MapEvent::runOnLoadEvent(uint32_t hash, uint32_t offset)
{
  DataPack::PackedFile old_eventFile = eventFile;
  uint32_t old_eventFileHash = eventFileHash;
  uint32_t old_currentBufferPos = currentBufferPos;
  int32_t old_eventPos = eventPos;
  int32_t old_currentEvent = currentEvent;
  uint8_t old_counters[4] = {counters[0], counters[1], counters[2], counters[3]};
  bool old_atEnd = atEnd;
  setup(hash);
  begin(offset);
  while (!atEnd)
    update();
  eventFile = old_eventFile;
  eventFileHash = old_eventFileHash;
  currentBufferPos = old_currentBufferPos;
  if (eventFileHash != 0)
  {
    eventFile.seek(currentBufferPos);
    PTAD::dataFile->readBytes(&eventFile, eventBuffer, PTAD::EVENT_BUFFER_MEMORY_SIZE);
  }
  eventPos = old_eventPos;
  currentEvent = old_currentEvent;
  counters[0] = old_counters[0];
  counters[1] = old_counters[1];
  counters[2] = old_counters[2];
  counters[3] = old_counters[3];
  atEnd = old_atEnd;
}

void PTAD::MapEvent::resetCounters()
{
  PTAD::globalCounter = 0;
  counters[0] = 0;
  counters[1] = 0;
  counters[2] = 0;
  counters[3] = 0;
}

uint8_t PTAD::MapEvent::nextByte()
{
  bool updateBuffer = false;
  while (eventPos < 0)
  {
    eventPos += PTAD::EVENT_BUFFER_MEMORY_SIZE;
    currentEvent += PTAD::EVENT_BUFFER_MEMORY_SIZE;
    currentBufferPos -= PTAD::EVENT_BUFFER_MEMORY_SIZE;
    updateBuffer = true;
  }
  while (eventPos >= PTAD::EVENT_BUFFER_MEMORY_SIZE)
  {
    eventPos -= PTAD::EVENT_BUFFER_MEMORY_SIZE;
    currentEvent -= PTAD::EVENT_BUFFER_MEMORY_SIZE;
    currentBufferPos += PTAD::EVENT_BUFFER_MEMORY_SIZE;
    updateBuffer = true;
  }
  if (updateBuffer)
  {
    eventFile.seek(currentBufferPos);
    PTAD::dataFile->readBytes(&eventFile, eventBuffer, PTAD::EVENT_BUFFER_MEMORY_SIZE);
  }
  return eventBuffer[eventPos++];
}

void PTAD::MapEvent::readValue(uint8_t *value, size_t size)
{
  for (size_t i = 0; i < size; ++i)
    *value++ = nextByte();
}

bool PTAD::MapEvent::hideScreen()
{
  uint8_t transitionID = nextByte();
  counters[2] = nextByte();
  if (transitionID == HIDE_INSTANT)
  {
    PD::setTASRowMask(0);
    Pokitto::lcdFillSurface(0);
    for (int i = 0; i < 256; ++i)
      PD::palette[i] = 0;
  }
  else if (transitionID == HIDE_FADEOUT)
  {
    counters[0] += counters[2];
    if (counters[0] < 64)
    {
      PD::loadRGBPalette(miloslav);
      for (int i = 0; i < 256; ++i)
      {
        uint8_t r = (PD::palette[i] >> 11) & 31;
        uint8_t g = (PD::palette[i] >> 5) & 63;
        uint8_t b = PD::palette[i] & 31;
        r = (r * (64 - counters[0])) / 64;
        g = (g * (64 - counters[0])) / 64;
        b = (b * (64 - counters[0])) / 64;
        PD::palette[i] = (r << 11) + (g << 5) + b;
      }
      eventPos = currentEvent;
      return false;
    }
    else
    {
      PD::setTASRowMask(0);
      Pokitto::lcdFillSurface(0);
      for (uint32_t i = 0; i < 256; ++i)
        PD::palette[i] = 0;
    }
  }
  else if (transitionID == HIDE_WIPE_DOWN)
  {
    if (counters[0] < 21)
    {
      ++counters[1];
      if (counters[1] == counters[2])
      {
        PD::setTASRowMask(wipeDownMasks[counters[0]]);
        Pokitto::lcdFillSurface(0);
        ++counters[0];
        counters[1] = 0;
      }
      eventPos = currentEvent;
      return false;
    }
    else
    {
      PD::setTASRowMask(0);
      Pokitto::lcdFillSurface(0);
      for (uint32_t i = 0; i < 256; ++i)
        PD::palette[i] = 0;
    }
  }
  else if (transitionID == HIDE_WIPE_UP)
  {
    if (counters[0] < 21)
    {
      ++counters[1];
      if (counters[1] == counters[2])
      {
        PD::setTASRowMask(wipeUpMasks[counters[0]]);
        Pokitto::lcdFillSurface(0);
        ++counters[0];
        counters[1] = 0;
      }
      eventPos = currentEvent;
      return false;
    }
    else
    {
      PD::setTASRowMask(0);
      Pokitto::lcdFillSurface(0);
      for (uint32_t i = 0; i < 256; ++i)
        PD::palette[i] = 0;
    }
  }
  else if (transitionID == HIDE_WIPE_IN)
  {
    if (counters[0] < 11)
    {
      ++counters[1];
      if (counters[1] == counters[2])
      {
        PD::setTASRowMask(wipeInMasks[counters[0]]);
        Pokitto::lcdFillSurface(0);
        ++counters[0];
        counters[1] = 0;
      }
      eventPos = currentEvent;
      return false;
    }
    else
    {
      PD::setTASRowMask(0);
      Pokitto::lcdFillSurface(0);
      for (uint32_t i = 0; i < 256; ++i)
        PD::palette[i] = 0;
    }
  }
  else if (transitionID == HIDE_MASK_OUT)
    PD::setTASRowMask(0);
  else if (transitionID == HIDE_WHITE_OUT)
  {
    counters[0] += counters[2];
    if (counters[0] < 64)
    {
      PD::loadRGBPalette(miloslav);
      for (int i = 0; i < 256; ++i)
      {
        uint8_t r = (PD::palette[i] >> 11) & 31;
        uint8_t g = (PD::palette[i] >> 5) & 63;
        uint8_t b = PD::palette[i] & 31;
        r = (r * (64 - counters[0]) + 31 * counters[0]) / 64;
        g = (g * (64 - counters[0]) + 63 * counters[0]) / 64;
        b = (b * (64 - counters[0]) + 31 * counters[0]) / 64;
        PD::palette[i] = (r << 11) + (g << 5) + b;
      }
      eventPos = currentEvent;
      return false;
    }
    else
    {
      PD::setTASRowMask(0);
      Pokitto::lcdFillSurface(0xFFFF);
      for (uint32_t i = 0; i < 256; ++i)
        PD::palette[i] = 0xFFFF;
    }
  }
  resetCounters();
  return true;
}

bool PTAD::MapEvent::showScreen()
{
  uint8_t transitionID = nextByte();
  counters[2] = nextByte();
  if (transitionID == SHOW_INSTANT)
  {
    PD::loadRGBPalette(miloslav);
    PD::setTASRowMask(0xFFFFFFFF);
  }
  else if (transitionID == SHOW_FADEIN)
  {
    PD::setTASRowMask(0xFFFFFFFF);
    counters[0] += counters[2];
    if (counters[0] < 64)
    {
      PD::loadRGBPalette(miloslav);
      for (int i = 0; i < 256; ++i)
      {
        uint8_t r = (PD::palette[i] >> 11) & 31;
        uint8_t g = (PD::palette[i] >> 5) & 63;
        uint8_t b = PD::palette[i] & 31;
        r = (r * counters[0]) / 64;
        g = (g * counters[0]) / 64;
        b = (b * counters[0]) / 64;
        PD::palette[i] = (r << 11) + (g << 5) + b;
      }
      eventPos = currentEvent;
      return false;
    }
    else
      PD::loadRGBPalette(miloslav);
  }
  else if (transitionID == SHOW_UNWIPE_DOWN)
  {
    PD::loadRGBPalette(miloslav);
    if (counters[0] < 21)
    {
      ++counters[1];
      if (counters[1] == counters[2])
      {
        PD::setTASRowMask(wipeDownMasks[20 - counters[0]]);
        ++counters[0];
        counters[1] = 0;
      }
      eventPos = currentEvent;
      return false;
    }
    else
      PD::setTASRowMask(0xFFFFFFFF);
  }
  else if (transitionID == SHOW_UNWIPE_UP)
  {
    PD::loadRGBPalette(miloslav);
    if (counters[0] < 21)
    {
      ++counters[1];
      if (counters[1] == counters[2])
      {
        PD::setTASRowMask(wipeUpMasks[20 - counters[0]]);
        ++counters[0];
        counters[1] = 0;
      }
      eventPos = currentEvent;
      return false;
    }
    else
      PD::setTASRowMask(0xFFFFFFFF);
  }
  else if (transitionID == SHOW_UNWIPE_OUT)
  {
    PD::loadRGBPalette(miloslav);
    if (counters[0] < 11)
    {
      ++counters[1];
      if (counters[1] == counters[2])
      {
        PD::setTASRowMask(wipeInMasks[10 - counters[0]]);
        ++counters[0];
        counters[1] = 0;
      }
      eventPos = currentEvent;
      return false;
    }
    else
      PD::setTASRowMask(0xFFFFFFFF);
  }
  else if (transitionID == SHOW_WHITE_IN)
  {
    PD::setTASRowMask(0xFFFFFFFF);
    counters[0] += counters[2];
    if (counters[0] < 64)
    {
      PD::loadRGBPalette(miloslav);
      for (int i = 0; i < 256; ++i)
      {
        uint8_t r = (PD::palette[i] >> 11) & 31;
        uint8_t g = (PD::palette[i] >> 5) & 63;
        uint8_t b = PD::palette[i] & 31;
        r = (r * counters[0] + 31 * (64 - counters[0])) / 64;
        g = (g * counters[0] + 63 * (64 - counters[0])) / 64;
        b = (b * counters[0] + 31 * (64 - counters[0])) / 64;
        PD::palette[i] = (r << 11) + (g << 5) + b;
      }
      eventPos = currentEvent;
      return false;
    }
    else
      PD::loadRGBPalette(miloslav);
  }
  resetCounters();
  return true;
}

bool PTAD::MapEvent::teleport()
{
  uint32_t mapHash;
  readValue((uint8_t*)&mapHash, sizeof(mapHash));
  PTAD::Game::player.x = (int32_t)nextByte() * 16;
  PTAD::Game::player.y = (int32_t)nextByte() * 16;
  uint8_t dir = nextByte();
  if (dir != 4)
    PTAD::Game::player.dir = dir;
  if (mapHash != PTAD::Game::player.mapHash)
    PTAD::Game::state = PTAD::Game::State::MapInit;
  else
    PTAD::Map::updateChunks();
  PTAD::Game::player.mapHash = mapHash;
  return true;
}

bool PTAD::MapEvent::showDialog()
{
  uint8_t name[8];
  PTAD::Dialog::showWindow();
  for (int i = 0; i < 8; ++i)
    name[i] = nextByte();
  if (name[0] == 255)
  {
    PTAD::Ui::drawText(PTAD::Game::player.name, 8, 2, 15);
    for (int i = 7; PTAD::Game::player.name[i] == PTAD::FONT_SPACE; --i)
      PTAD::Ui::drawCharacter(PTAD::FONT_FRAME_U, 2 + i, 15);
  }
  else
    PTAD::Ui::drawText(name, 8, 2, 15);
  PTAD::Ui::fixFrameHeader(1, 10, 15);
  return true;
}

bool PTAD::MapEvent::hideDialog()
{
  PTAD::Ui::clear();
  return true;
}

bool PTAD::MapEvent::bufferMessage()
{
  uint8_t length = nextByte();
  for (uint8_t i = 0; i < length; ++i)
    PTAD::Dialog::bufferCharacter(nextByte());
  return true;
}

bool PTAD::MapEvent::bufferValue()
{
  uint8_t value = nextByte();
  if (value == VALUE_PLAYER_NAME)
    PTAD::Dialog::bufferText(PTAD::Game::player.name, 8);
  else if (value == VALUE_PLAYER_HP)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.hp, 100);
  else if (value == VALUE_PLAYER_MAXHP)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.maxHP, 100);
  else if (value == VALUE_PLAYER_MP)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.mp, 10);
  else if (value == VALUE_PLAYER_MAXMP)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.maxMP, 10);
  else if (value == VALUE_PLAYER_EXPERIENCE)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.experience, 100000);
  else if (value == VALUE_PLAYER_NEXTLEVEL)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.nextLevel, 100000);
  else if (value == VALUE_PLAYER_EXPERIENCELEFT)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.nextLevel - PTAD::Game::player.experience, 1000);
  else if (value == VALUE_PLAYER_GOLD)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.gold, 10000);
  else if (value == VALUE_PLAYER_LEVEL)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.level, 10);
  else if (value == VALUE_PLAYER_ATTACK)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.stats.attack, 100);
  else if (value == VALUE_PLAYER_DEFENSE)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.stats.defense, 100);
  else if (value == VALUE_PLAYER_AGILITY)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.stats.agility, 100);
  else if (value == VALUE_PLAYER_MAGIC)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.stats.magic, 100);
  else if (value == VALUE_VARIABLE)
    PTAD::Dialog::bufferNumber(PTAD::Game::player.variables[nextByte()], 1000000000);
  else if (value == VALUE_SUPPLIED)
  {
    int32_t supplied;
    readValue((uint8_t*)&supplied, sizeof(supplied));
    PTAD::Dialog::bufferNumber(supplied, 1000000000);
  }
  PTAD::Dialog::trimBuffer();
  return true;
}

bool PTAD::MapEvent::bufferCharacter()
{
  PTAD::Dialog::bufferCharacter(nextByte());
  return true;
}

bool PTAD::MapEvent::showMessage()
{
  PTAD::Dialog::beginMessage();
  return false;
}

bool PTAD::MapEvent::showOneLiner()
{
  uint8_t length = nextByte();
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  for (uint8_t i = 0; i < length; ++i)
    PTAD::Ui::drawCharacter(nextByte(), i + 2, 19);
  return true;
}

bool PTAD::MapEvent::showShopMenu()
{
  for (int i = 0; i < 24; ++i)
    PTAD::Map::shopMessage[i] = nextByte();
  readValue((uint8_t*)&PTAD::Map::shopItems, sizeof(PTAD::Map::shopItems));
  for (int i = 0; i < 6; ++i)
    readValue((uint8_t*)&PTAD::Map::shopEquipment[i], sizeof(PTAD::Map::shopEquipment[0]));
  PTAD::Ui::clear();
  PTAD::Map::state = PTAD::Map::State::Menu;
  PTAD::Map::menuState = PTAD::Map::MenuState::ShopMenu;
  PTAD::Map::drawShopMenu();
  return false;
}

bool PTAD::MapEvent::showGold()
{
  PTAD::Ui::drawText(goldUI[0], 7, 20, 13);
  PTAD::Ui::drawText(goldUI[1], 7, 20, 14);
  PTAD::Ui::drawNumber(PTAD::Game::player.gold, 21, 14, 10000);
  return true;
}

bool PTAD::MapEvent::hideGold()
{
  PTAD::Ui::fillCharacter(255, 7, 20, 13);
  PTAD::Ui::fillCharacter(255, 7, 20, 14);
  return true;
}

bool PTAD::MapEvent::waitButtons()
{
  if (!PTAD::justPressed(PTAD::BTN_MASK_A) && !PB::bBtn())
    eventPos = currentEvent;
  return false;
}

bool PTAD::MapEvent::waitFrames()
{
  counters[2] = nextByte();
  ++counters[0];
  if (counters[0] < counters[2])
  {
    eventPos = currentEvent;
    return false;
  }
  resetCounters();
  return true;
}

bool PTAD::MapEvent::battle()
{
  PTAD::battleMonsterID = nextByte();
  PTAD::battleBG = nextByte();
  PTAD::battleBGM = nextByte();
  PTAD::Game::state = PTAD::Game::State::BattleInit;
  return false;
}

bool PTAD::MapEvent::shakeScreen()
{
  PTAD::Map::shakeScreen = nextByte();
  PTAD::Map::shakeMagnitude = nextByte();
  return true;
}

bool PTAD::MapEvent::cutscene() //TODO
{
  uint8_t id = nextByte();
  return false;
}

bool PTAD::MapEvent::jump()
{
  int32_t offset;
  readValue((uint8_t*)&offset, sizeof(offset));
  eventPos = offset - (int32_t)currentBufferPos;
  return eventPos > currentEvent;
}

bool PTAD::MapEvent::jumpIfSwitch()
{
  int32_t offPos, onPos;
  uint8_t switchID;
  readValue((uint8_t*)&offPos, sizeof(offPos));
  readValue((uint8_t*)&onPos, sizeof(onPos));
  switchID = nextByte();
  if ((PTAD::Game::player.switches[switchID / 32] >> (switchID % 32)) & 1)
    eventPos = onPos - (int32_t)currentBufferPos;
  else
    eventPos = offPos - (int32_t)currentBufferPos;
  return eventPos > currentEvent;
}

bool PTAD::MapEvent::jumpIfVariable()
{
  int32_t truePos, falsePos;
  int32_t value;
  uint8_t variableID, condition;
  bool test = false;
  readValue((uint8_t*)&truePos, sizeof(truePos));
  readValue((uint8_t*)&falsePos, sizeof(falsePos));
  readValue((uint8_t*)&value, sizeof(value));
  variableID = nextByte();
  condition = nextByte();
  if (condition == CONDITION_EQUAL_TO)
    test = PTAD::Game::player.variables[variableID] == value;
  else if (condition == CONDITION_NOT_EQUAL_TO)
    test = PTAD::Game::player.variables[variableID] != value;
  else if (condition == CONDITION_GREATER_THAN)
    test = PTAD::Game::player.variables[variableID] > value;
  else if (condition == CONDITION_GREATER_THAN_OR_EQUAL_TO)
    test = PTAD::Game::player.variables[variableID] >= value;
  else if (condition == CONDITION_LESS_THAN)
    test = PTAD::Game::player.variables[variableID] < value;
  else if (condition == CONDITION_LESS_THAN_OR_EQUAL_TO)
    test = PTAD::Game::player.variables[variableID] <= value;
  if (test)
    eventPos = truePos - (int32_t)currentBufferPos;
  else
    eventPos = falsePos - (int32_t)currentBufferPos;
  return eventPos > currentEvent;
}

bool PTAD::MapEvent::jumpIfHasItem()
{
  int32_t truePos, falsePos;
  uint8_t itemType, itemID;
  bool test = false;
  readValue((uint8_t*)&truePos, sizeof(truePos));
  readValue((uint8_t*)&falsePos, sizeof(falsePos));
  itemID = nextByte();
  itemType = (itemID >> 4) & 0xF;
  itemID &= 0xF;
  if (itemType < 2 && PTAD::Game::player.items[itemType] > 0)
    test = true;
  else if (itemType >= 2 && ((PTAD::Game::player.equippableItems[itemType - 2] >> itemID) & 1) != 0)
    test = true;
  if (test)
    eventPos = truePos - (int32_t)currentBufferPos;
  else
    eventPos = falsePos - (int32_t)currentBufferPos;
  return eventPos > currentEvent;
}

bool PTAD::MapEvent::jumpIfExited()
{
  int32_t truePos, falsePos;
  uint8_t direction = nextByte();
  bool test = false;
  readValue((uint8_t*)&truePos, sizeof(truePos));
  readValue((uint8_t*)&falsePos, sizeof(falsePos));
  if (direction == PTAD::DIR_UP)
    test = PTAD::Game::player.y < 0;
  else if (direction == PTAD::DIR_RIGHT)
    test = PTAD::Game::player.x >= PTAD::Map::data->width * 256;
  else if (direction == PTAD::DIR_DOWN)
    test = PTAD::Game::player.y >= PTAD::Map::data->height * 256;
  else if (direction == PTAD::DIR_LEFT)
    test = PTAD::Game::player.x < 0;
  if (test)
    eventPos = truePos - (int32_t)currentBufferPos;
  else
    eventPos = falsePos - (int32_t)currentBufferPos;
  return eventPos > currentEvent;
}

bool PTAD::MapEvent::jumpIfYesNo()
{
  int32_t yesPos, noPos;
  PTAD::Ui::drawText(yesNoUI[0], 8, 1, 13);
  PTAD::Ui::drawText(yesNoUI[1], 8, 1, 14);
  readValue((uint8_t*)&yesPos, sizeof(yesPos));
  readValue((uint8_t*)&noPos, sizeof(noPos));
  if (PTAD::justPressed(PTAD::BTN_MASK_LEFT) || PTAD::justPressed(PTAD::BTN_MASK_RIGHT))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    counters[0] ^= 32;
  }
  else if (PB::bBtn())
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    eventPos = noPos - (int32_t)currentBufferPos;
    PTAD::Ui::fillCharacter(255, 8, 1, 13);
    PTAD::Ui::fillCharacter(255, 8, 1, 14);
    resetCounters();
    return eventPos > currentEvent;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
    if (counters[0] == 0)
      eventPos = yesPos - (int32_t)currentBufferPos;
    else
      eventPos = noPos - (int32_t)currentBufferPos;
    PTAD::Ui::fillCharacter(255, 8, 1, 13);
    PTAD::Ui::fillCharacter(255, 8, 1, 14);
    resetCounters();
    return eventPos > currentEvent;
  }
  PTAD::Ui::cursorX = (int32_t)counters[0] - 2;
  PTAD::Ui::cursorY = 112;
  eventPos = currentEvent;
  return false;
}

bool PTAD::MapEvent::jumpIfValue()
{
  int32_t truePos, falsePos, current, desired;
  uint8_t value, condition;
  bool test;
  readValue((uint8_t*)&truePos, sizeof(truePos));
  readValue((uint8_t*)&falsePos, sizeof(falsePos));
  value = nextByte();
  condition = nextByte();
  readValue((uint8_t*)&desired, sizeof(desired));
  if (value == VALUE_PLAYER_HP)
    current = PTAD::Game::player.hp;
  else if (value == VALUE_PLAYER_MAXHP)
    current = PTAD::Game::player.maxHP;
  else if (value == VALUE_PLAYER_MP)
    current = PTAD::Game::player.mp;
  else if (value == VALUE_PLAYER_MAXMP)
    current = PTAD::Game::player.maxMP;
  else if (value == VALUE_PLAYER_EXPERIENCE)
    current = PTAD::Game::player.experience;
  else if (value == VALUE_PLAYER_NEXTLEVEL)
    current = PTAD::Game::player.nextLevel;
  else if (value == VALUE_PLAYER_EXPERIENCELEFT)
    current = PTAD::Game::player.nextLevel - PTAD::Game::player.experience;
  else if (value == VALUE_PLAYER_GOLD)
    current = PTAD::Game::player.gold;
  else if (value == VALUE_PLAYER_LEVEL)
    current = PTAD::Game::player.level;
  else if (value == VALUE_PLAYER_ATTACK)
    current = PTAD::Game::player.stats.attack;
  else if (value == VALUE_PLAYER_DEFENSE)
    current = PTAD::Game::player.stats.defense;
  else if (value == VALUE_PLAYER_AGILITY)
    current = PTAD::Game::player.stats.agility;
  else if (value == VALUE_PLAYER_MAGIC)
    current = PTAD::Game::player.stats.magic;
  if (condition == CONDITION_EQUAL_TO)
    test = current == desired;
  else if (condition == CONDITION_NOT_EQUAL_TO)
    test = current != desired;
  else if (condition == CONDITION_GREATER_THAN)
    test = current > desired;
  else if (condition == CONDITION_GREATER_THAN_OR_EQUAL_TO)
    test = current >= desired;
  else if (condition == CONDITION_LESS_THAN)
    test = current < desired;
  else if (condition == CONDITION_LESS_THAN_OR_EQUAL_TO)
    test = current <= desired;
  if (test)
    eventPos = truePos - (int32_t)currentBufferPos;
  else
    eventPos = falsePos - (int32_t)currentBufferPos;
  return eventPos > currentEvent;
}

bool PTAD::MapEvent::playSoundEffect()
{
  PTAD::Music::playSFX(nextByte());
  return true;
}

bool PTAD::MapEvent::playMusic()
{
  uint8_t bgmID = nextByte();
  bool loop = (bgmID & 128) != 0;
  bgmID &= 0x7F;
  if (loop)
    PTAD::Music::playMusic(bgmID, 0);
  else
    PTAD::Music::playMusic(bgmID, 1, false);
  return true;
}

bool PTAD::MapEvent::pauseMusic()
{
  PTAD::Music::pauseMusic();
  return true;
}

bool PTAD::MapEvent::resumeMusic()
{
  PTAD::Music::resumeMusic();
  return true;
}

bool PTAD::MapEvent::waitMusic()
{
  if (PTAD::Music::isMusicPlaying())
    eventPos = currentEvent;
  return false;
}

bool PTAD::MapEvent::waitSoundEffect()
{
  if (PTAD::Music::isSFXPlaying())
    eventPos = currentEvent;
  return false;
}

bool PTAD::MapEvent::turnSwitchOn()
{
  uint8_t switchID = nextByte();
  PTAD::Game::player.switches[switchID / 32] |= 1 << (switchID % 32);
  return true;
}

bool PTAD::MapEvent::turnSwitchOff()
{
  uint8_t switchID = nextByte();
  PTAD::Game::player.switches[switchID / 32] &= ~(1 << (switchID % 32));
  return true;
}

bool PTAD::MapEvent::toggleSwitch()
{
  uint8_t switchID = nextByte();
  PTAD::Game::player.switches[switchID / 32] ^= 1 << (switchID % 32);
  return true;
}

bool PTAD::MapEvent::turnSwitchRangeOn()
{
  uint8_t startID = nextByte();
  uint8_t endID = nextByte();
  for (uint8_t switchID = startID; switchID <= endID; ++switchID)
    PTAD::Game::player.switches[switchID / 32] |= 1 << (switchID % 32);
  return true;
}

bool PTAD::MapEvent::turnSwitchRangeOff()
{
  uint8_t startID = nextByte();
  uint8_t endID = nextByte();
  for (uint8_t switchID = startID; switchID <= endID; ++switchID)
    PTAD::Game::player.switches[switchID / 32] &= ~(1 << (switchID % 32));
  return true;
}

bool PTAD::MapEvent::toggleSwitchRange()
{
  uint8_t startID = nextByte();
  uint8_t endID = nextByte();
  for (uint8_t switchID = startID; switchID <= endID; ++switchID)
    PTAD::Game::player.switches[switchID / 32] ^= 1 << (switchID % 32);
  return true;
}

bool PTAD::MapEvent::changeVariable()
{
  int32_t value;
  uint8_t variableID, operation;
  readValue((uint8_t*)&value, sizeof(value));
  variableID = nextByte();
  operation = nextByte();
  if (operation == OPERATION_SET)
    PTAD::Game::player.variables[variableID] = value;
  else if (operation == OPERATION_ADD)
    PTAD::Game::player.variables[variableID] += value;
  else if (operation == OPERATION_SUB)
    PTAD::Game::player.variables[variableID] -= value;
  else if (operation == OPERATION_MUL)
    PTAD::Game::player.variables[variableID] *= value;
  else if (operation == OPERATION_DIV)
    PTAD::Game::player.variables[variableID] /= value;
  else if (operation == OPERATION_MOD)
    PTAD::Game::player.variables[variableID] %= value;
  return true;
}

bool PTAD::MapEvent::changeSprite()
{
  uint8_t eventID = nextByte();
  uint8_t spriteID = nextByte();
  PTAD::Map::data->events[eventID].spriteID = spriteID;
  return true;
}

bool PTAD::MapEvent::changeEventLocation()
{
  uint8_t eventID = nextByte();
  PTAD::Map::data->events[eventID & 0x7F].x = nextByte();
  PTAD::Map::data->events[eventID & 0x7F].y = nextByte();
  PTAD::Map::data->events[eventID & 0x7F].flags = (PTAD::Map::data->events[eventID & 0x7F].flags & 0xF7) | ((eventID & 128) >> 4);
  return true;
}

bool PTAD::MapEvent::changeEventFlags()
{
  uint8_t eventID = nextByte();
  uint8_t operation = (eventID >> 6) & 3;
  uint8_t flags = nextByte();
  eventID &= 0x3F;
  if (operation == FLAG_SET)
    PTAD::Map::data->events[eventID].flags |= flags;
  else if (operation == FLAG_CLEAR)
    PTAD::Map::data->events[eventID].flags &= ~(flags);
  else if (operation == FLAG_TOGGLE)
    PTAD::Map::data->events[eventID].flags ^= flags;
  return true;
}

bool PTAD::MapEvent::changePassability()
{
  uint8_t x = nextByte();
  uint8_t y = nextByte();
  uint8_t operation = nextByte();
  if (operation == PASSABILITY_SET)
    PTAD::Map::passability[(y / 8) * PTAD::Map::data->width * 16 + x] |= 1 << (y % 8);
  else if (operation == PASSABILITY_CLEAR)
    PTAD::Map::passability[(y / 8) * PTAD::Map::data->width * 16 + x] &= ~(1 << (y % 8));
  else if (operation == PASSABILITY_TOGGLE)
    PTAD::Map::passability[(y / 8) * PTAD::Map::data->width * 16 + x] ^= 1 << (y % 8);
  return true;
}

bool PTAD::MapEvent::givePlayerItem()
{
  uint8_t itemID = nextByte();
  uint8_t itemType = (itemID >> 4) & 0xF;
  itemID &= 0xF;
  if (itemType < 2)
  {
    uint8_t quantity = nextByte();
    if (99 - PTAD::Game::player.items[itemType][itemID] < quantity)
      PTAD::Game::player.items[itemType][itemID] = 99;
    else
      PTAD::Game::player.items[itemType][itemID] += quantity;
  }
  else
    PTAD::Game::player.equippableItems[itemType - 2] |= 1 << itemID;
  return true;
}

bool PTAD::MapEvent::givePlayerGold()
{
  int16_t amount;
  readValue((uint8_t*)&amount, sizeof(amount));
  if (amount > 0 && 0xFFFF - PTAD::Game::player.gold < amount)
    PTAD::Game::player.gold = 0xFFFF;
  else if (amount < 0 && PTAD::Game::player.gold < -amount)
    PTAD::Game::player.gold = 0;
  else
    PTAD::Game::player.gold += amount;
  return true;
}

bool PTAD::MapEvent::heal()
{
  PTAD::Game::player.hp = PTAD::Game::player.maxHP;
  PTAD::Game::player.mp = PTAD::Game::player.maxMP;
  return false;
}

bool PTAD::MapEvent::showImage()
{
  DataPack::PackedFile file;
  uint32_t hash;
  uint8_t scanline[220];
  readValue((uint8_t*)&hash, sizeof(hash));
  PTAD::dataFile->getPackedFile(hash, &file);
  PD::loadRGBPalette(miloslav);
  PD::setTASRowMask(0);
  Pokitto::lcdPrepareRefresh();
  for (int y = 0; y < 176; ++y)
  {
    PTAD::dataFile->readBytes(&file, (void*)scanline, 220);
    flushLine(PD::palette, scanline);
  }
  return true;
}

bool PTAD::MapEvent::hideImage()
{
  PD::setTASRowMask(0xFFFFFFFF);
  return true;
}

bool PTAD::MapEvent::movePlayer()
{
  uint8_t movement = nextByte();
  uint8_t direction = PTAD::Game::player.dir;
  bool retainFacing = (movement & 128) != 0;
  movement = movement & 0x7F;
  if (movement >= MOVE_FORWARD && movement <= MOVE_STRAFE_LEFT)
    movement = (direction + movement - MOVE_FORWARD) % 4;
  if (movement == MOVE_UP)
  {
    direction = PTAD::DIR_UP;
    PTAD::Map::state = PTAD::Map::State::MoveUp;
  }
  else if (movement == MOVE_RIGHT)
  {
    direction = PTAD::DIR_RIGHT;
    PTAD::Map::state = PTAD::Map::State::MoveRight;
  }
  else if (movement == MOVE_DOWN)
  {
    direction = PTAD::DIR_DOWN;
    PTAD::Map::state = PTAD::Map::State::MoveDown;
  }
  else if (movement == MOVE_LEFT)
  {
    direction = PTAD::DIR_LEFT;
    PTAD::Map::state = PTAD::Map::State::MoveLeft;
  }
  else if (movement >= MOVE_FACE_UP && movement <= MOVE_FACE_LEFT)
    direction = movement - MOVE_FACE_UP;
  else if (movement == MOVE_TURN_LEFT)
    direction = (direction + 3) % 4;
  else if (movement == MOVE_TURN_RIGHT)
    direction = (direction + 1) % 4;
  else if (movement == MOVE_TURN_AROUND)
    direction = (direction + 2) % 4;
  if (!retainFacing)
    PTAD::Game::player.dir = direction;
  return false;
}

bool PTAD::MapEvent::endEventProcessing()
{
  atEnd = true;
  return false;
}
