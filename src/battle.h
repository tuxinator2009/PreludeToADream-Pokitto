#pragma once

#include "headers.h"

namespace PTAD
{
	class Battle
	{
    friend class BattleEvent;
    friend class BattleAnimation;
		public:
      struct EnemyData
      {
        uint16_t experience;
        uint16_t hp;
        uint16_t maxHP;
        PTAD::Game::Stats stats;
        uint16_t outroOffset;
        uint16_t turnOffset;
        uint16_t spellResistance;
        uint8_t statusResistance[2];
        uint8_t mp;
        uint8_t gold;
      };
			enum class State
			{
				Init,
				SurpriseAttack,
        PlayerTurnInit,
        ChooseFightRun,
        Run,
        ChooseAction,
        Attack,
        ChooseItem,
        ChooseSkill,
				ChooseSpell,
				CastSpell,
				UseItem,
        UseSkill,
        EnemyTurnInit,
				EnemyTurn,
				Victory,
				Defeat
			};
			static void setup();
      static void update();
      static const uint32_t backgrounds[];
      static const uint32_t enemies[];
      static const uint32_t playerBattleAnimations[];
      static const uint8_t enemyNames[];
      static const uint8_t numEnemies;
		private:
			static bool showBackground();
			static void showFrames();
      static void loadBackgroundImage();
      static void loadBattlerSprite();
      static void loadPlayerSprite();
      static void flashBattlerSprite(uint8_t duration, uint8_t color1, uint8_t color2, uint8_t color3);
      static void flashPlayerSprite(uint8_t duration, uint8_t color1, uint8_t color2, uint8_t color3);
      static int attackDamageDealt(int attack, int agility1, int defense, int agility2, int resistance);
      static int magicDamageDealt(int attack, int magic1, int spellLevel, int defense, int magic2, int resistance);
			static void init();
			static void surpriseAttack();
			static void playerTurnInit();
      static void chooseFightRun();
      static void chooseAction();
      static void chooseItem();
      static void chooseSkill();
			static void chooseSpell();
      static void run();
      static void attack(uint8_t multiplier=1);
			static void useItem();
      static void useSkill();
      static void castSpell();
      static void enemyTurnInit();
			static void enemyTurn();
			static void victory();
			static void defeat();
      static void endOfPlayersTurn();
      static uint16_t getPlayerAttack();
      static uint16_t getPlayerDefense();
      static uint16_t getPlayerAgility();
      static uint16_t getPlayerMagic();
      static uint16_t getEnemyAttack();
      static uint16_t getEnemyDefense();
      static uint16_t getEnemyAgility();
      static uint16_t getEnemyMagic();
			static constexpr uint8_t ACTION_ATTACK = 0;
			static constexpr uint8_t ACTION_ITEM = 1;
			static constexpr uint8_t ACTION_SKILL = 2;
			static constexpr uint8_t ACTION_SPELL = 3;
      static constexpr uint8_t STATUS_POISON = 0;
      static constexpr uint8_t STATUS_SPEED = 2;
      static constexpr uint8_t STATUS_FOCUSED = 4;
      static constexpr uint8_t STATUS_BERSERK = 6;
      static constexpr uint8_t SPEED_NORMAL = 0;
      static constexpr uint8_t SPEED_SLOW = 1;
      static constexpr uint8_t SPEED_HASTE = 2;
			static const uint8_t showBackgroundOrder[];
			static const uint8_t txtAction[];
			static const uint8_t txtActions[];
      static const uint8_t txtStatsDisplay[];
      static const uint8_t txtNoEffect[];
      static const uint8_t txtNotEnoughMana[];
      static const uint8_t berserkFailRate[];
      static const uint16_t focusMultiplier[];
			static State state;
			static DataPack::PackedFile backgroundFile;
			static DataPack::PackedFile battlerFile;
			static DataPack::PackedFile enemyFile;
      static PTAD::Game::Stats playerStats;
			static uint8_t *backgroundImage;
			static uint8_t *battlerImage;
      static uint8_t *playerImage;
			static uint8_t shakeScreen;
      static uint8_t shakeRate;
			static uint8_t flashBattler;
      static uint8_t flashPlayer;
			static uint8_t flashUi;
      static uint8_t playerStatus;
      static uint8_t enemyStatus;
      static uint16_t playerSpellResistance;
      static uint8_t playerStatusResistance[2];
      static uint8_t messageBackup[96];
      static uint8_t turnCount;
      static uint32_t lastPress;
      static EnemyData *enemy;
      static bool keepFocus;
	};
};
