#pragma once

#include "headers.h"

namespace PTAD
{
	class MapEvent
	{
		public:
      struct Event
      {
        uint8_t x;
        uint8_t y;
        uint8_t flags;
        uint8_t spriteID;
        uint32_t offset;
      };
      static constexpr uint8_t FLAGS_ACTIVATE_BUTTON = 1;
      static constexpr uint8_t FLAGS_DOOR = 2;
      static constexpr uint8_t FLAGS_EXIT = 4;
      static constexpr uint8_t FLAGS_OFFSET = 8;
      static constexpr uint8_t FLAGS_NULL = 255;
			static void setup(uint32_t hash);
      static void begin(uint32_t offset);
			static bool update();
      static void runOnLoadEvent(uint32_t hash, uint32_t offset);
      static bool inMapEvent() {return !atEnd;}
		private:
      static void resetCounters();
      static uint8_t nextByte();
      static void readValue(uint8_t *value, size_t size);
			static bool hideScreen();
			static bool showScreen();
			static bool teleport();
			static bool showDialog();
			static bool hideDialog();
			static bool bufferMessage();
			static bool bufferValue();
      static bool bufferCharacter();
			static bool showMessage();
      static bool showOneLiner();
      static bool showShopMenu();
      static bool showGold();
      static bool hideGold();
			static bool waitButtons();
      static bool waitFrames();
			static bool battle();
			static bool shakeScreen();
			static bool cutscene();
			static bool jump();
			static bool jumpIfSwitch();
			static bool jumpIfVariable();
      static bool jumpIfHasItem();
      static bool jumpIfExited();
      static bool jumpIfYesNo();
      static bool jumpIfValue();
			static bool playSoundEffect();
      static bool playMusic();
      static bool pauseMusic();
      static bool resumeMusic();
      static bool waitMusic();
      static bool waitSoundEffect();
			static bool turnSwitchOn();
			static bool turnSwitchOff();
			static bool toggleSwitch();
      static bool turnSwitchRangeOn();
      static bool turnSwitchRangeOff();
      static bool toggleSwitchRange();
      static bool changeVariable();
      static bool changeSprite();
      static bool changeEventLocation();
      static bool changeEventFlags();
      static bool changePassability();
      static bool givePlayerItem();
      static bool givePlayerGold();
      static bool heal();
      static bool showImage();
      static bool hideImage();
      static bool movePlayer();
			static bool endEventProcessing();
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
			static constexpr uint8_t VALUE_VARIABLE = 14;
			static constexpr uint8_t VALUE_SUPPLIED = 15;
			static constexpr uint8_t CONDITION_EQUAL_TO = 0;
			static constexpr uint8_t CONDITION_NOT_EQUAL_TO = 1;
			static constexpr uint8_t CONDITION_GREATER_THAN = 2;
			static constexpr uint8_t CONDITION_GREATER_THAN_OR_EQUAL_TO = 3;
			static constexpr uint8_t CONDITION_LESS_THAN = 4;
			static constexpr uint8_t CONDITION_LESS_THAN_OR_EQUAL_TO = 5;
			static constexpr uint8_t OPERATION_SET = 0;
			static constexpr uint8_t OPERATION_ADD = 1;
			static constexpr uint8_t OPERATION_SUB = 2;
			static constexpr uint8_t OPERATION_MUL = 3;
			static constexpr uint8_t OPERATION_DIV = 4;
			static constexpr uint8_t OPERATION_MOD = 5;
      static constexpr uint8_t HIDE_INSTANT = 0;
      static constexpr uint8_t HIDE_FADEOUT = 1;
      static constexpr uint8_t HIDE_WIPE_DOWN = 2;
      static constexpr uint8_t HIDE_WIPE_UP = 3;
      static constexpr uint8_t HIDE_WIPE_IN = 4;
      static constexpr uint8_t HIDE_MASK_OUT = 5;
      static constexpr uint8_t HIDE_WHITE_OUT = 6;
      static constexpr uint8_t SHOW_INSTANT = 0;
      static constexpr uint8_t SHOW_FADEIN = 1;
      static constexpr uint8_t SHOW_UNWIPE_DOWN = 2;
      static constexpr uint8_t SHOW_UNWIPE_UP = 3;
      static constexpr uint8_t SHOW_UNWIPE_OUT = 4;
      static constexpr uint8_t SHOW_WHITE_IN = 5;
      static constexpr uint8_t PASSABILITY_SET = 0;
      static constexpr uint8_t PASSABILITY_CLEAR = 1;
      static constexpr uint8_t PASSABILITY_TOGGLE = 2;
      static constexpr uint8_t MOVE_UP = 0;
      static constexpr uint8_t MOVE_RIGHT = 1;
      static constexpr uint8_t MOVE_DOWN = 2;
      static constexpr uint8_t MOVE_LEFT = 3;
      static constexpr uint8_t MOVE_FORWARD = 4;
      static constexpr uint8_t MOVE_STRAFE_RIGHT = 5;
      static constexpr uint8_t MOVE_BACKWARD = 6;
      static constexpr uint8_t MOVE_STRAFE_LEFT = 7;
      static constexpr uint8_t MOVE_FACE_UP = 8;
      static constexpr uint8_t MOVE_FACE_RIGHT = 9;
      static constexpr uint8_t MOVE_FACE_DOWN = 10;
      static constexpr uint8_t MOVE_FACE_LEFT = 11;
      static constexpr uint8_t MOVE_TURN_LEFT = 12;
      static constexpr uint8_t MOVE_TURN_RIGHT = 13;
      static constexpr uint8_t MOVE_TURN_AROUND = 14;
      static constexpr uint8_t FLAG_SET = 0;
      static constexpr uint8_t FLAG_CLEAR = 1;
      static constexpr uint8_t FLAG_TOGGLE = 2;
      static const uint32_t wipeDownMasks[21];
      static const uint32_t wipeUpMasks[21];
      static const uint32_t wipeInMasks[11];
      static const uint8_t yesNoUI[2][8];
      static const uint8_t goldUI[2][7];
      static DataPack::PackedFile eventFile;
      static uint32_t eventFileHash;
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
}
