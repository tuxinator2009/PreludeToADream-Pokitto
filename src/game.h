#pragma once

#include "headers.h"

namespace PTAD
{
	class Game
	{
		public:
			enum class State
			{
				TitleInit,
				Title,
				MapInit,
				Map,
				BattleInit,
				Battle,
        MenuInit,
        Menu,
				GameOverInit,
				GameOver
      };
      struct Stats
      {
        uint16_t attack;
        uint16_t defense;
        uint16_t agility;
        uint16_t magic;
      };
			struct PlayerData
			{
				int32_t variables[4];
				uint32_t switches[2];
        uint16_t skills;
        uint16_t spells;
				uint8_t name[8];
				int32_t x, y;
				uint16_t hp, maxHP;
				uint8_t mp, maxMP;
				uint32_t experience, nextLevel;
				uint16_t gold;
        uint8_t level;
        Stats stats;
				uint32_t mapHash;
				uint8_t messageSpeed;
				uint8_t dir;
        uint8_t equippedItems[6];
        uint16_t equippableItems[6];
        uint8_t items[2][16];
        uint32_t enemiesScanned[8];
        uint32_t playTime;
			};
      struct SaveData
      {
        PlayerData slots[3];
        uint8_t unused[20];
      };
      //possible items
      //Potion
      //H-Potion
      //X-Potion
      //Antidote
      //Ether
      //H-Ether
      //X-Ether
      //Elixir
      //Shards
      //
			struct StatGrowth
			{
				uint32_t start;
				float base;
				float exponent;
			};
			static void setup();
			static void update();
			static constexpr uint32_t getStatForLevel(uint8_t level, const StatGrowth &growth);
			static void levelUp();
      static uint16_t calculateSpellResistance();
      static uint8_t getPoisonResistance();
      static uint8_t getSlowResistance();
      static void loadGame(uint8_t slot);
      static void saveGame(uint8_t slot);
      static void eraseGame(uint8_t slot);
      static SaveData getSaveData();
			static constexpr StatGrowth hpGrowth = {13,3,1.75};
			static constexpr StatGrowth mpGrowth = {1,0.25,1.5};
			static constexpr StatGrowth attackGrowth = {2,2,1.3};
			static constexpr StatGrowth defenseGrowth = {3,0.5,1.6};
			static constexpr StatGrowth agilityGrowth = {1,1.25,1.4};
			static constexpr StatGrowth magicGrowth = {0,0.75,1.5};
			static constexpr StatGrowth experienceGrowth = {10,6,1.6};
      static constexpr uint8_t ITEM_TYPE_CONSUMABLES = 0;
      static constexpr uint8_t ITEM_TYPE_TREASURES = 1;
      static constexpr uint8_t ITEM_TYPE_WEAPONS = 2;
      static constexpr uint8_t ITEM_TYPE_HELMETS = 3;
      static constexpr uint8_t ITEM_TYPE_ARMOR = 4;
      static constexpr uint8_t ITEM_TYPE_BOOTS = 5;
      static constexpr uint8_t ITEM_TYPE_RINGS = 6;
      static constexpr uint8_t ITEM_TYPE_AMULETS = 7;
      static constexpr uint8_t GAME_OVER_VALUE1 = 2;
      static constexpr uint8_t GAME_OVER_VALUE2 = 6;//TODO: set to max story progression variable for first game over screen (Cecilia chained up in boat hold)
			static PlayerData player;
			static State state;
      static State previousState;
      static uint32_t previousFrame;
      static uint8_t volume;
			static const uint8_t skillLearned[99];
      static const uint8_t messageSpeeds[];
      static const PlayerData newGame;
	};
}
