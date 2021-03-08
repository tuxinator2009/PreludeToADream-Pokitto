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
      static bool event_changeBattlerSprite();
      static bool event_changeBackgroundImage();
      static bool event_playBattleAnimation();
      static bool event_waitFrames();
      static bool event_waitButtons();
      static bool event_inflictStatus();
      static bool event_random();
      static bool event_endEventProcessing();
      static bool (*const execEvent[]) ();
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
