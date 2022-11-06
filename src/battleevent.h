#pragma once

#include "headers.h"

namespace PTAD
{
  class BattleEvent
  {
    public:
      static void setup(DataPack::PackedFile &file);
      static void begin(uint32_t offset);
      static bool update();
      static bool inBattleEvent() {return !atEnd;}
    private:
      static void resetCounters();
      static uint8_t nextByte();
      static void readValue(uint8_t *value, size_t size);
      static bool event_shakeScreen();
      static bool event_flashBattler();
      static bool event_flashUi();
      static bool event_basicAttack();
      static bool event_useSkill();
      static bool event_castSpell();
      static bool event_playSoundEffect();
      static bool event_bufferMessage();
      static bool event_bufferValue();
      static bool event_BufferCharacter();
      static bool event_showMessage();
      static bool event_jump();
      static bool event_jumpIf();
      static bool event_jumpIfStatus();
      static bool event_jumpIfStat();
      static bool event_changeBattlerSprite();
      static bool event_changeBackgroundImage();
      static bool event_playBattleAnimation();
      static bool event_waitFrames();
      static bool event_waitButtons();
      static bool event_inflictStatus();
      static bool event_consumeMP();
      static bool event_random();
      static bool event_endEventProcessing();
      static bool (*const execEvent[]) ();
      static constexpr uint8_t VALUE_PLAYER_NAME = 0;
      static constexpr uint8_t VALUE_PLAYER_HP = 1;
      static constexpr uint8_t VALUE_PLAYER_MAXHP = 2;
      static constexpr uint8_t VALUE_PLAYER_MP = 3;
      static constexpr uint8_t VALUE_PLAYER_MAXMP = 4;
      static constexpr uint8_t VALUE_PLAYER_EXPERIENCE = 5;
      static constexpr uint8_t VALUE_PLAYER_NEXTLEVEL = 6;
      static constexpr uint8_t VALUE_PLAYER_EXPERIENCELEFT = 7;
      static constexpr uint8_t VALUE_PLAYER_GOLD = 8;
      static constexpr uint8_t VALUE_PLAYER_LEVEL = 9;
      static constexpr uint8_t VALUE_PLAYER_ATTACK = 10;
      static constexpr uint8_t VALUE_PLAYER_DEFENSE = 11;
      static constexpr uint8_t VALUE_PLAYER_AGILITY = 12;
      static constexpr uint8_t VALUE_PLAYER_MAGIC = 13;
      static constexpr uint8_t VALUE_MONSTER_NAME = 14;
      static constexpr uint8_t VALUE_MONSTER_HP = 15;
      static constexpr uint8_t VALUE_MONSTER_MAXHP = 16;
      static constexpr uint8_t VALUE_MONSTER_MP = 17;
      static constexpr uint8_t VALUE_MONSTER_EXPERIENCE = 18;
      static constexpr uint8_t VALUE_MONSTER_GOLD = 19;
      static constexpr uint8_t VALUE_MONSTER_ATTACK = 20;
      static constexpr uint8_t VALUE_MONSTER_DEFENSE = 21;
      static constexpr uint8_t VALUE_MONSTER_AGILITY = 22;
      static constexpr uint8_t VALUE_MONSTER_MAGIC = 23;
      static constexpr uint8_t VALUE_VARIABLE = 24;
      static constexpr uint8_t VALUE_SUPPLIED = 25;
      static constexpr uint8_t CONDITION_EQUAL_TO = 0;
      static constexpr uint8_t CONDITION_NOT_EQUAL_TO = 1;
      static constexpr uint8_t CONDITION_GREATER_THAN = 2;
      static constexpr uint8_t CONDITION_GREATER_THAN_OR_EQUAL_TO = 3;
      static constexpr uint8_t CONDITION_LESS_THAN = 4;
      static constexpr uint8_t CONDITION_LESS_THAN_OR_EQUAL_TO = 5;
      static DataPack::PackedFile eventFile;
      static int damageDealt;
      static uint32_t currentBufferPos;
      static int32_t eventPos;
      static int32_t currentEvent;
      static uint8_t counters[4];
      #ifndef POK_SIM
      static uint8_t *eventBuffer;
      #else
      static uint8_t eventBuffer[];
      #endif
      static bool atEnd;
  };
};
