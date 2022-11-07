#include <cmath>
#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

//#define SHOW_FPS

#define START_LEVEL 1

/*const uint8_t PTAD::Game::skillLearned[99] =
{
  255,SKILL_EXAMINE,255,SKILL_FOCUS_1,255,255,SKILL_MEDITATE,255,255,
  255,255,SKILL_DBL_HIT,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255
};*/

const uint8_t PTAD::Game::messageSpeeds[]
{
  //Instant
  0x22,0x41,0x46,0x47,0x34,0x41,0x47,0x0B,
  //Fastest
  0x1F,0x34,0x46,0x47,0x38,0x46,0x47,0x0B,
  //Fast
  0x1F,0x34,0x46,0x47,0x0A,0x0A,0x0A,0x0B,
  //Normal
  0x27,0x42,0x45,0x40,0x34,0x3F,0x0A,0x0B,
  //Slow
  0x2C,0x3F,0x42,0x4A,0x0A,0x0A,0x0A,0x0B,
  //Slowest
  0x2C,0x3F,0x42,0x4A,0x38,0x46,0x47,0x0B,
  //Preview
  0x29,0x45,0x38,0x49,0x3C,0x38,0x4A,0x07
};

const PTAD::Game::PlayerData PTAD::Game::newGame =
{
  //variables
  {0, 0, 0, 0},
  //switches
  {0x00000000, 0x00000000},
  //Skills
  0x0000,
  //Spells
  0x0000,
  //name
  //{0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A},
  //{PTAD::FONT_A, PTAD::FONT_d, PTAD::FONT_r, PTAD::FONT_i, PTAD::FONT_a, PTAD::FONT_n, PTAD::FONT_SPACE, PTAD::FONT_SPACE},
  {PTAD::Resources::defaultName[0], PTAD::Resources::defaultName[1], PTAD::Resources::defaultName[2], PTAD::Resources::defaultName[3], PTAD::Resources::defaultName[4], PTAD::Resources::defaultName[5], PTAD::Resources::defaultName[6], PTAD::Resources::defaultName[7]},
  //x, y
  //192, 128,
  PTAD::Resources::startX * 16, PTAD::Resources::startY * 16,
  //hp, maxHP
  (uint16_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::hpGrowth), (uint16_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::hpGrowth),
  //mp, maxMP
  (uint8_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::mpGrowth), (uint8_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::mpGrowth),
  //experience, nextLevel
  0, PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::experienceGrowth),
  //gold
  0,
  //level
  START_LEVEL,
  //attack, defense, agility, magic
  {
    (uint16_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::attackGrowth),
    (uint16_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::defenseGrowth),
    (uint16_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::agilityGrowth),
    (uint16_t)PTAD::Resources::getStatForLevel(START_LEVEL, PTAD::Resources::magicGrowth)
  },
  //mapID
  //DataPack::hash("/maps/map0003.xml"),
  PTAD::Resources::startMap,
  //messageSpeed
  0x11,
  //dir
  //PTAD::DIR_DOWN,
  PTAD::Resources::startFacing,
  //equpped items
  {255, 255, 255, 255, 255, 255},
  //equippableItems
  {0, 0, 0, 0, 0, 0},
  //items
  {
    //consumableItems
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //tradeableItems
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  //Enemies scanned
  {0, 0, 0, 0, 0, 0, 0, 0},
  //playTime
  0
};

PTAD::Game::PlayerData PTAD::Game::player;
PTAD::Game::State PTAD::Game::state = PTAD::Game::State::Map;
PTAD::Game::State PTAD::Game::previousState = PTAD::Game::state;
uint32_t PTAD::Game::previousFrame = 0;
uint8_t PTAD::Game::volume = 0;

void PTAD::Game::setup()
{
	PTAD::dataFile = new DataPack;
	if (!PTAD::dataFile->open(PATH_PREFIX"/data/preludetoadream.pak"))
	{
		LOG("Failed to open /data/preludetoadream.pak\n");
		LOG("Error: ");
		LOG(dataFile->getError());
		LOG("\n");
	}
	LOG("Save Size: ", sizeof(PlayerData), " bytes\n");
  LOG("Save Data: ", sizeof(SaveData), " bytes\n");
  LOG("Memory needed: ", PTAD::MEMORY_SIZE, " bytes\n");
  LOG("Title:         ", PTAD::MEMORY_TITLE_SIZE, " bytes\n");
  LOG("Tileset:       ", PTAD::MEMORY_TILESET_SIZE, " bytes\n");
  LOG("Battle System: ", PTAD::MEMORY_BATTLE_SIZE, " bytes\n");
  LOG("Map System:    ", PTAD::MEMORY_MAP_SIZE, " bytes\n");
  LOG("MapData:       ", sizeof(PTAD::Map::MapData), " bytes\n");
  LOG("EnemyData:     ", sizeof(PTAD::Battle::EnemyData), " bytes\n");
  memcpy((void*)&player, (void*)&newGame, sizeof(PlayerData));
	PTAD::Dialog::setup();
  //PTAD::Menu::setup(PTAD::Menu::State::NameEntryMenu);
  PD::shiftTilemap(2, 0);
  memset(PTAD::tilemapFG, 255, PTAD::TILEMAP_MEMORY_SIZE);
  state = State::TitleInit;
  volume = std::min((uint8_t)6, Pokitto::discrete_vol);
  PTAD::Music::playMusic(PTAD::Resources::music_mainTheme, 0);
  previousFrame = PC::getTime();
}

void PTAD::Game::update()
{
  uint32_t currentTime = PC::getTime();
#ifdef SHOW_FPS
  ++PTAD::frameCount;
  if (currentTime - PTAD::fpsStart >= 1000)
  {
    PTAD::fps = PTAD::frameCount * 1000 / (currentTime - PTAD::fpsStart);
    PTAD::frameCount = 0;
    PTAD::fpsStart = currentTime;
  }
#endif
  if ((PTAD::Ui::dialogCursor & PTAD::Ui::DIALOG_CURSOR_BLINK) != 0)
    PTAD::Ui::dialogCursor = PTAD::Ui::DIALOG_CURSOR_BLINK | ((currentTime / 250) % 2);
  PTAD::Game::player.playTime += currentTime - previousFrame;
	switch (state)
	{
    case State::TitleInit:
      previousState = state;
      state = State::Title;
      PTAD::globalCounter = 0;
      PD::lineFillers[0] = &PTAD::Title::lineFiller;
      PD::lineFillers[1] = &PTAD::Ui::lineFiller;
      PD::lineFillers[2] = &TAS::NOPFiller;
      PD::lineFillers[3] = &TAS::NOPFiller;
      PTAD::Title::setup();
			[[fallthrough]];
		case State::Title:
      PTAD::Title::update();
			break;
    case State::MapInit:
      state = State::Map;
      PTAD::Dialog::setMessageSpeed(player.messageSpeed & 0xF);
			PTAD::Map::setup();
      PD::lineFillers[0] = &PTAD::tileFillerBG;
      PD::lineFillers[1] = &TAS::SpriteFiller;
      PD::lineFillers[2] = &PTAD::tileFillerFG;
      PD::lineFillers[3] = &PTAD::Ui::lineFiller;
      previousState = state;
			[[fallthrough]];
		case State::Map:
			PTAD::Map::update();
			break;
    case State::BattleInit:
      state = State::Battle;
      PTAD::Dialog::setMessageSpeed((player.messageSpeed >> 4) & 0xF);
			PTAD::Battle::setup();
      PD::lineFillers[0] = &PTAD::tileFillerBG;
      PD::lineFillers[1] = &PTAD::tileFillerFG;
      PD::lineFillers[2] = &PTAD::Ui::lineFiller;
      PD::lineFillers[3] = &TAS::SpriteFiller;
      previousState = state;
			[[fallthrough]];
    case State::Battle:
			PTAD::Battle::update();
			break;
    case State::MenuInit:
      state = State::Menu;
      PD::lineFillers[0] = &PTAD::tileFillerBG;
      PD::lineFillers[1] = &PTAD::tileFillerFG;
      PD::lineFillers[2] = &PTAD::Ui::lineFiller;
      PD::lineFillers[3] = &TAS::SpriteFiller;
      [[fallthrough]];
    case State::Menu:
      PTAD::Menu::update();
      break;
		case State::GameOverInit:
      state = State::GameOver;
      previousState = state;
      PD::lineFillers[0] = &TAS::NOPFiller;
      PD::lineFillers[1] = &TAS::NOPFiller;
      PD::lineFillers[2] = &TAS::NOPFiller;
      PD::lineFillers[3] = &TAS::NOPFiller;
      PTAD::GameOver::setup();
			[[fallthrough]];
		case State::GameOver:
      PTAD::GameOver::update();
			break;
  }
  previousFrame = PC::getTime();
#ifdef SHOW_FPS
	PTAD::Ui::drawNumber(PTAD::fps, 1, 1, 100);
#endif
}

//constexpr uint32_t PTAD::Game::getStatForLevel(uint8_t level, const PTAD::Resources::StatGrowth &growth)
//{
//	return growth.start + (uint32_t)(growth.base * pow(level, growth.exponent));
//}

void PTAD::Game::levelUp()
{
	++player.level;
	player.maxHP = PTAD::Resources::getStatForLevel(player.level, PTAD::Resources::hpGrowth);
	player.hp = player.maxHP;
  player.maxMP = PTAD::Resources::getStatForLevel(player.level, PTAD::Resources::mpGrowth);
	player.mp = player.maxMP;
  player.nextLevel += PTAD::Resources::getStatForLevel(player.level, PTAD::Resources::experienceGrowth);
  player.stats.attack = PTAD::Resources::getStatForLevel(player.level, PTAD::Resources::attackGrowth);
  player.stats.defense = PTAD::Resources::getStatForLevel(player.level, PTAD::Resources::defenseGrowth);
  player.stats.agility = PTAD::Resources::getStatForLevel(player.level, PTAD::Resources::agilityGrowth);
  player.stats.magic = PTAD::Resources::getStatForLevel(player.level, PTAD::Resources::magicGrowth);
  if (PTAD::Resources::skillLearned[player.level - 1] != 255)
    player.skills |= 1 << PTAD::Resources::skillLearned[player.level - 1];
}

uint16_t PTAD::Game::calculateSpellResistance()
{
  uint16_t total = 0;
  for (int i = 0; i < 7; ++i)
  {
    uint8_t resistance = 0;
    for (int j = 0; j < 6; ++j)
    {
      //if player has equipment type 'j' equipped then add that equipment's resistance to spell 'i'
      if (player.equippedItems[j] != 255)
        resistance += (PTAD::Resources::equipmentStats[j][player.equippedItems[j]].spellResistance >> (i * 2)) & 3;
      else
        resistance += 2;
    }
    //Cap resistance between -2 and 1
    if (resistance < 10)
      resistance = 0;
    else if (resistance > 13)
      resistance = 3;
    else
      resistance -= 10;
    total |= resistance << (i * 2);
  }
  return total;
}

uint8_t PTAD::Game::getPoisonResistance()
{
  int16_t total = 0;
  for (int i = 0; i < 6; ++i)
  {
    if (player.equippedItems[i] != 255)
      total += PTAD::Resources::equipmentStats[i][player.equippedItems[i]].statusResistance[0];
  }
  if (total < 0)
    total = 0;
  if (total > 255)
    total = 255;
  return (uint8_t)total;
}

uint8_t PTAD::Game::getSlowResistance()
{
  int16_t total = 0;
  for (int i = 0; i < 6; ++i)
  {
    if (player.equippedItems[i] != 255)
      total += PTAD::Resources::equipmentStats[i][player.equippedItems[i]].statusResistance[1];
  }
  if (total < 0)
    total = 0;
  if (total > 255)
    total = 255;
  return (uint8_t)total;
}

void PTAD::Game::loadGame(uint8_t slot)
{
  File saveFile;
  SaveData data;
  saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav");
  if (saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav"))
  {
    saveFile.read((void*)&data, sizeof(SaveData));
    memcpy((void*)&player, (void*)&(data.slots[slot]), sizeof(PlayerData));
  }
  else
  {
    LOG("Failed to load save file: ", saveFile.error, "\n");
    memcpy((void*)&player, (void*)&newGame, sizeof(PlayerData));
  }
  saveFile.close();
}

void PTAD::Game::saveGame(uint8_t slot)
{
  File saveFile;
  SaveData data;
  saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav");
  if (saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav"))
    saveFile.read((void*)&data, sizeof(SaveData));
  else
    memset((void*)&data, 0, sizeof(SaveData));
  saveFile.close();
  memcpy((void*)&(data.slots[slot]), (void*)&player, sizeof(PlayerData));
  saveFile.openRW(PATH_PREFIX"/data/preludetoadream.sav", true, false);
  if (saveFile.openRW(PATH_PREFIX"/data/preludetoadream.sav", true, false))
    saveFile.write((void*)&data, sizeof(SaveData));
  saveFile.close();
}

void PTAD::Game::eraseGame(uint8_t slot)
{
  File saveFile;
  SaveData data;
  saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav");
  if (saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav"))
    saveFile.read((void*)&data, sizeof(SaveData));
  else
    memset((void*)&data, 0, sizeof(SaveData));
  saveFile.close();
  memset((void*)&(data.slots[slot]), 0, sizeof(PlayerData));
  saveFile.openRW(PATH_PREFIX"/data/preludetoadream.sav", true, false);
  if (saveFile.openRW(PATH_PREFIX"/data/preludetoadream.sav", true, false))
    saveFile.write((void*)&data, sizeof(SaveData));
  saveFile.close();
}

PTAD::Game::SaveData PTAD::Game::getSaveData()
{
  File saveFile;
  SaveData data;
  saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav");
  if (saveFile.openRO(PATH_PREFIX"/data/preludetoadream.sav"))
    saveFile.read((void*)&data, sizeof(SaveData));
  else
    memset((void*)&data, 0, sizeof(SaveData));
  saveFile.close();
  return data;
}
