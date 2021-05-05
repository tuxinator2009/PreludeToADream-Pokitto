#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

const uint32_t PTAD::Battle::backgrounds[] =
{
	DataPack::hash("backdrops/field.gfx"),    //0x00 ( 0)
	DataPack::hash("backdrops/forest.gfx"),   //0x01 ( 1)
	DataPack::hash("backdrops/shipdeck.gfx"), //0x02 ( 2)
	DataPack::hash("backdrops/swamp.gfx"),    //0x03 ( 3)
	DataPack::hash("backdrops/cavern.gfx"),   //0x04 ( 4)
	DataPack::hash("backdrops/mountain.gfx"), //0x05 ( 5)
	DataPack::hash("backdrops/mountain2.gfx"),//0x06 ( 6)
};

const uint32_t PTAD::Battle::enemies[] =
{
  DataPack::hash("battlers/wolf.dat"),     //0x00 ( 0)
	DataPack::hash("battlers/snake.dat"),    //0x01 ( 1)
	DataPack::hash("battlers/rat.dat"),      //0x02 ( 2)
	DataPack::hash("battlers/spider.dat"),   //0x03 ( 3)
	DataPack::hash("battlers/brigand.dat"),  //0x04 ( 4)
	DataPack::hash("battlers/goliath.dat"),  //0x05 ( 5)
	DataPack::hash("battlers/bat.dat"),      //0x06 ( 6)
	DataPack::hash("battlers/benjamin.dat"), //0x07 ( 7)
	DataPack::hash("battlers/cougar.dat"),   //0x08 ( 8)
	DataPack::hash("battlers/grizzly.dat"),  //0x09 ( 9)
	DataPack::hash("battlers/gorilla.dat"),  //0x0A (10)
};

const uint8_t PTAD::Battle::enemyNames[]
{
  //Wolf____
  0x30,0x42,0x3F,0x39,0x0A,0x0A,0x0A,0x0A,
  //Snake___
  0x2C,0x41,0x34,0x3E,0x38,0x0A,0x0A,0x0A,
  //Rat_____
  0x2B,0x34,0x47,0x0A,0x0A,0x0A,0x0A,0x0A,
  //Spider__
  0x2C,0x43,0x3C,0x37,0x38,0x45,0x0A,0x0A,
  //Brigand_
  0x1B,0x45,0x3C,0x3A,0x34,0x41,0x37,0x0A,
  //Goliath_
  0x20,0x42,0x3F,0x3C,0x34,0x47,0x3B,0x0A,
  //Bat_____
  0x1B,0x34,0x47,0x0A,0x0A,0x0A,0x0A,0x0A,
  //Benjamin
  0x1B,0x38,0x41,0x3D,0x34,0x40,0x3C,0x41,
  //Cougar__
  0x1C,0x42,0x48,0x3A,0x34,0x45,0x0A,0x0A,
  //Grizzly_
  0x20,0x45,0x3C,0x4D,0x4D,0x3F,0x4C,0x0A,
  //Gorilla_
  0x20,0x42,0x45,0x3C,0x3F,0x3F,0x34,0x0A,
};

const uint8_t PTAD::Battle::numEnemies = sizeof(PTAD::Battle::enemies) / sizeof(PTAD::Battle::enemies[0]);

const uint8_t PTAD::Battle::showBackgroundOrder[] =
{
	 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
	18,19,19,20,20,21,21,22,22,23,23, 7,
	18,32,33,33,34,34,35,35,36,36,24, 7,
	17,32,42,43,43,44,44,45,45,37,24, 8,
	17,31,42,48,48,47,47,46,46,37,25, 8,
	16,31,41,41,40,40,39,39,38,38,25, 9,
	16,30,30,29,29,28,28,27,27,26,26, 9,
	15,15,14,14,13,13,12,12,11,11,10,10
};

const uint8_t PTAD::Battle::txtAction[] = {0x1A,0x1C,0x2D,0x22,0x28,0x27};
const uint8_t PTAD::Battle::txtActions[] = {0x0A,0x1F,0x3C,0x3A,0x3B,0x47,0x0A,0x0A,0x2B,0x48,0x41,0x0A,0x1A,0x47,0x47,0x34,0x36,0x3E,0x0A,0x22,0x47,0x38,0x40,0x0A,0x2C,0x3E,0x3C,0x3F,0x3F,0x0A,0x0A,0x2C,0x43,0x38,0x3F,0x3F};
const uint8_t PTAD::Battle::txtStatsDisplay[] = {0x25,0x38,0x49,0x38,0x21,0x29,0x0A,0x0A,0x26,0x34,0x41,0x34,0x1E,0x4B,0x43,0x0A,0x20,0x0A,0x0A,0x0A};
const uint8_t PTAD::Battle::txtNoEffect[] = {0x22,0x47,0x0A,0x4A,0x3C,0x3F,0x3F,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x38,0x39,0x39,0x38,0x36,0x47,0x02,0x0A};
const uint8_t PTAD::Battle::txtNotEnoughMana[] = {0x32,0x42,0x48,0x0A,0x37,0x42,0x41,0x04,0x47,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x38,0x41,0x42,0x48,0x3A,0x3B,0x0A,0x40,0x43};
const uint8_t PTAD::Battle::berserkFailRate[] = {0, 64, 128, 192};
const uint16_t PTAD::Battle::focusMultiplier[] = {1, 3, 5, 8};

PTAD::Battle::State PTAD::Battle::state;
DataPack::PackedFile PTAD::Battle::backgroundFile;
DataPack::PackedFile PTAD::Battle::battlerFile;
DataPack::PackedFile PTAD::Battle::enemyFile;
PTAD::Game::Stats PTAD::Battle::playerStats;
uint8_t *PTAD::Battle::backgroundImage = PTAD::memory + PTAD::MEMORY_BATTLE_BACKGROUND_OFFSET;
uint8_t *PTAD::Battle::battlerImage = PTAD::memory + PTAD::MEMORY_BATTLE_BATTLER_OFFSET;
uint8_t *PTAD::Battle::playerImage = PTAD::memory + PTAD::MEMORY_BATTLE_PLAYER_OFFSET;
uint8_t PTAD::Battle::shakeScreen = 0;
uint8_t PTAD::Battle::shakeRate = 0;
uint8_t PTAD::Battle::flashBattler = 0;
uint8_t PTAD::Battle::flashPlayer = 0;
uint8_t PTAD::Battle::flashUi = 0;
uint8_t PTAD::Battle::playerStatus;
uint8_t PTAD::Battle::enemyStatus;
uint16_t PTAD::Battle::playerSpellResistance = 0;
uint8_t PTAD::Battle::playerStatusResistance[2] = {0, 0};
uint8_t PTAD::Battle::messageBackup[96];
uint8_t PTAD::Battle::turnCount;
uint32_t PTAD::Battle::lastPress = 0;
PTAD::Battle::EnemyData *PTAD::Battle::enemy = (PTAD::Battle::EnemyData*)(PTAD::memory + PTAD::MEMORY_BATTLE_ENEMY_OFFSET);
bool PTAD::Battle::keepFocus = false;

void PTAD::Battle::setup()
{
	state = State::Init;
  PTAD::dataFile->getPackedFile(DataPack::hash("battlers/battlers.gfx"), &battlerFile);
	PTAD::dataFile->getPackedFile(enemies[PTAD::battleMonsterID], &enemyFile);
	PTAD::dataFile->readBytes(&enemyFile, (void*)enemy, sizeof(EnemyData));
	PTAD::globalCounter = 0;
  PTAD::Music::playMusic(PTAD::battleBGM, 1);
  loadBackgroundImage();
  loadBattlerSprite();
  loadPlayerSprite();
  PTAD::BattleEvent::setup(enemyFile);
  playerStatus = 0;
  enemyStatus = 0;
  playerStats = PTAD::Game::player.stats;
  playerSpellResistance = PTAD::Game::calculateSpellResistance();
  playerStatusResistance[0] = PTAD::Game::getPoisonResistance();
  playerStatusResistance[1] = PTAD::Game::getSlowResistance();
  if (PTAD::Map::isDark())
  {
    if (PTAD::Game::player.equippedItems[1] == 1)
      PTAD::darkness = 3;
    else
      PTAD::darkness = 1;
  }
}

void PTAD::Battle::update()
{
  if (shakeScreen > 0 && PC::frameCount % shakeRate == 0)
  {
    if (shakeScreen != 255)
      --shakeScreen;
    PD::shiftTilemap(random(0, 8), random(0, 8));
  }
  else if (shakeScreen == 0)
    PD::shiftTilemap(2, 0);
	if (flashBattler > 0)
	{
    --flashBattler;
    if (flashBattler == 0)
      loadBattlerSprite();
	}
	if (flashPlayer > 0)
  {
    --flashPlayer;
    if (flashPlayer == 0)
      loadPlayerSprite();
  }
	if (flashUi > 0)
    --flashUi;
  else if (PTAD::Game::player.hp > PTAD::Game::player.maxHP / 4)
    PTAD::Ui::fgColor = 7;
  else if (PTAD::Game::player.hp > 0)
    PTAD::Ui::fgColor = 95;
  else
    PTAD::Ui::fgColor = 175;
	if (state != State::Init)
	{
		PTAD::Ui::drawNumber(PTAD::Game::player.level, 9, 6, 10);
		PTAD::Ui::drawNumber(PTAD::Game::player.hp, 7, 7, 1000);
    PTAD::Ui::drawNumber(PTAD::Game::player.mp, 8, 8, 100);
    PTAD::Ui::drawNumber((PTAD::Game::player.experience < PTAD::Game::player.nextLevel) ? (PTAD::Game::player.nextLevel - PTAD::Game::player.experience):0, 7, 9, 1000);
		PTAD::Ui::drawNumber(PTAD::Game::player.gold, 6, 10, 10000);
    if (state != State::ChooseSpell && state != State::ChooseItem && state != State::ChooseSkill && (state != State::UseSkill || lastPress != PTAD::SKILL_EXAMINE))
    {
      if (enemy->hp > 0)
        PD::drawSpriteBitmap(134, 52, 48, 48, battlerImage);
      if (PTAD::Game::player.hp > 0)
        PD::drawSpriteBitmap(150, 95, 16, 16, playerImage);
    }
    if (state != State::Victory)
    {
      if (((enemyStatus >> STATUS_POISON) & 3) != 0)
        PD::drawSpriteBitmap(22, 12, 16, 16, PTAD::statusIcons + ((((enemyStatus >> STATUS_POISON) & 3) - 1) * 256));
      if (((enemyStatus >> STATUS_SPEED) & 3) != 0)
        PD::drawSpriteBitmap(22 + 16, 12, 16, 16, PTAD::statusIcons + 768 + ((((enemyStatus >> STATUS_SPEED) & 3) - 1) * 256));
      if (((enemyStatus >> STATUS_FOCUSED) & 3) != 0)
        PD::drawSpriteBitmap(22 + 32, 12, 16, 16, PTAD::statusIcons + 1536 + ((((enemyStatus >> STATUS_FOCUSED) & 3) - 1) * 256));
      if (((enemyStatus >> STATUS_BERSERK) & 3) != 0)
        PD::drawSpriteBitmap(22 + 48, 12, 16, 16, PTAD::statusIcons + 2304 + ((((enemyStatus >> STATUS_BERSERK) & 3) - 1) * 256));
      if (((playerStatus >> STATUS_POISON) & 3) != 0)
        PD::drawSpriteBitmap(22, 88, 16, 16, PTAD::statusIcons + ((((playerStatus >> STATUS_POISON) & 3) - 1) * 256));
      if (((playerStatus >> STATUS_SPEED) & 3) != 0)
        PD::drawSpriteBitmap(22 + 16, 88, 16, 16, PTAD::statusIcons + 768 + ((((playerStatus >> STATUS_SPEED) & 3) - 1) * 256));
      if (((playerStatus >> STATUS_FOCUSED) & 3) != 0)
        PD::drawSpriteBitmap(22 + 32, 88, 16, 16, PTAD::statusIcons + 1536 + ((((playerStatus >> STATUS_FOCUSED) & 3) - 1) * 256));
      if (((playerStatus >> STATUS_BERSERK) & 3) != 0)
        PD::drawSpriteBitmap(22 + 48, 88, 16, 16, PTAD::statusIcons + 2304 + ((((playerStatus >> STATUS_BERSERK) & 3) - 1) * 256));
    }
    if (PTAD::BattleAnimation::isAnimationPlaying())
      PTAD::BattleAnimation::renderFrame();
	}
	if (PTAD::Dialog::updateMessage())
		return;
  if (PTAD::BattleAnimation::isAnimationPlaying())
    return;
  if (!PTAD::BattleEvent::update())
    return;
	switch (state)
	{
		case State::Init:
			init();
			break;
		case State::SurpriseAttack:
			surpriseAttack();
			break;
		case State::PlayerTurnInit:
			playerTurnInit();
			break;
    case State::ChooseFightRun:
      chooseFightRun();
      break;
    case State::Run:
      run();
      break;
		case State::ChooseAction:
			chooseAction();
      break;
    case State::Attack:
      attack();
      break;
    case State::ChooseItem:
      chooseItem();
      break;
    case State::ChooseSkill:
      chooseSkill();
      break;
		case State::ChooseSpell:
			chooseSpell();
			break;
		case State::UseItem:
			useItem();
      break;
    case State::UseSkill:
      useSkill();
      break;
    case State::CastSpell:
      castSpell();
      break;
    case State::EnemyTurnInit:
      enemyTurnInit();
      [[fallthrough]];
		case State::EnemyTurn:
			enemyTurn();
			break;
		case State::Victory:
			victory();
			break;
		case State::Defeat:
			defeat();
			break;
	}
}

bool PTAD::Battle::showBackground()
{
	int count = 0;
	if (PTAD::justPressed(PTAD::BTN_MASK_A | PTAD::BTN_MASK_B) || PB::cBtn())
		PTAD::globalCounter = 255;
	for (uint8_t y = 0; y < 8; ++y)
	{
		for (uint8_t x = 0; x < 12; ++x)
		{
			if (showBackgroundOrder[y * 12 + x] <= PTAD::globalCounter)
			{
				PTAD::drawTile(x + 14, y + 6, y * 12 + x + 128);
        PTAD::drawTile(x + 14, y + 6, 255, false);
				++count;
			}
		}
	}
	if (count < 96)
		return false;
	return true;
}

void PTAD::Battle::showFrames()
{
  PTAD::Ui::drawFrame(2, 1, 11, 3); //Enemy status icons
  for (uint8_t i = 0; i < 8; ++i)
    PTAD::Ui::drawCharacter(enemyNames[PTAD::battleMonsterID * 8 + i], i + 3, 1);
  for (uint8_t i = 10; i >= 3; --i)
  {
    if (PTAD::Ui::getCharacter(i, 1) == PTAD::FONT_SPACE)
      PTAD::Ui::drawCharacter(PTAD::FONT_FRAME_U, i, 1);
    else
      break;
  }
  PTAD::Ui::fixFrameHeader(2, 11, 3);
	PTAD::Ui::drawFrame(2, 5, 11, 13); //Stats frame
  for (uint8_t i = 0; i < 8; ++i)
  {
    if (PTAD::Game::player.name[i] != PTAD::FONT_SPACE)
      PTAD::Ui::drawCharacter(PTAD::Game::player.name[i], i + 3, 5);
  }
  PTAD::Ui::fixFrameHeader(2, 11, 5);
	for (uint8_t i = 0; i < 5; ++i)
		PTAD::Ui::drawText(txtStatsDisplay + i * 4, 4, 3, i + 6);
	PTAD::Ui::drawCharacter(PTAD::FONT_l, 7, 6);
	PTAD::Ui::drawFrame(13, 1, 26, 4); //Command frame
	PTAD::Ui::drawText(txtAction, 6, 17, 1);
	PTAD::Ui::drawText(txtActions, 12, 14, 2);
  PTAD::Ui::fixFrameHeader(13, 26, 1);
	PTAD::Dialog::showWindow();
}

void PTAD::Battle::loadBackgroundImage()
{
  PTAD::dataFile->getPackedFile(backgrounds[PTAD::battleBG], &backgroundFile);
  PTAD::dataFile->readBytes(&backgroundFile, backgroundImage, PTAD::MEMORY_BATTLE_BACKGROUND_SIZE);
}

void PTAD::Battle::loadBattlerSprite()
{
  battlerFile.seek(PTAD::MEMORY_BATTLE_BATTLER_SIZE * PTAD::battleMonsterID);
  PTAD::dataFile->readBytes(&battlerFile, battlerImage, PTAD::MEMORY_BATTLE_BATTLER_SIZE);
}

void PTAD::Battle::loadPlayerSprite()
{
  const uint8_t *src = PTAD::playerSprite + 384;
  uint8_t *dst = playerImage;
  for (int i = 0; i < MEMORY_BATTLE_PLAYER_SIZE; ++i)
    *dst++ = *src++;
}

void PTAD::Battle::flashBattlerSprite(uint8_t duration, uint8_t color1, uint8_t color2, uint8_t color3)
{
  loadBattlerSprite();
  flashBattler = duration;
  for (uint32_t i = 0; i < PTAD::MEMORY_BATTLE_BATTLER_SIZE; ++i)
  {
    if (battlerImage[i] != 0)
    {
      if (battlerImage[i] < color2)
        battlerImage[i] = (battlerImage[i] % 8) + color1;
      else if (battlerImage[i] < color3)
        battlerImage[i] = (battlerImage[i] % 8) + color2;
      else
        battlerImage[i] = (battlerImage[i] % 8) + color3;
    }
  }
}

void PTAD::Battle::flashPlayerSprite(uint8_t duration, uint8_t color1, uint8_t color2, uint8_t color3)
{
  loadPlayerSprite();
  flashPlayer = duration;
  for (uint32_t i = 0; i < PTAD::MEMORY_BATTLE_PLAYER_SIZE; ++i)
  {
    if (playerImage[i] != 0)
    {
      if (playerImage[i] < color2)
        playerImage[i] = (playerImage[i] % 8) + color1;
      else if (playerImage[i] < color3)
        playerImage[i] = (playerImage[i] % 8) + color2;
      else
        playerImage[i] = (playerImage[i] % 8) + color3;
    }
  }
}

int PTAD::Battle::attackDamageDealt(int attack, int agility1, int defense, int agility2, int resistance)
{
	if (random(0, agility1 * 16) <= agility2)
    return 0;
  int damage = attack / 2 - defense / 4;
  if (resistance == 0)
    damage = 1;
  else if (resistance == 1)
    damage /= 2;
  else if (resistance == 3)
    damage *= 2;
	return std::min(999, std::max(1, damage));
}

int PTAD::Battle::magicDamageDealt(int attack, int magic1, int spellLevel, int defense, int magic2, int resistance)
{
  int damage = (attack * magic1 * spellLevel) / 4 - (defense * magic2) / 8;
  if (resistance == 0)
    damage = 1;
  else if (resistance == 1)
    damage /= 2;
  else if (resistance == 3)
    damage *= 2;
  return std::min(999, std::max(1, damage));
}

void PTAD::Battle::init()
{
	if (showBackground())
	{
		if (random(0, getEnemyAgility()) > getPlayerAgility() && !PTAD::MapEvent::inMapEvent())
			state = State::SurpriseAttack;
		else
			state = State::PlayerTurnInit;
		PTAD::globalCounter = 0;
		showFrames();
    PTAD::BattleEvent::begin(0);
	}
	else
		++PTAD::globalCounter;
}

void PTAD::Battle::surpriseAttack()
{
	PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_SURPRISE_ATTACK);
	PTAD::Dialog::beginMessage();
	PTAD::globalCounter = 1;
  state = State::EnemyTurnInit;
}

void PTAD::Battle::playerTurnInit()
{
  uint8_t playerSpeed = (playerStatus >> STATUS_SPEED) & 3;
  uint8_t enemySpeed = (enemyStatus >> STATUS_SPEED) & 3;
	state = State::ChooseFightRun;
	PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_CHOOSE_ACTION);
	PTAD::Dialog::beginMessage();
  if (turnCount == 0)
  {
    if (playerSpeed == SPEED_NORMAL && enemySpeed == SPEED_SLOW)
      turnCount = 2;
    else if (playerSpeed == SPEED_HASTE && enemySpeed == SPEED_NORMAL)
      turnCount = 2;
    else if (playerSpeed == SPEED_HASTE && enemySpeed == SPEED_SLOW)
      turnCount = 3;
    else
      turnCount = 1;
  }
  keepFocus = false;
}

void PTAD::Battle::chooseFightRun()
{
  PTAD::Ui::drawText(txtActions, 12, 14, 2);
  PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 12, 14, 3);
  if (PTAD::justPressed(PTAD::BTN_MASK_LEFT) || PTAD::justPressed(PTAD::BTN_MASK_RIGHT))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    PTAD::globalCounter ^= 1;
  }
  PTAD::Ui::cursorX = 102 + PTAD::globalCounter * 56;
  PTAD::Ui::cursorY = 16;
  if (PTAD::justPressed(BTN_MASK_A) || PB::cBtn())
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
    if (PTAD::globalCounter == 0)
      state = State::ChooseAction;
    else
      state = State::Run;
    PTAD::globalCounter = 0;
  }
  else if ((playerStatus >> STATUS_BERSERK) & 3 != 0)
  {
    state = State::ChooseAction;
    PTAD::globalCounter = 0;
  }
}

void PTAD::Battle::chooseAction()
{
  PTAD::Ui::drawText(txtActions + 12, 12, 14, 2);
  PTAD::Ui::drawText(txtActions + 24, 12, 14, 3);
  if (PTAD::justPressed(PTAD::BTN_MASK_LEFT) || PTAD::justPressed(PTAD::BTN_MASK_RIGHT))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    PTAD::globalCounter ^= 1;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_UP) || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    PTAD::globalCounter ^= 2;
  }
  PTAD::Ui::cursorX = 94 + (PTAD::globalCounter % 2) * 56;
  PTAD::Ui::cursorY = 16 + (PTAD::globalCounter / 2) * 8;
  if (PTAD::justPressed(BTN_MASK_A) || PB::cBtn())
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
		if (PTAD::globalCounter == ACTION_ATTACK)
      state = State::Attack;
    else if (PTAD::globalCounter == ACTION_ITEM)
    {
      state = State::ChooseItem;
      PTAD::Ui::backupSection(messageBackup, 2, 16, 25, 19);
      PTAD::Ui::drawFrame(14, 1, 26, 18);
      PTAD::Ui::drawFrame(1, 18, 26, 20);
      for (int i = 0; i < 16; ++i)
      {
        if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][i] > 0)
        {
          PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][i].name, 8, 15, i + 2);
          PTAD::Ui::drawNumber(PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][i], 24, i + 2, 10);
        }
      }
      if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][0] > 0)
        PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][0].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
		else if (PTAD::globalCounter == ACTION_SKILL)
    {
			state = State::ChooseSkill;
      PTAD::Ui::backupSection(messageBackup, 2, 16, 25, 19);
      PTAD::Ui::drawFrame(14, 1, 26, 18);
      PTAD::Ui::drawFrame(1, 18, 26, 20);
      for (int i = 0; i < 16; ++i)
      {
        if (((PTAD::Game::player.skills >> i) & 1) != 0)
        {
          PTAD::Ui::drawText(PTAD::skills[i].name, 8, 15, i + 2);
          PTAD::Ui::drawNumber(PTAD::skills[i].price, 24, i + 2, 10);
        }
      }
      if ((PTAD::Game::player.skills & 1) != 0)
        PTAD::Ui::drawText(PTAD::skills[0].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
    else if (PTAD::globalCounter == ACTION_SPELL)
    {
      state = State::ChooseSpell;
      PTAD::Ui::backupSection(messageBackup, 2, 16, 25, 19);
      PTAD::Ui::drawFrame(14, 1, 26, 18);
      PTAD::Ui::drawFrame(1, 18, 26, 20);
      for (int i = 0; i < 16; ++i)
      {
        if (((PTAD::Game::player.spells >> i) & 1) != 0)
        {
          PTAD::Ui::drawText(PTAD::spells[i].name, 8, 15, i + 2);
          PTAD::Ui::drawNumber(PTAD::spells[i].price, 24, i + 2, 10);
        }
      }
      if ((PTAD::Game::player.spells & 1) != 0)
        PTAD::Ui::drawText(PTAD::spells[0].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
		PTAD::globalCounter = 0;
	}
	else if (PTAD::justPressed(BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    state = State::ChooseFightRun;
    PTAD::globalCounter = 0;
  }
}

void PTAD::Battle::chooseItem()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 15;
      if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter] > 0)
        PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 16)
        PTAD::globalCounter = 0;
      if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter] > 0)
        PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter] == 0)
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    else if (PTAD::globalCounter >= PTAD::ITEM_POTION && PTAD::globalCounter <= PTAD::ITEM_S_POTION && PTAD::Game::player.hp == PTAD::Game::player.maxHP)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter >= PTAD::ITEM_ETHER && PTAD::globalCounter <= PTAD::ITEM_S_ETHER && PTAD::Game::player.mp == PTAD::Game::player.maxMP)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::ITEM_ELIXIR && PTAD::Game::player.hp != PTAD::Game::player.maxHP && PTAD::Game::player.mp != PTAD::Game::player.maxMP)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::ITEM_ANTIDOTE && ((playerStatus >> STATUS_POISON) & 3) == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::ITEM_STIMULNT && ((playerStatus >> STATUS_SPEED) & 3) != SPEED_SLOW)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::ITEM_HERBS && ((playerStatus >> STATUS_BERSERK) & 3) == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::ITEM_REMEDY && (playerStatus & 0xF) == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::ITEM_SMOKE_B && battleBGM != 2)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      state = State::UseItem;
      lastPress = PTAD::globalCounter;
      PTAD::globalCounter = 0;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    state = State::ChooseAction;
    PTAD::globalCounter = ACTION_ITEM;
  }
  if (state == State::ChooseItem)
  {
    PTAD::Ui::cursorX = 102;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
  else
  {
    PTAD::Ui::fill(14, 1, 26, 20, 255);
    showFrames();
    PTAD::Ui::restoreBackup(messageBackup, 2, 16, 25, 19);
  }
}

void PTAD::Battle::chooseSkill()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 15;
      if (((PTAD::Game::player.skills >> PTAD::globalCounter) & 1) != 0)
        PTAD::Ui::drawText(PTAD::skills[PTAD::globalCounter].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 16)
        PTAD::globalCounter = 0;
      if (((PTAD::Game::player.skills >> PTAD::globalCounter) & 1) != 0)
        PTAD::Ui::drawText(PTAD::skills[PTAD::globalCounter].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (((PTAD::Game::player.skills >> PTAD::globalCounter) & 1) == 0)
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    else if (PTAD::globalCounter == PTAD::SKILL_FOCUS_1 && ((playerStatus >> STATUS_FOCUSED) & 3) != 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_FOCUS_2 && ((playerStatus >> STATUS_FOCUSED) & 3) != 1)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_FOCUS_3 && ((playerStatus >> STATUS_FOCUSED) & 3) != 2)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_DASH && ((playerStatus >> STATUS_SPEED) & 3) == SPEED_HASTE)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_ENRAGE_1 && ((playerStatus >> STATUS_BERSERK) & 3) != 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_ENRAGE_2 && ((playerStatus >> STATUS_BERSERK) & 3) != 1)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_ENRAGE_3 && ((playerStatus >> STATUS_BERSERK) & 3) != 2)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_MEDITATE && ((playerStatus >> STATUS_BERSERK) & 3) == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_UNKNOWN1)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_UNKNOWN2)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_UNKNOWN3)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_UNKNOWN4)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SKILL_UNKNOWN5)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::Game::player.mp < PTAD::skills[PTAD::globalCounter].price)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNotEnoughMana, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      state = State::UseSkill;
      lastPress = PTAD::globalCounter;
      PTAD::globalCounter = 0;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    state = State::ChooseAction;
    PTAD::globalCounter = ACTION_SKILL;
  }
  if (state == State::ChooseSkill)
  {
    PTAD::Ui::cursorX = 102;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
  else
  {
    PTAD::Ui::fill(14, 1, 26, 20, 255);
    showFrames();
    PTAD::Ui::restoreBackup(messageBackup, 2, 16, 25, 19);
  }
}

void PTAD::Battle::chooseSpell()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 15;
      if (((PTAD::Game::player.spells >> PTAD::globalCounter) & 1) != 0)
        PTAD::Ui::drawText(PTAD::spells[PTAD::globalCounter].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 16)
        PTAD::globalCounter = 0;
      if (((PTAD::Game::player.spells >> PTAD::globalCounter) & 1) != 0)
        PTAD::Ui::drawText(PTAD::spells[PTAD::globalCounter].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (((PTAD::Game::player.spells >> PTAD::globalCounter) & 1) == 0)
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    else if (PTAD::globalCounter == PTAD::SPELL_ENVENOM && ((enemyStatus >> STATUS_POISON) & 3) == 3)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SPELL_ENSNARE && ((enemyStatus >> STATUS_SPEED) & 3) == SPEED_SLOW)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SPELL_RECOVERY && PTAD::Game::player.hp == PTAD::Game::player.maxHP)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::globalCounter == PTAD::SPELL_CLEANSER && (playerStatus & 0x0F) == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNoEffect, 24, 2, 19);
    }
    else if (PTAD::Game::player.mp < PTAD::spells[PTAD::globalCounter].price)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(txtNotEnoughMana, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      state = State::CastSpell;
      lastPress = PTAD::globalCounter;
      PTAD::globalCounter = 0;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    state = State::ChooseAction;
    PTAD::globalCounter = ACTION_SPELL;
  }
  if (state == State::ChooseSpell)
  {
    PTAD::Ui::cursorX = 102;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
  else
  {
    PTAD::Ui::fill(14, 1, 26, 20, 255);
    showFrames();
    PTAD::Ui::restoreBackup(messageBackup, 2, 16, 25, 19);
  }
}

void PTAD::Battle::run()
{
	if (PTAD::globalCounter == 0)
	{
		if (battleBGM != 2)
		{
			PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RUN_IMPOSSIBLE);
			state = State::PlayerTurnInit;
		}
		if (random(0, getPlayerAgility() * 16) <= getEnemyAgility())
		{
      PTAD::Music::playSFX(PTAD::Music::SFX_MISS);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RUN_FAIL);
      endOfPlayersTurn();
		}
		else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_RUN);
			PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RUN_SUCCESS);
    }
		PTAD::Dialog::beginMessage();
		++PTAD::globalCounter;
	}
	else
	{
		PTAD::globalCounter = 0;
		PTAD::Game::state = PTAD::Game::State::MapInit;
		PTAD::Ui::clear();
	}
}

void PTAD::Battle::attack(uint8_t multiplier)
{
  if (PTAD::globalCounter == 0)
  {
    PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_FIGHT);
    PTAD::Dialog::beginMessage();
  }
  if (PTAD::globalCounter == 1)
  {
    int damageDealt = attackDamageDealt(getPlayerAttack() * multiplier, getPlayerAgility(), getEnemyDefense(), getEnemyAgility(), (enemy->spellResistance >> 14) & 3);
    if (damageDealt == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_MISS);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_MISS);
    }
    else if (random(0, 100) < 5)
    {
      flashBattlerSprite(16, 8, 88, 168);
      PTAD::Music::playSFX(PTAD::Music::SFX_HIT);
      damageDealt *= 2;
      enemyStatus &= ~(3 << STATUS_FOCUSED);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_CRITICAL_HIT);
      PTAD::Dialog::bufferNumber(damageDealt, 100);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DAMAGE_TAKEN_END);
    }
    else
    {
      flashBattlerSprite(16, 8, 88, 168);
      PTAD::Music::playSFX(PTAD::Music::SFX_HIT);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DAMAGE_DEALT);
      PTAD::Dialog::bufferNumber(damageDealt, 100);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DAMAGE_TAKEN_END);
    }
    if (damageDealt > enemy->hp)
      enemy->hp = 0;
    else
      enemy->hp -= damageDealt;
    PTAD::Dialog::beginMessage();
  }
  else if (PTAD::globalCounter == 2)
  {
    PTAD::globalCounter = -1;
    if (enemy->hp == 0)
      state = State::Victory;
    else
      endOfPlayersTurn();
  }
  ++PTAD::globalCounter;
}

void PTAD::Battle::useItem()
{
  if (PTAD::globalCounter == 0)
  {
    if (random(0, 255) < berserkFailRate[(playerStatus >> STATUS_BERSERK) & 3] && lastPress != PTAD::ITEM_HERBS) //attack instead
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_BERSERK_ATTACK);
      PTAD::Dialog::beginMessage();
      state = State::Attack;
      return;
    }
    PTAD::Music::playSFX(PTAD::Music::SFX_USEITEM);
    PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_USE_ITEM);
    PTAD::Dialog::bufferText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][lastPress].name, 8);
    PTAD::Dialog::trimBuffer();
    PTAD::Dialog::beginMessage();
    --PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][lastPress];
  }
  else if (PTAD::globalCounter == 1)
  {
    if (lastPress == PTAD::ITEM_POTION)
    {
      PTAD::Game::player.hp += 50;
      if (PTAD::Game::player.hp > PTAD::Game::player.maxHP)
        PTAD::Game::player.hp = PTAD::Game::player.maxHP;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RECOVERED_AMOUNT);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_5);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_0);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_H);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_P);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_H_POTION)
    {
      PTAD::Game::player.hp += 150;
      if (PTAD::Game::player.hp > PTAD::Game::player.maxHP)
        PTAD::Game::player.hp = PTAD::Game::player.maxHP;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RECOVERED_AMOUNT);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_1);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_5);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_0);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_H);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_P);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_S_POTION)
    {
      PTAD::Game::player.hp += 300;
      if (PTAD::Game::player.hp > PTAD::Game::player.maxHP)
        PTAD::Game::player.hp = PTAD::Game::player.maxHP;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RECOVERED_AMOUNT);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_3);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_0);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_0);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_H);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_P);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_ETHER)
    {
      PTAD::Game::player.mp += 5;
      if (PTAD::Game::player.mp > PTAD::Game::player.maxMP)
        PTAD::Game::player.mp = PTAD::Game::player.maxMP;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RECOVERED_AMOUNT);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_5);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_m);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_n);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_H_ETHER)
    {
      PTAD::Game::player.mp += 15;
      if (PTAD::Game::player.mp > PTAD::Game::player.maxMP)
        PTAD::Game::player.mp = PTAD::Game::player.maxMP;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RECOVERED_AMOUNT);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_1);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_5);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_m);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_n);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_S_ETHER)
    {
      PTAD::Game::player.mp += 30;
      if (PTAD::Game::player.mp > PTAD::Game::player.maxMP)
        PTAD::Game::player.mp = PTAD::Game::player.maxMP;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RECOVERED_AMOUNT);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_3);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_0);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_m);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_n);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_ELIXIR)
    {
      PTAD::Game::player.hp += 150;
      PTAD::Game::player.mp += 15;
      if (PTAD::Game::player.hp > PTAD::Game::player.maxHP)
        PTAD::Game::player.hp = PTAD::Game::player.maxHP;
      if (PTAD::Game::player.mp > PTAD::Game::player.maxMP)
        PTAD::Game::player.mp = PTAD::Game::player.maxMP;
      PTAD::Dialog::bufferCharacter(PTAD::FONT_1);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_5);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_0);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_H);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_P);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_n);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_d);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_1);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_5);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_m);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_n);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_a);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_ANTIDOTE)
    {
      playerStatus &= ~(3 << STATUS_POISON);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_REMOVE_POISON);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_STIMULNT)
    {
      playerStatus &= ~(3 << STATUS_SPEED);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_REMOVE_SLOW);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_HERBS)
    {
      playerStatus &= ~(3 << STATUS_BERSERK);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_REMOVE_BERSERK);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_REMEDY)
    {
      playerStatus &= 0xF0;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_CLEANSED);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_X_ATTACK)
    {
      playerStats.attack = (playerStats.attack * 3) / 2;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_ATTACK_UP);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_X_DEFEND)
    {
      playerStats.defense = (playerStats.defense * 3) / 2;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DEFENSE_UP);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_X_SPEED)
    {
      playerStats.agility = (playerStats.agility * 3) / 2;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_AGILITY_UP);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_X_MAGIC)
    {
      playerStats.magic = (playerStats.magic * 3) / 2;
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_MAGIC_UP);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::ITEM_SMOKE_B)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RUN_SUCCESS);
      PTAD::Dialog::beginMessage();
      ++PTAD::globalCounter;
    }
  }
  else if (PTAD::globalCounter == 2)
  {
    PTAD::globalCounter = -1;
    endOfPlayersTurn();
  }
  else
  {
    PTAD::globalCounter = 0;
    PTAD::Game::state = PTAD::Game::State::MapInit;
    PTAD::Ui::clear();
  }
  ++PTAD::globalCounter;
}

void PTAD::Battle::useSkill()
{
  if (PTAD::globalCounter == 0)
  {
    if (random(0, 255) < berserkFailRate[(playerStatus >> STATUS_BERSERK) & 3] && lastPress < PTAD::SKILL_ENRAGE_1 && lastPress > PTAD::SKILL_MEDITATE) //attack instead
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_BERSERK_ATTACK);
      PTAD::Dialog::beginMessage();
      state = State::Attack;
      return;
    }
    PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_USE_SKILL);
    PTAD::Dialog::bufferText(PTAD::skills[lastPress].name, 8);
    PTAD::Dialog::trimBuffer();
    PTAD::Dialog::beginMessage();
    //TODO: Play skill's battle animation
    PTAD::Game::player.mp -= PTAD::skills[lastPress].price;
  }
  else if (PTAD::globalCounter == 1)
  {
    if (lastPress == PTAD::SKILL_EXAMINE)
    {
      PTAD::Ui::drawFrame(15, 5, 24, 15);
      PTAD::Ui::drawCharacter(PTAD::FONT_H, 16, 6);
      PTAD::Ui::drawCharacter(PTAD::FONT_P, 17, 6);
      PTAD::Ui::drawNumber(enemy->hp, 19, 6, 10000);
      PTAD::Ui::drawCharacter(PTAD::FONT_M, 16, 7);
      PTAD::Ui::drawCharacter(PTAD::FONT_P, 17, 7);
      PTAD::Ui::drawNumber(enemy->mp, 21, 7, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_E, 16, 8);
      PTAD::Ui::drawCharacter(PTAD::FONT_x, 17, 8);
      PTAD::Ui::drawCharacter(PTAD::FONT_p, 18, 8);
      PTAD::Ui::drawNumber(enemy->experience, 20, 8, 1000);
      PTAD::Ui::drawCharacter(PTAD::FONT_G, 16, 9);
      PTAD::Ui::drawCharacter(PTAD::FONT_o, 17, 9);
      PTAD::Ui::drawCharacter(PTAD::FONT_l, 18, 9);
      PTAD::Ui::drawCharacter(PTAD::FONT_d, 19, 9);
      PTAD::Ui::drawNumber(enemy->gold, 21, 9, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_A, 16, 10);
      PTAD::Ui::drawNumber(enemy->stats.attack, 17, 10, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_D, 20, 10);
      PTAD::Ui::drawNumber(enemy->stats.defense, 21, 10, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_S, 16, 11);
      PTAD::Ui::drawNumber(enemy->stats.agility, 17, 11, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_M, 20, 11);
      PTAD::Ui::drawNumber(enemy->stats.magic, 21, 11, 100);
      for (int i = 0; i < 7; ++i)
      {
        PTAD::Ui::drawCharacter(PTAD::FONT_ICON_FIRE + i, (i % 4) * 2 + 16, (i / 4) + 12);
        PTAD::Ui::drawCharacter(PTAD::FONT_DMG_0 + ((enemy->spellResistance >> (i * 2)) & 3), (i % 4) * 2 + 17, (i / 4) + 12);
      }
      PTAD::Ui::drawCharacter(PTAD::FONT_P, 16, 14);
      PTAD::Ui::drawNumber(enemy->statusResistance[0], 17, 14, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_S, 20, 14);
      PTAD::Ui::drawNumber(enemy->statusResistance[1], 21, 14, 100);
      PTAD::Game::player.enemiesScanned[PTAD::battleMonsterID / 32] |= 1 << (PTAD::battleMonsterID % 32);
      if (PTAD::justPressed(PTAD::BTN_MASK_A) || PTAD::justPressed(PTAD::BTN_MASK_B))
      {
        PTAD::Ui::fill(15, 5, 24, 14, 255);
        PTAD::Ui::fillCharacter(PTAD::FONT_FRAME_U, 10, 15, 15);
      }
      else
        --PTAD::globalCounter;
    }
    if (lastPress == PTAD::SKILL_FOCUS_1)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_FOCUS1);
      PTAD::Dialog::beginMessage();
      playerStatus = (playerStatus & ~(3 << STATUS_FOCUSED)) | (1 << STATUS_FOCUSED);
      keepFocus = true;
    }
    else if (lastPress == PTAD::SKILL_FOCUS_2)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_FOCUS2);
      PTAD::Dialog::beginMessage();
      playerStatus = (playerStatus & ~(3 << STATUS_FOCUSED)) | (2 << STATUS_FOCUSED);
      keepFocus = true;
    }
    else if (lastPress == PTAD::SKILL_FOCUS_3)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_FOCUS3);
      PTAD::Dialog::beginMessage();
      playerStatus = (playerStatus & ~(3 << STATUS_FOCUSED)) | (3 << STATUS_FOCUSED);
      keepFocus = true;
    }
    else if (lastPress == PTAD::SKILL_DBL_HIT)
    {
      attack();
      if (enemy->hp == 0)
      {
        PTAD::globalCounter = -1;
        state = State::Victory;
      }
      else
        PTAD::globalCounter = 0;
      lastPress = 16;
    }
    else if (lastPress == PTAD::SKILL_PWR_SHOT)
    {
      attack(2);
      --PTAD::globalCounter;
    }
    else if (lastPress == PTAD::SKILL_DASH)
    {
      uint8_t playerSpeed = (playerStatus >> STATUS_SPEED) & 3;
      if (playerSpeed == SPEED_SLOW)
        playerSpeed = SPEED_NORMAL;
      else if (playerSpeed == SPEED_NORMAL)
        playerSpeed = SPEED_HASTE;
      playerStatus = (playerStatus & ~(3 << STATUS_SPEED)) | (playerSpeed << STATUS_SPEED);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_INFLICT_HASTE);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::SKILL_ENRAGE_1)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_BERSERK1);
      PTAD::Dialog::beginMessage();
      playerStatus = (playerStatus & ~(3 << STATUS_BERSERK)) | (1 << STATUS_BERSERK);
    }
    else if (lastPress == PTAD::SKILL_ENRAGE_2)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_BERSERK2);
      PTAD::Dialog::beginMessage();
      playerStatus = (playerStatus & ~(3 << STATUS_BERSERK)) | (2 << STATUS_BERSERK);
    }
    else if (lastPress == PTAD::SKILL_ENRAGE_3)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_BERSERK3);
      PTAD::Dialog::beginMessage();
      playerStatus = (playerStatus & ~(3 << STATUS_BERSERK)) | (3 << STATUS_BERSERK);
    }
    else if (lastPress == PTAD::SKILL_MEDITATE)
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_REMOVE_BERSERK);
      PTAD::Dialog::beginMessage();
      playerStatus &= ~(3 << STATUS_BERSERK);
    }
    else if (lastPress == PTAD::SKILL_UNKNOWN1)
    {
      //TODO
      PTAD::Dialog::bufferText(txtNoEffect, 24);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::SKILL_UNKNOWN2)
    {
      //TODO
      PTAD::Dialog::bufferText(txtNoEffect, 24);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::SKILL_UNKNOWN3)
    {
      //TODO
      PTAD::Dialog::bufferText(txtNoEffect, 24);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::SKILL_UNKNOWN4)
    {
      //TODO
      PTAD::Dialog::bufferText(txtNoEffect, 24);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == PTAD::SKILL_UNKNOWN5)
    {
      //TODO
      PTAD::Dialog::bufferText(txtNoEffect, 24);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == 16) //DBL_HIT second hit
    {
      attack();
      --PTAD::globalCounter;
    }
  }
  else if (PTAD::globalCounter == 2)
  {
    PTAD::globalCounter = -1;
    if (enemy->hp == 0)
      state = State::Victory;
    else
      endOfPlayersTurn();
  }
  ++PTAD::globalCounter;
}

void PTAD::Battle::castSpell()
{
  if (PTAD::globalCounter == 0)
  {
    if (random(0, 255) < berserkFailRate[(playerStatus >> STATUS_BERSERK) & 3]) //attack instead
    {
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_BERSERK_ATTACK);
      PTAD::Dialog::beginMessage();
      state = State::Attack;
      return;
    }
    PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_CAST_SPELL);
    PTAD::Dialog::bufferText(PTAD::spells[lastPress].name, 8);
    PTAD::Dialog::trimBuffer();
    PTAD::Dialog::beginMessage();
    PTAD::Game::player.mp -= PTAD::spells[lastPress].price;
    //TODO show spells battle animation
  }
  else if (PTAD::globalCounter == 1)
  {
    if (lastPress >= PTAD::SPELL_FIREBALL && lastPress <= PTAD::SPELL_ECLIPSE)
    {
      int damageDealt = magicDamageDealt(getPlayerAttack(), getPlayerMagic(), (lastPress & 1) + 1, getEnemyDefense(), getEnemyMagic(), (enemy->spellResistance >> (lastPress & 14)) & 3);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DAMAGE_DEALT);
      PTAD::Dialog::bufferNumber(damageDealt, 1000);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DAMAGE_TAKEN_END);
      PTAD::Dialog::beginMessage();
      if (damageDealt > enemy->hp)
        enemy->hp = 0;
      else
        enemy->hp -= damageDealt;
    }
    else if (lastPress == SPELL_ENVENOM)
    {
      if (random(0, 0xFFFF) <= 256 * (255 - enemy->statusResistance[0]))
      {
        uint8_t enemyPoisonLevel = (enemyStatus >> STATUS_POISON) & 3;
        ++enemyPoisonLevel;
        PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_INFLICT_POISON);
        enemyStatus = (enemyStatus & ~(3 << STATUS_POISON)) | enemyPoisonLevel;
      }
      else
        PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_INFLICT_POISON_FAIL);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == SPELL_ENSNARE)
    {
      if (random(0, 0xFFFF) <= 256 * (255 - enemy->statusResistance[0]))
      {
        uint8_t enemySlownessLevel = (enemyStatus >> STATUS_SPEED) & 3;
        if (enemySlownessLevel == SPEED_NORMAL)
          enemySlownessLevel = SPEED_SLOW;
        else if (enemySlownessLevel == SPEED_HASTE)
          enemySlownessLevel = SPEED_NORMAL;
        PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_INFLICT_SLOW);
        enemyStatus = (enemyStatus & ~(3 << STATUS_SPEED)) | enemySlownessLevel;
      }
      else
        PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_INFLICT_SLOW_FAIL);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == SPELL_RECOVERY)
    {
      int32_t hpRecovered = std::max(4, getPlayerMagic() * 3);
      if (PTAD::Game::player.hp + hpRecovered > PTAD::Game::player.maxHP)
        hpRecovered = PTAD::Game::player.maxHP - PTAD::Game::player.hp;
      PTAD::Game::player.hp += hpRecovered;
      PTAD::Music::playSFX(PTAD::Music::SFX_HEAL);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_RECOVERED_AMOUNT);
      PTAD::Dialog::bufferNumber(hpRecovered, 1000);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_SPACE);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_H);
      PTAD::Dialog::bufferCharacter(PTAD::FONT_P);
      PTAD::Dialog::beginMessage();
    }
    else if (lastPress == SPELL_CLEANSER)
    {
      playerStatus &= 0xF0;
      PTAD::Music::playSFX(PTAD::Music::SFX_HEAL);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_CLEANSED);
      PTAD::Dialog::beginMessage();
    }
  }
  else if (PTAD::globalCounter == 2)
  {
    PTAD::globalCounter = -1;
    if (enemy->hp == 0)
      state = State::Victory;
    else
      endOfPlayersTurn();
  }
  ++PTAD::globalCounter;
}

void PTAD::Battle::enemyTurnInit()
{
  uint8_t playerSpeed = (playerStatus >> STATUS_SPEED) & 3;
  uint8_t enemySpeed = (enemyStatus >> STATUS_SPEED) & 3;
  PTAD::BattleEvent::begin(enemy->turnOffset);
  state = State::EnemyTurn;
  if (turnCount == 0)
  {
    if (enemySpeed == SPEED_NORMAL && playerSpeed == SPEED_SLOW) //Enemy has SLOW
      turnCount = 2;
    else if (enemySpeed == SPEED_HASTE && playerSpeed == SPEED_NORMAL)
      turnCount = 2;
    else if (enemySpeed == SPEED_HASTE && playerSpeed == SPEED_SLOW)
      turnCount = 3;
    else
      turnCount = 1;
  }
  keepFocus = false;
}

void PTAD::Battle::enemyTurn()
{
  if (PTAD::BattleEvent::inBattleEvent())
    return;
  if (PTAD::Game::player.hp == 0)
    state = State::Defeat;
  else if (enemy->hp == 0)
    state = State::Victory;
  else
  {
    int damageDealt = std::max((enemyStatus >> STATUS_POISON) & 3, PTAD::Game::player.maxHP * ((enemyStatus >> STATUS_POISON) & 3) / 20);
    --turnCount;
    if (turnCount == 0)
      state = State::PlayerTurnInit;
    else
      state = State::EnemyTurnInit;
    if (damageDealt > 0)
    {
      flashBattlerSprite(8, 32, 112, 192);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_POISON_BEGIN);
      PTAD::Dialog::bufferNumber(damageDealt, 100);
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DAMAGE_TAKEN_END);
      PTAD::Dialog::beginMessage();
      if (damageDealt > enemy->hp)
      {
        PTAD::globalCounter = -1;
        enemy->hp = 0;
        state = State::Victory;
      }
      else
        enemy->hp -= damageDealt;
    }
    if (!keepFocus)
      enemyStatus &= ~(3 << STATUS_FOCUSED);
    keepFocus = false;
  }
}

void PTAD::Battle::victory()
{
	if (PTAD::globalCounter == 0)
	{
    PTAD::BattleEvent::begin(enemy->outroOffset);
		PTAD::Music::playMusic(PTAD::battleBGM + 25, 1, false);
	}
	else if (globalCounter == 1)
	{
		PTAD::Game::player.gold += enemy->gold;
    PTAD::Dialog::bufferNumber(enemy->gold, 100);
		PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_GOLD_EARNED);
		PTAD::Dialog::beginMessage();
	}
	else if (globalCounter == 2)
	{
		if (PTAD::Game::player.level < 99)
		{
      PTAD::Game::player.experience += enemy->experience;
      PTAD::Dialog::bufferNumber(enemy->experience, 1000);
			PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_EXPERIENCE_GAINED);
			PTAD::Dialog::beginMessage();
		}
		else
			PTAD::globalCounter = 5;
	}
	else if (globalCounter == 3)
	{
		if (PTAD::Game::player.experience >= PTAD::Game::player.nextLevel)
		{
			PTAD::Game::levelUp();
      PTAD::Music::playMusic(PTAD::Music::MUSIC_LEVEL_UP, 1, false);
			PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_LEVEL_UP);
			PTAD::Dialog::beginMessage();
		}
		else
			PTAD::globalCounter = 5;
	}
	else if (globalCounter == 4)
	{
		if (PTAD::Game::skillLearned[PTAD::Game::player.level - 1] != 255)
    {
      PTAD::Music::playMusic(PTAD::Music::MUSIC_REVIVED, 1, false);
			PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_SKILL_LEARNED);
      PTAD::Dialog::bufferText(PTAD::skills[PTAD::Game::skillLearned[PTAD::Game::player.level - 1]].name, 8);
      PTAD::Dialog::trimBuffer();
      PTAD::Dialog::bufferCharacter(PTAD::FONT_EXCLAMATION);
			PTAD::Dialog::beginMessage();
		}
		else
			PTAD::globalCounter = 5;
	}
	else if (PTAD::justPressed(PTAD::BTN_MASK_A | PTAD::BTN_MASK_B))
	{
    PTAD::globalCounter = -1;
    PTAD::battleMonsterID = 255;
		PTAD::Game::state = PTAD::Game::State::MapInit;
		PTAD::Ui::clear();
	}
	if (PTAD::globalCounter < 5)
		++PTAD::globalCounter;
}

void PTAD::Battle::defeat()
{
	if (PTAD::globalCounter == 0)
	{
		PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DEFEAT);
		PTAD::Dialog::beginMessage();
    PTAD::Music::stopMusic();
		++PTAD::globalCounter;
	}
	else if (PTAD::globalCounter == 1)
	{
		if (PTAD::justPressed(PTAD::BTN_MASK_A | PTAD::BTN_MASK_B) || PB::cBtn())
    {
			++PTAD::globalCounter;
      lastPress = 64;
      PTAD::Music::playMusic(PTAD::Music::MUSIC_GAME_OVER, 1, false);
    }
	}
	else if (PTAD::globalCounter == 2)
	{
    --lastPress;
    if (lastPress > 0)
    {
      PD::loadRGBPalette(miloslav);
      for (int i = 0; i < 256; ++i)
      {
        uint8_t r = (PD::palette[i] >> 11) & 31;
        uint8_t g = (PD::palette[i] >> 5) & 63;
        uint8_t b = PD::palette[i] & 31;
        r = (r * lastPress) / 64;
        g = (g * lastPress) / 64;
        b = (b * lastPress) / 64;
        PD::palette[i] = (r << 11) + (g << 5) + b;
      }
    }
    else
      ++PTAD::globalCounter;
	}
	else if (PTAD::globalCounter == 3)
	{
    PTAD::Game::state = PTAD::Game::State::GameOverInit;
    PTAD::Game::previousState = PTAD::Game::State::TitleInit;
		PTAD::globalCounter = 0;
    PTAD::Ui::fgColor = 7;
    PTAD::Ui::clear();
    if (PTAD::Game::player.variables[0] <= PTAD::Game::GAME_OVER_VALUE1)
      PTAD::GameOver::load(DataPack::hash("screens/gameover1.gfx"));
    else if (PTAD::Game::player.variables[0] <= PTAD::Game::GAME_OVER_VALUE2)
      PTAD::GameOver::load(DataPack::hash("screens/gameover1.gfx"));//TODO replace with screen showing Cecilia trapped in a cage
	}
}

void PTAD::Battle::endOfPlayersTurn()
{
  int damageDealt = std::max((playerStatus >> STATUS_POISON) & 3, PTAD::Game::player.maxHP * ((playerStatus >> STATUS_POISON) & 3) / 20);
  --turnCount;
  if (turnCount == 0)
    state = State::EnemyTurnInit;
  else
    state = State::PlayerTurnInit;
  if (damageDealt > 0 && (playerStatus & (3 << STATUS_POISON)) != 0)
  {
    PTAD::Ui::fgColor = 203;
    shakeScreen = 1;
    shakeRate = 8;
    flashUi = 16;
    PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_POISON_BEGIN);
    PTAD::Dialog::bufferNumber(damageDealt, 100);
    PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_BATTLE_DAMAGE_TAKEN_END);
    PTAD::Dialog::beginMessage();
    if (damageDealt >= PTAD::Game::player.hp)
    {
      PTAD::globalCounter = -1;
      PTAD::Game::player.hp = 0;
      state = State::Defeat;
    }
    else
      PTAD::Game::player.hp -= damageDealt;
  }
  if (!keepFocus)
    playerStatus &= ~(3 << STATUS_FOCUSED);
  keepFocus = false;
}

//The purpose of these functions is to allow arbitrary things to be added that modify the base stats including temporary modifications

uint16_t PTAD::Battle::getPlayerAttack()
{
  uint16_t value = playerStats.attack * focusMultiplier[(playerStatus >> STATUS_FOCUSED) & 3];
  for (int i = 0; i < 6; ++i)
  {
    if (PTAD::Game::player.equippedItems[i] != 255)
      value += PTAD::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[0];
  }
  return value * (((playerStatus >> STATUS_BERSERK) & 3) + 1);
}

uint16_t PTAD::Battle::getPlayerDefense()
{
  uint16_t value = playerStats.defense * focusMultiplier[(playerStatus >> STATUS_FOCUSED) & 3];
  for (int i = 0; i < 6; ++i)
  {
    if (PTAD::Game::player.equippedItems[i] != 255)
      value += PTAD::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[1];
  }
  return value;
}

uint16_t PTAD::Battle::getPlayerAgility()
{
  uint16_t value = playerStats.agility * focusMultiplier[(playerStatus >> STATUS_FOCUSED) & 3];
  for (int i = 0; i < 6; ++i)
  {
    if (PTAD::Game::player.equippedItems[i] != 255)
      value += PTAD::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[2];
  }
  return value;
}

uint16_t PTAD::Battle::getPlayerMagic()
{
  uint16_t value = playerStats.magic * focusMultiplier[(playerStatus >> STATUS_FOCUSED) & 3];
  for (int i = 0; i < 6; ++i)
  {
    if (PTAD::Game::player.equippedItems[i] != 255)
      value += PTAD::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[3];
  }
  return value;
}

uint16_t PTAD::Battle::getEnemyAttack()
{
  uint16_t value = enemy->stats.attack * focusMultiplier[(enemyStatus >> STATUS_FOCUSED) & 3] * (((enemyStatus >> STATUS_BERSERK) & 3) + 1);
  return value;
}

uint16_t PTAD::Battle::getEnemyDefense()
{
  uint16_t value = enemy->stats.defense * focusMultiplier[(enemyStatus >> STATUS_FOCUSED) & 3];
  return value;
}

uint16_t PTAD::Battle::getEnemyAgility()
{
  uint16_t value = enemy->stats.agility * focusMultiplier[(enemyStatus >> STATUS_FOCUSED) & 3];
  return value;
}

uint16_t PTAD::Battle::getEnemyMagic()
{
  uint16_t value = enemy->stats.magic * focusMultiplier[(enemyStatus >> STATUS_FOCUSED) & 3];
  return value;
}
