#include "headers.h"

//#define DEBUG_BATTLEEVENTS

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

bool (*const PTAD::BattleEvent::execEvent[]) () =
{
  PTAD::BattleEvent::event_shakeScreen,           //0x00 ( 0)
  PTAD::BattleEvent::event_flashBattler,          //0x01 ( 1)
  PTAD::BattleEvent::event_flashUi,               //0x02 ( 2)
  PTAD::BattleEvent::event_basicAttack,           //0x03 ( 3)
  PTAD::BattleEvent::event_useSkill,              //0x04 ( 4)
  PTAD::BattleEvent::event_castSpell,             //0x05 ( 5)
  PTAD::BattleEvent::event_playSoundEffect,       //0x06 ( 6)
  PTAD::BattleEvent::event_bufferMessage,         //0x07 ( 7)
  PTAD::BattleEvent::event_bufferValue,           //0x08 ( 8)
  PTAD::BattleEvent::event_BufferCharacter,       //0x09 ( 9)
  PTAD::BattleEvent::event_showMessage,           //0x0A (10)
  PTAD::BattleEvent::event_jump,                  //0x0B (11)
  PTAD::BattleEvent::event_jumpIf,                //0x0C (12)
  PTAD::BattleEvent::event_jumpIfStatus,          //0x0D (13)
  PTAD::BattleEvent::event_jumpIfStat,            //0x0E (14)
  PTAD::BattleEvent::event_changeBattlerSprite,   //0x0F (15)
  PTAD::BattleEvent::event_changeBackgroundImage, //0x10 (16)
  PTAD::BattleEvent::event_playBattleAnimation,   //0x11 (17)
  PTAD::BattleEvent::event_waitFrames,            //0x12 (18)
  PTAD::BattleEvent::event_waitButtons,           //0x13 (19)
  PTAD::BattleEvent::event_inflictStatus,         //0x14 (20)
  PTAD::BattleEvent::event_consumeMP,             //0x15 (21)
  PTAD::BattleEvent::event_random,                //0x16 (22)
  PTAD::BattleEvent::event_endEventProcessing     //0x17 (23)
};

#ifdef DEBUG_BATTLEEVENTS
const char *battleEventNames[] =
{
  "shakeScreen",
  "flashBattler",
  "flashUi",
  "basicAttack",
  "useSkill",
  "castSpell",
  "playSoundEffect",
  "bufferMessage",
  "bufferValue",
  "BufferCharacter",
  "showMessage",
  "jump",
  "jumpIf",
  "jumpIfStatus",
  "jumpIfStat",
  "changeBattlerSprite",
  "changeBackgroundImage",
  "playBattleAnimation",
  "waitFrames",
  "waitButtons",
  "inflictStatus",
  "consumeMP",
  "random",
  "endEventProcessing"
};
#endif

DataPack::PackedFile PTAD::BattleEvent::eventFile;
int PTAD::BattleEvent::damageDealt = 0;
uint32_t PTAD::BattleEvent::currentBufferPos = 0;
int32_t PTAD::BattleEvent::eventPos = 0;
int32_t PTAD::BattleEvent::currentEvent = 0;
uint8_t PTAD::BattleEvent::counters[4] = {0, 0, 0, 0};
#ifndef POK_SIM
uint8_t *PTAD::BattleEvent::eventBuffer = (uint8_t*)PTAD::EVENT_BUFFER_MEMORY_ADDRESS;
#else
uint8_t PTAD::BattleEvent::eventBuffer[PTAD::EVENT_BUFFER_MEMORY_SIZE];
#endif
bool PTAD::BattleEvent::atEnd = true;

void PTAD::BattleEvent::setup(DataPack::PackedFile &file)
{
  eventFile = file;
}

void PTAD::BattleEvent::begin(uint32_t offset)
{
#ifdef DEBUG_BATTLEEVENTS
  printf("PTAD::BattleEvent::begin(%d)\n", offset);
  fflush(stdout);
#endif
  eventFile.seek(sizeof(PTAD::Battle::EnemyData));
  currentBufferPos = sizeof(PTAD::Battle::EnemyData);
  PTAD::dataFile->readBytes(&eventFile, eventBuffer, PTAD::EVENT_BUFFER_MEMORY_SIZE);
  eventPos = offset;
  atEnd = false;
  resetCounters();
}

bool PTAD::BattleEvent::update()
{
  uint8_t eventID;
  if (atEnd)
    return true;
  do
  {
    currentEvent = eventPos;
    eventID = nextByte();
#ifdef DEBUG_BATTLEEVENTS
    printf("%d: %s\n", currentEvent + currentBufferPos, battleEventNames[eventID]);
    fflush(stdout);
#endif
  } while (execEvent[eventID]());
  return atEnd;
}

void PTAD::BattleEvent::resetCounters()
{
  PTAD::globalCounter = 0;
  counters[0] = 0;
  counters[1] = 0;
  counters[2] = 0;
  counters[3] = 0;
}

uint8_t PTAD::BattleEvent::nextByte()
{
  bool updateBuffer = false;
  while (eventPos < 0)
  {
    eventPos += PTAD::EVENT_BUFFER_MEMORY_SIZE;
    currentEvent += PTAD::EVENT_BUFFER_MEMORY_SIZE;
    currentBufferPos -= PTAD::EVENT_BUFFER_MEMORY_SIZE;
    updateBuffer = true;
  }
  while (eventPos >= (int32_t)PTAD::EVENT_BUFFER_MEMORY_SIZE)
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

void PTAD::BattleEvent::readValue(uint8_t *value, size_t size)
{
  for (size_t i = 0; i < size; ++i)
    *value++ = nextByte();
}

bool PTAD::BattleEvent::event_shakeScreen()
{
  PTAD::Battle::shakeScreen = nextByte();
  PTAD::Battle::shakeRate = nextByte();
  return true;
}

bool PTAD::BattleEvent::event_flashBattler()
{
  uint8_t duration = nextByte();
  uint8_t color1 = nextByte();
  uint8_t color2 = nextByte();
  uint8_t color3 = nextByte();
  PTAD::Battle::flashBattlerSprite(duration, color1, color2, color3);
  return true;
}

bool PTAD::BattleEvent::event_flashUi()
{
  PTAD::Battle::flashUi = nextByte();
  PTAD::Ui::fgColor = nextByte();
  return true;
}

bool PTAD::BattleEvent::event_basicAttack()
{
  uint32_t animation;
  readValue((uint8_t*)&animation, sizeof(animation));
  damageDealt = PTAD::Battle::attackDamageDealt(PTAD::Battle::getEnemyAttack(), PTAD::Battle::getEnemyAgility(), PTAD::Battle::getPlayerDefense(), PTAD::Battle::getPlayerAgility(), (PTAD::Battle::playerSpellResistance >> 12) & 3);
  if (damageDealt > 0)
  {
    PTAD::BattleAnimation::beginAnimation(animation);
    if (random(0, 100) < 5)
    {
      counters[3] = 2;
      damageDealt *= 2;
      PTAD::Battle::playerStatus &= ~(3 << PTAD::Battle::STATUS_FOCUSED);
      PTAD::Dialog::addMessage(PTAD::Resources::battleCriticalHit);
      PTAD::Dialog::bufferNumber(damageDealt, 100);
      PTAD::Dialog::addMessage(PTAD::Resources::battleDamageTakenEnd);
    }
    else
    {
      counters[3] = 1;
      PTAD::Dialog::addMessage(PTAD::Resources::battleDamageTakenBegin);
      PTAD::Dialog::bufferNumber(damageDealt, 100);
      PTAD::Dialog::addMessage(PTAD::Resources::battleDamageTakenEnd);
    }
  }
  else
  {
    counters[3] = 0;
    PTAD::Music::playSFX(PTAD::Resources::sfx_miss);
    PTAD::Dialog::addMessage(PTAD::Resources::battleMiss);
  }
  PTAD::Dialog::beginMessage();
  if (damageDealt >= PTAD::Game::player.hp)
  {
    atEnd = true;
    PTAD::Game::player.hp = 0;
  }
  else
    PTAD::Game::player.hp -= damageDealt;
  return false;
}

bool PTAD::BattleEvent::event_useSkill() //TODO
{
  return true;
}

bool PTAD::BattleEvent::event_castSpell()
{
  //TODO: implement animation
  uint32_t animation;
  uint8_t type, level, mp;
  readValue((uint8_t*)&animation, sizeof(animation));
  if (!PTAD::BattleAnimation::isAnimationPlaying())
  {
    if (counters[3] != 255) //Wait until animation is finished playing
    {
      counters[3] = 255;
      PTAD::BattleAnimation::beginAnimation(animation);
      eventPos = currentEvent;
      return false;
    }
    type = nextByte() * 2;
    level = nextByte();
    mp = nextByte();
    damageDealt = PTAD::Battle::magicDamageDealt(PTAD::Battle::getEnemyAttack(), PTAD::Battle::getEnemyMagic(), level, PTAD::Battle::getPlayerDefense(), PTAD::Battle::getPlayerMagic(), (PTAD::Battle::playerSpellResistance >> type) & 3);
    if (damageDealt > 0)
    {
      counters[3] = 1;
      PTAD::Dialog::addMessage(PTAD::Resources::battleDamageTakenBegin);
      PTAD::Dialog::bufferNumber(damageDealt, 100);
      PTAD::Dialog::addMessage(PTAD::Resources::battleDamageTakenEnd);
    }
    else
    {
      counters[3] = 0;
      PTAD::Music::playSFX(PTAD::Resources::sfx_miss);
      PTAD::Dialog::addMessage(PTAD::Resources::battleMiss);
    }
    PTAD::Dialog::beginMessage();
    if (damageDealt >= PTAD::Game::player.hp)
    {
      atEnd = true;
      PTAD::Game::player.hp = 0;
    }
    else
      PTAD::Game::player.hp -= damageDealt;
    PTAD::Battle::enemy->mp -= mp;
    return true;
  }
  eventPos = currentEvent;
  return false;
}

bool PTAD::BattleEvent::event_playSoundEffect()
{
  PTAD::Music::playSFX(nextByte());
  return true;
}

bool PTAD::BattleEvent::event_bufferMessage()
{
  uint8_t length = nextByte();
  for (uint8_t i = 0; i < length; ++i)
    PTAD::Dialog::bufferCharacter(nextByte());
  return true;
}

bool PTAD::BattleEvent::event_bufferValue() //TODO
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
  else if (value == VALUE_MONSTER_NAME)
    PTAD::Dialog::bufferText(PTAD::Resources::enemyNames + PTAD::battleMonsterID * 8, 8);
  else if (value == VALUE_MONSTER_HP)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->hp, 10000);
  else if (value == VALUE_MONSTER_MAXHP)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->maxHP, 10000);
  else if (value == VALUE_MONSTER_MP)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->mp, 100);
  else if (value == VALUE_MONSTER_EXPERIENCE)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->experience, 10000);
  else if (value == VALUE_MONSTER_GOLD)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->gold, 100);
  else if (value == VALUE_MONSTER_ATTACK)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->stats.attack, 10000);
  else if (value == VALUE_MONSTER_DEFENSE)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->stats.defense, 10000);
  else if (value == VALUE_MONSTER_AGILITY)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->stats.agility, 10000);
  else if (value == VALUE_MONSTER_MAGIC)
    PTAD::Dialog::bufferNumber(PTAD::Battle::enemy->stats.magic, 10000);
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

bool PTAD::BattleEvent::event_BufferCharacter()
{
  PTAD::Dialog::bufferCharacter(nextByte());
  return true;
}

bool PTAD::BattleEvent::event_showMessage()
{
  PTAD::Dialog::beginMessage();
  return false;
}

bool PTAD::BattleEvent::event_jump()
{
  int32_t offset;
  readValue((uint8_t*)&offset, sizeof(offset));
  eventPos = offset - (int32_t)currentBufferPos + sizeof(PTAD::Battle::EnemyData);
  return eventPos > currentEvent;
}

bool PTAD::BattleEvent::event_jumpIf()
{
  int32_t truePos, falsePos;
  uint8_t counter, value, condition;
  bool test = false;
  readValue((uint8_t*)&truePos, sizeof(truePos));
  readValue((uint8_t*)&falsePos, sizeof(falsePos));
  counter = nextByte();
  value = nextByte();
  condition = nextByte();
  if (condition == CONDITION_EQUAL_TO)
    test = counters[counter] == value;
  else if (condition == CONDITION_NOT_EQUAL_TO)
    test = counters[counter] != value;
  else if (condition == CONDITION_GREATER_THAN)
    test = counters[counter] > value;
  else if (condition == CONDITION_GREATER_THAN_OR_EQUAL_TO)
    test = counters[counter] >= value;
  else if (condition == CONDITION_LESS_THAN)
    test = counters[counter] < value;
  else if (condition == CONDITION_LESS_THAN_OR_EQUAL_TO)
    test = counters[counter] <= value;
  if (test)
    eventPos = truePos - (int32_t)currentBufferPos + sizeof(PTAD::Battle::EnemyData);
  else
    eventPos = falsePos - (int32_t)currentBufferPos + sizeof(PTAD::Battle::EnemyData);
  return eventPos > currentEvent;
}

bool PTAD::BattleEvent::event_jumpIfStatus()
{
  int32_t truePos, falsePos;
  uint8_t value = nextByte();
  uint8_t condition = nextByte();
  uint8_t status = (value >> 2) & 7;
  uint8_t level = value & 3;
  uint8_t current;
  bool self = (value & 128) != 0;
  bool test = false;
  readValue((uint8_t*)&truePos, sizeof(truePos));
  readValue((uint8_t*)&falsePos, sizeof(falsePos));
  if (self)
    current = (PTAD::Battle::enemyStatus >> status) & 3;
  else
    current = (PTAD::Battle::playerStatus >> status) & 3;
  if (condition == CONDITION_EQUAL_TO)
    test = current == level;
  else if (condition == CONDITION_NOT_EQUAL_TO)
    test = current != level;
  else if (condition == CONDITION_GREATER_THAN)
    test = current > level;
  else if (condition == CONDITION_GREATER_THAN_OR_EQUAL_TO)
    test = current >= level;
  else if (condition == CONDITION_LESS_THAN)
    test = current < level;
  else if (condition == CONDITION_LESS_THAN_OR_EQUAL_TO)
    test = current <= level;
  if (test)
    eventPos = truePos - (int32_t)currentBufferPos + sizeof(PTAD::Battle::EnemyData);
  else
    eventPos = falsePos - (int32_t)currentBufferPos + sizeof(PTAD::Battle::EnemyData);
  return eventPos > currentEvent;
}

bool PTAD::BattleEvent::event_jumpIfStat()
{
  int32_t truePos, falsePos;
  uint8_t condition = nextByte();
  uint16_t value;
  uint16_t current;
  bool test = false;
  bool hp = (condition & 128) != 0;
  condition &= 0x7F;
  readValue((uint8_t*)&value, sizeof(value));
  readValue((uint8_t*)&truePos, sizeof(truePos));
  readValue((uint8_t*)&falsePos, sizeof(falsePos));
  if (hp)
    current = PTAD::Battle::enemy->hp;
  else
    current = PTAD::Battle::enemy->mp;
  if (condition == CONDITION_EQUAL_TO)
    test = current == value;
  else if (condition == CONDITION_NOT_EQUAL_TO)
    test = current != value;
  else if (condition == CONDITION_GREATER_THAN)
    test = current > value;
  else if (condition == CONDITION_GREATER_THAN_OR_EQUAL_TO)
    test = current >= value;
  else if (condition == CONDITION_LESS_THAN)
    test = current < value;
  else if (condition == CONDITION_LESS_THAN_OR_EQUAL_TO)
    test = current <= value;
  if (test)
    eventPos = truePos - (int32_t)currentBufferPos + sizeof(PTAD::Battle::EnemyData);
  else
    eventPos = falsePos - (int32_t)currentBufferPos + sizeof(PTAD::Battle::EnemyData);
  return eventPos > currentEvent;
}

bool PTAD::BattleEvent::event_changeBattlerSprite()
{
  PTAD::battleMonsterID = nextByte();
  PTAD::Battle::loadBattlerSprite();
  return true;
}

bool PTAD::BattleEvent::event_changeBackgroundImage()
{
  PTAD::battleBG = nextByte();
  PTAD::Battle::loadBackgroundImage();
  return true;
}

bool PTAD::BattleEvent::event_playBattleAnimation()
{
  uint32_t animation;
  readValue((uint8_t*)&animation, sizeof(animation));
  PTAD::BattleAnimation::beginAnimation(animation);
  return true;
}

bool PTAD::BattleEvent::event_waitFrames()
{
  if (counters[0] == 0)
  {
    counters[0] = 1;
    counters[1] = nextByte() - 1;
    eventPos = currentEvent;
    return false;
  }
  else if (counters[1] > 0)
  {
    --counters[1];
    eventPos = currentEvent;
    return false;
  }
  resetCounters();
  return true;
}

bool PTAD::BattleEvent::event_waitButtons()
{
  if (PTAD::justPressed(PTAD::BTN_MASK_A) || PB::bBtn())
    return true;
  else
    eventPos = currentEvent;
  return false;
}

bool PTAD::BattleEvent::event_inflictStatus()
{
  uint8_t value = nextByte();
  uint8_t successMessageLength = nextByte();
  uint8_t failMessageLength = nextByte();
  uint8_t status = (value >> 2) & 7;
  uint8_t level = value & 3;
  bool self = (value & 128) != 0;
  if (self)
  {
    if (status == PTAD::Battle::STATUS_POISON)
      PTAD::Battle::enemyStatus &= ~(3 << status);
    else if (status == PTAD::Battle::STATUS_SPEED)
    {
      uint8_t current = (PTAD::Battle::enemyStatus >> PTAD::Battle::STATUS_SPEED) & 3;
      PTAD::Battle::enemyStatus &= (3 << PTAD::Battle::STATUS_SPEED);
      if (current != PTAD::Battle::SPEED_SLOW)
        PTAD::Battle::enemyStatus |= (PTAD::Battle::SPEED_HASTE << PTAD::Battle::STATUS_SPEED);
    }
    else if (status == PTAD::Battle::STATUS_FOCUSED)
    {
      if (((PTAD::Battle::enemyStatus >> PTAD::Battle::STATUS_FOCUSED) & 3) < level)
        PTAD::Battle::enemyStatus += (1 << PTAD::Battle::STATUS_FOCUSED);
      PTAD::Battle::keepFocus = true;
    }
    else if (status == PTAD::Battle::STATUS_BERSERK)
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_berserk);
      if (((PTAD::Battle::enemyStatus >> PTAD::Battle::STATUS_BERSERK) & 3) < level)
        PTAD::Battle::enemyStatus += (1 << PTAD::Battle::STATUS_BERSERK);
    }
    for (uint8_t i = 0; i < successMessageLength; ++i)
      PTAD::Dialog::bufferCharacter(nextByte());
    eventPos += failMessageLength;
  }
  else
  {
    uint16_t chance = nextByte();
    printf("inflictStats: player (%d) status=%d level=%d\n", chance, status, level);
    fflush(stdout);
    if (random(0, 0xFFFF) <= chance * (255 - PTAD::Battle::playerStatusResistance[status / 2]))
    {
      if (status == PTAD::Battle::STATUS_POISON)
      {
        uint8_t current = (PTAD::Battle::playerStatus >> PTAD::Battle::STATUS_POISON) & 3;
        PTAD::Battle::playerStatus &= ~(3 << PTAD::Battle::STATUS_POISON);
        if (current > level)
          level = current;
        PTAD::Battle::playerStatus |= (level << PTAD::Battle::STATUS_POISON);
        PTAD::Music::playSFX(PTAD::Resources::sfx_poison);
        PTAD::Ui::fgColor = 203;
        PTAD::Battle::shakeScreen = 1;
        PTAD::Battle::shakeRate = 8;
        PTAD::Battle::flashUi = 16;
      }
      else if (status == PTAD::Battle::STATUS_SPEED)
      {
        uint8_t current = (PTAD::Battle::playerStatus >> PTAD::Battle::STATUS_SPEED) & 3;
        printf("Inflict SLOW: %d (%d) -> ", PTAD::Battle::playerStatus, current);
        PTAD::Battle::playerStatus &= ~(3 << PTAD::Battle::STATUS_SPEED);
        if (current != PTAD::Battle::SPEED_HASTE)
          PTAD::Battle::playerStatus |= (PTAD::Battle::SPEED_SLOW << PTAD::Battle::STATUS_SPEED);
        printf("%d\n", PTAD::Battle::playerStatus);
        fflush(stdout);
        PTAD::Music::playSFX(PTAD::Resources::sfx_slow);
        PTAD::Ui::fgColor = 3;
        PTAD::Battle::shakeScreen = 1;
        PTAD::Battle::shakeRate = 8;
        PTAD::Battle::flashUi = 16;
      }
      else if (status == PTAD::Battle::STATUS_FOCUSED)
        PTAD::Battle::playerStatus &= ~(3 << PTAD::Battle::STATUS_FOCUSED);
      else if (status == PTAD::Battle::STATUS_BERSERK)
      {
        if (((PTAD::Battle::playerStatus >> PTAD::Battle::STATUS_BERSERK) & 3) < level)
          PTAD::Battle::playerStatus += (1 << PTAD::Battle::STATUS_BERSERK);
        PTAD::Music::playSFX(PTAD::Resources::sfx_berserk);
        PTAD::Ui::fgColor = 175;
        PTAD::Battle::shakeScreen = 1;
        PTAD::Battle::shakeRate = 8;
        PTAD::Battle::flashUi = 16;
      }
      for (uint8_t i = 0; i < successMessageLength; ++i)
        PTAD::Dialog::bufferCharacter(nextByte());
      eventPos += failMessageLength;
    }
    else
    {
      eventPos += successMessageLength;
      for (uint8_t i = 0; i < failMessageLength; ++i)
        PTAD::Dialog::bufferCharacter(nextByte());
    }
  }
  if (successMessageLength != 0 || failMessageLength != 0)
    PTAD::Dialog::beginMessage();
  return false;
}

bool PTAD::BattleEvent::event_consumeMP()
{
  uint8_t amount = nextByte();
  if (PTAD::Battle::enemy->mp <= amount)
    PTAD::Battle::enemy->mp = 0;
  else
    PTAD::Battle::enemy->mp -= amount;
  return true;
}

bool PTAD::BattleEvent::event_random()
{
  uint8_t counter = nextByte();
  uint8_t max = nextByte();
  counters[counter] = random(0, max);
  return true;
}

bool PTAD::BattleEvent::event_endEventProcessing()
{
  atEnd = true;
  return false;
}
