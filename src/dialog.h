#pragma once

#include "headers.h"

namespace PTAD
{
  class Dialog
  {
    public:
      static void setup();
      static void showWindow();
      static void shiftTextUp();
      static void bufferText(const uint8_t *array, uint8_t numChars);
      static void bufferNumber(int32_t value, int32_t start);
      static void bufferCharacter(uint8_t character);
      static void trimBuffer();
      static void addMessage(uint8_t id);
      static void beginMessage(bool shiftUp=true);
      static bool updateMessage(bool skippable=true);
      static void stopMessage();
      static void setMessageSpeed(uint8_t speed);
      static uint8_t getMessageSpeed();
      static constexpr uint8_t MESSAGES_SIZE = 24;
      /*static constexpr uint8_t MESSAGES_NULL = 0;
      static constexpr uint8_t MESSAGES_BATTLE_CHOOSE_ACTION = 1;
      static constexpr uint8_t MESSAGES_BATTLE_SURPRISE_ATTACK = 2;
      static constexpr uint8_t MESSAGES_BATTLE_CRITICAL_HIT = 3;
      static constexpr uint8_t MESSAGES_BATTLE_GOLD_EARNED = 4;
      static constexpr uint8_t MESSAGES_BATTLE_EXPERIENCE_GAINED = 5;
      static constexpr uint8_t MESSAGES_BATTLE_LEVEL_UP = 6;
      static constexpr uint8_t MESSAGES_BATTLE_SKILL_LEARNED = 7;
      static constexpr uint8_t MESSAGES_BATTLE_DAMAGE_TAKEN_BEGIN = 8;
      static constexpr uint8_t MESSAGES_BATTLE_DAMAGE_TAKEN_END = 9;
      static constexpr uint8_t MESSAGES_BATTLE_DEFEAT = 10;
      static constexpr uint8_t MESSAGES_BATTLE_FIGHT = 11;
      static constexpr uint8_t MESSAGES_BATTLE_RUN_SUCCESS = 12;
      static constexpr uint8_t MESSAGES_BATTLE_RUN_FAIL = 13;
      static constexpr uint8_t MESSAGES_BATTLE_RUN_IMPOSSIBLE = 14;
      static constexpr uint8_t MESSAGES_BATTLE_DAMAGE_DEALT = 15;
      static constexpr uint8_t MESSAGES_BATTLE_MISS = 16;
      static constexpr uint8_t MESSAGES_BATTLE_POISON_BEGIN = 17;
      static constexpr uint8_t MESSAGES_MENU_DIALOG_TEST = 18;
      static constexpr uint8_t MESSAGES_BATTLE_BERSERK_ATTACK = 19;
      static constexpr uint8_t MESSAGES_BATTLE_USE_ITEM = 20;
      static constexpr uint8_t MESSAGES_BATTLE_USE_SKILL = 21;
      static constexpr uint8_t MESSAGES_BATTLE_CAST_SPELL = 22;
      static constexpr uint8_t MESSAGES_BATTLE_INFLICT_POISON = 23;
      static constexpr uint8_t MESSAGES_BATTLE_INFLICT_POISON_FAIL = 24;
      static constexpr uint8_t MESSAGES_BATTLE_INFLICT_SLOW = 25;
      static constexpr uint8_t MESSAGES_BATTLE_INFLICT_SLOW_FAIL = 26;
      static constexpr uint8_t MESSAGES_BATTLE_RECOVERED_AMOUNT = 27;
      static constexpr uint8_t MESSAGES_BATTLE_CLEANSED = 28;
      static constexpr uint8_t MESSAGES_BATTLE_FOCUS1 = 29;
      static constexpr uint8_t MESSAGES_BATTLE_FOCUS2 = 30;
      static constexpr uint8_t MESSAGES_BATTLE_FOCUS3 = 31;
      static constexpr uint8_t MESSAGES_BATTLE_BERSERK1 = 32;
      static constexpr uint8_t MESSAGES_BATTLE_BERSERK2 = 33;
      static constexpr uint8_t MESSAGES_BATTLE_BERSERK3 = 34;
      static constexpr uint8_t MESSAGES_BATTLE_REMOVE_BERSERK = 35;
      static constexpr uint8_t MESSAGES_BATTLE_INFLICT_HASTE = 36;
      static constexpr uint8_t MESSAGES_BATTLE_REMOVE_POISON = 37;
      static constexpr uint8_t MESSAGES_BATTLE_REMOVE_SLOW = 38;
      static constexpr uint8_t MESSAGES_BATTLE_ATTACK_UP = 39;
      static constexpr uint8_t MESSAGES_BATTLE_DEFENSE_UP = 40;
      static constexpr uint8_t MESSAGES_BATTLE_AGILITY_UP = 41;
      static constexpr uint8_t MESSAGES_BATTLE_MAGIC_UP = 42;*/
    private:
      //static const uint8_t messages[];
      //static const uint8_t messageSizes[];
      static DataPack::PackedFile messagesFile;
      static uint8_t buffer[MESSAGES_SIZE];
      static uint8_t counter;
      static uint8_t messageSize;
      static uint8_t messageSpeed;
  };
}
