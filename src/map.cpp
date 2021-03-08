#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

DataPack::PackedFile PTAD::Map::mapFile;
PTAD::Map::State PTAD::Map::state;
PTAD::Map::MenuState PTAD::Map::menuState = PTAD::Map::MenuState::MainMenu;
int8_t PTAD::Map::chunkID[4] = {-1, -1, -1, -1};
int8_t PTAD::Map::chunkX[4] = {0, 0, 0, 0};
int8_t PTAD::Map::chunkY[4] = {0, 0, 0, 0};
int32_t PTAD::Map::screenX;
int32_t PTAD::Map::screenY;
uint32_t PTAD::Map::lastPress;
int8_t PTAD::Map::shakeOffsetX = 0;
int8_t PTAD::Map::shakeOffsetY = 0;
uint8_t PTAD::Map::shakeScreen = 0;
uint8_t PTAD::Map::shakeRate = 8;
uint8_t PTAD::Map::playerFrame = 1;
uint8_t PTAD::Map::eventID = 255;
uint8_t PTAD::Map::shopMessage[24];
uint16_t PTAD::Map::shopItems;
uint16_t PTAD::Map::shopEquipment[6];
uint8_t PTAD::Map::itemID = 0;
uint8_t PTAD::Map::steps = 0;
PTAD::Map::MapData *PTAD::Map::data = (PTAD::Map::MapData*)(PTAD::memory + PTAD::MEMORY_MAP_DATA_OFFSET);
uint8_t *PTAD::Map::passability = PTAD::memory + PTAD::MEMORY_MAP_PASSABILITY_OFFSET;
uint8_t *PTAD::Map::chunksBG = PTAD::memory + PTAD::MEMORY_MAP_CHUNKSBG_OFFSET;
uint8_t *PTAD::Map::chunksFG = PTAD::memory + PTAD::MEMORY_MAP_CHUNKSFG_OFFSET;
uint8_t *PTAD::Map::sprites = PTAD::memory + PTAD::MEMORY_MAP_SPRITES_OFFSET;
uint8_t *PTAD::Map::tiledata = PTAD::memory + PTAD::MEMORY_MAP_TILEDATA_OFFSET;
const uint8_t PTAD::Map::playerFrames[] = {0, 1, 2, 1};
const uint8_t PTAD::Map::movement[12] = {1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2};

const uint32_t PTAD::Map::tiledataFiles[] =
{
  DataPack::hash("tilesets/overworld.dat"),
  DataPack::hash("tilesets/town.dat"),
  DataPack::hash("tilesets/interrior.dat"),
  DataPack::hash("tilesets/cavern2.dat")
};

const uint8_t PTAD::Map::txtOptions[] = 
{
  //Stats_
  0x2C,0x47,0x34,0x47,0x46,0x0A,
  //Equip_
  0x1E,0x44,0x48,0x3C,0x43,0x0A,
  //Items_
  0x22,0x47,0x38,0x40,0x46,0x0A,
  //Skills
  0x2C,0x3E,0x3C,0x3F,0x3F,0x46,
  //Spells
  0x2C,0x43,0x38,0x3F,0x3F,0x46,
  //Dialog
  0x1D,0x3C,0x34,0x3F,0x42,0x3A,
  //Battle
  0x1B,0x34,0x47,0x47,0x3F,0x38,
  //Volume
  0x2F,0x42,0x3F,0x48,0x40,0x38,
  //Beasts
  0x1B,0x38,0x34,0x46,0x47,0x46,
  //Save__
  0x2C,0x34,0x49,0x38,0x0A,0x0A
};

const uint8_t PTAD::Map::statsMenu[]
{
  //______________Level_____
  //HP______/_____MP____/___
  //Exp___________Gold______
  //------------------------
  //______ATK__DEF__AGL__MAG
  //Base____________________
  //------------------------
  //Wpn_____________________
  //Helm____________________
  //Armr____________________
  //Boot____________________
  //Ring____________________
  //Amlt____________________
  //------------------------
  //Total___________________
  //------------------------
  //F%W%E%A%______Poison____
  //L%D%S%______Slowness____
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x25,0x38,0x49,0x38,0x3F,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x21,0x29,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x03,0x0A,0x0A,0x0A,0x0A,0x0A,0x26,0x29,0x0A,0x0A,0x0A,0x0A,0x03,0x0A,0x0A,0x0A,
  0x1E,0x4B,0x43,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x20,0x42,0x3F,0x37,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x1A,0x2D,0x24,0x0A,0x0A,0x1D,0x1E,0x1F,0x0A,0x0A,0x1A,0x20,0x25,0x0A,0x0A,0x26,0x1A,0x20,
  0x1B,0x34,0x46,0x38,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
  0x30,0x43,0x41,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x21,0x38,0x3F,0x40,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x1A,0x45,0x40,0x45,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x1B,0x42,0x42,0x47,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x2B,0x3C,0x41,0x3A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x1A,0x40,0x3F,0x47,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
  0x2D,0x42,0x47,0x34,0x3F,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
  0x50,0x0A,0x51,0x0A,0x52,0x0A,0x53,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x29,0x42,0x3C,0x46,0x42,0x41,0x0A,0x0A,0x0A,0x0A,
  0x54,0x0A,0x55,0x0A,0x56,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x2C,0x3F,0x42,0x4A,0x41,0x38,0x46,0x46,0x0A,0x0A,0x0A,0x0A
};

const uint8_t PTAD::Map::infoText[] =
{
  //You_have_no_items_______
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x3C,0x47,0x38,0x40,0x46,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  //You_can't_use_that_here_
  0x32,0x42,0x48,0x0A,0x36,0x34,0x41,0x04,0x47,0x0A,0x48,0x46,0x38,0x0A,0x47,0x3B,0x34,0x47,0x0A,0x3B,0x38,0x45,0x38,0x0A,
  //You_don't_need_this_now_
  0x32,0x42,0x48,0x0A,0x37,0x42,0x41,0x04,0x47,0x0A,0x41,0x38,0x38,0x37,0x0A,0x47,0x3B,0x3C,0x46,0x0A,0x41,0x42,0x4A,0x0A,
  //You_have_no_skills______
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x46,0x3E,0x3C,0x3F,0x3F,0x46,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  //You_have_no_spells______
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x46,0x43,0x38,0x3F,0x3F,0x46,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  //You don't have any._____
  0x32,0x42,0x48,0x0A,0x37,0x42,0x41,0x04,0x47,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x34,0x41,0x4C,0x02,0x0A,0x0A,0x0A,0x0A,0x0A,
  //You don't have that many
  0x32,0x42,0x48,0x0A,0x37,0x42,0x41,0x04,0x47,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x47,0x3B,0x34,0x47,0x0A,0x40,0x34,0x41,0x4C
};

const uint8_t PTAD::Map::hpRecovered[8] =
{
  //Potion
  30,
  //H-Potion
  100,
  //S-Potion
  200,
  //Ether
  0,
  //H-Ether
  0,
  //S-Ether
  0,
  //Elixir
  100
};

const uint8_t PTAD::Map::mpRecovered[8] =
{
  //Potion
  0,
  //H-Potion
  0,
  //S-Potion
  0,
  //Ether
  3,
  //H-Ether
  10,
  //S-Ether
  20,
  //Elixir
  10
};

const uint8_t PTAD::Map::shopSoldOut[] = {PTAD::FONT_MINUS, PTAD::FONT_MINUS, PTAD::FONT_S, PTAD::FONT_o, PTAD::FONT_l, PTAD::FONT_d, PTAD::FONT_SPACE, PTAD::FONT_O, PTAD::FONT_u, PTAD::FONT_t, PTAD::FONT_MINUS, PTAD::FONT_MINUS};
const uint8_t PTAD::Map::shopNoneAvailable[] = {0x22,0x0A,0x37,0x42,0x41,0x04,0x47,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x34,0x41,0x4C,0x0A,0x47,0x42,0x0A,0x46,0x38,0x3F,0x3F};
const uint8_t PTAD::Map::shopNotEnoughMoney[] = {0x32,0x42,0x48,0x0A,0x36,0x34,0x41,0x04,0x47,0x0A,0x34,0x39,0x39,0x42,0x45,0x37,0x0A,0x47,0x3B,0x34,0x47,0x02,0x0A,0x0A};
const uint8_t PTAD::Map::shopHowMany[] = {0x21,0x42,0x4A,0x0A,0x40,0x34,0x41,0x4C,0x0A,0x4A,0x42,0x48,0x3F,0x37,0x0A,0x4C,0x42,0x48,0x0A,0x3F,0x3C,0x3E,0x38,0x05};
const uint8_t PTAD::Map::shopNotEnoughRoom[] = {0x32,0x42,0x48,0x0A,0x36,0x34,0x41,0x04,0x47,0x0A,0x36,0x34,0x45,0x45,0x4C,0x0A,0x34,0x41,0x4C,0x40,0x42,0x45,0x38,0x02};
const uint8_t PTAD::Map::shopSoldOutMessage[] = {0x1A,0x39,0x45,0x34,0x3C,0x37,0x0A,0x47,0x3B,0x34,0x47,0x04,0x46,0x0A,0x46,0x42,0x3F,0x37,0x0A,0x42,0x48,0x47,0x02,0x0A};
const uint8_t PTAD::Map::shopSellQtyMessage[] = {0x21,0x42,0x4A,0x0A,0x40,0x34,0x41,0x4C,0x0A,0x34,0x45,0x38,0x0A,0x4C,0x42,0x48,0x0A,0x46,0x38,0x3F,0x3F,0x3C,0x41,0x3A};

const uint8_t PTAD::Map::shopBuyOptions[7][7] =
{
  //Items__
  0x22,0x47,0x38,0x40,0x46,0x0A,0x0A,
  //Weapons
  0x30,0x38,0x34,0x43,0x42,0x41,0x46,
  //Helmets
  0x21,0x38,0x3F,0x40,0x38,0x47,0x46,
  //Armor__
  0x1A,0x45,0x40,0x42,0x45,0x0A,0x0A,
  //Boots__
  0x1B,0x42,0x42,0x47,0x46,0x0A,0x0A,
  //Rings__
  0x2B,0x3C,0x41,0x3A,0x46,0x0A,0x0A,
  //Amulets
  0x1A,0x40,0x48,0x3F,0x38,0x47,0x46
};

extern const uint8_t *tilemap[];

void PTAD::Map::setup()
{
  DataPack::PackedFile file;
  PTAD::dataFile->getPackedFile(PTAD::Game::player.mapHash, &mapFile);
  PTAD::dataFile->readBytes(&mapFile, (void*)data, sizeof(MapData));
  PTAD::dataFile->readBytes(&mapFile, passability, PTAD::MEMORY_MAP_PASSABILITY_SIZE);
  PTAD::dataFile->getPackedFile(tiledataFiles[data->tilesetID], &file);
  PTAD::dataFile->readBytes(&file, tiledata, PTAD::MEMORY_MAP_TILEDATA_SIZE);
  PTAD::loadTileset(data->tilesetID);
  loadSprites();
  screenX = PTAD::Game::player.x - 102;
  screenY = PTAD::Game::player.y - 80;
  PD::shiftTilemap((screenX + 128) % 8, (screenY + 128) % 8);
  for (int i = 0; i < 4; ++i)
    chunkID[i] = -1;
  updateChunks();
  PTAD::Music::playMusic(data->bgmID);
  state = State::Idle;
  PTAD::MapEvent::runOnLoadEvent(PTAD::Game::player.mapHash, sizeof(MapData) + PTAD::MEMORY_MAP_PASSABILITY_SIZE + data->width * data->height * (PTAD::MEMORY_MAP_CHUNKS_SIZE / 4) * 2);
  steps = 0;
}

void PTAD::Map::update()
{
  if (state == State::Idle)
  {
    if (PTAD::Dialog::updateMessage())
    {
      //Do nothing
    }
    else if (eventID != 255)
    {
      if (PTAD::MapEvent::update())
        eventID = 255;
    }
    else if (PB::upBtn())
    {
      if (canMove(PTAD::Game::player.x / 16, PTAD::Game::player.y / 16 - 1))
      {
        eventID = eventAtLocation(PTAD::Game::player.x / 16, PTAD::Game::player.y / 16 - 1);
        if (eventID != 255 && (data->events[eventID].flags & PTAD::MapEvent::FLAGS_DOOR) != 0)
          ++data->events[eventID].spriteID;
        eventID = 255;
        state = State::MoveUp;
      }
      PTAD::Game::player.dir = PTAD::DIR_UP;
    }
    else if (PB::downBtn())
    {
      if (canMove(PTAD::Game::player.x / 16, PTAD::Game::player.y / 16 + 1))
      {
        eventID = eventAtLocation(PTAD::Game::player.x / 16, PTAD::Game::player.y / 16 + 1);
        if (eventID != 255 && (data->events[eventID].flags & PTAD::MapEvent::FLAGS_DOOR) != 0)
          ++data->events[eventID].spriteID;
        eventID = 255;
        state = State::MoveDown;
      }
      PTAD::Game::player.dir = PTAD::DIR_DOWN;
    }
    else if (PB::leftBtn())
    {
      if (canMove(PTAD::Game::player.x / 16 - 1, PTAD::Game::player.y / 16))
      {
        eventID = eventAtLocation(PTAD::Game::player.x / 16 - 1, PTAD::Game::player.y / 16);
        if (eventID != 255 && (data->events[eventID].flags & PTAD::MapEvent::FLAGS_DOOR) != 0)
          ++data->events[eventID].spriteID;
        eventID = 255;
        state = State::MoveLeft;
      }
      PTAD::Game::player.dir = PTAD::DIR_LEFT;
    }
    else if (PB::rightBtn())
    {
      if (canMove(PTAD::Game::player.x / 16 + 1, PTAD::Game::player.y / 16))
      {
        eventID = eventAtLocation(PTAD::Game::player.x / 16 + 1, PTAD::Game::player.y / 16);
        if (eventID != 255 && (data->events[eventID].flags & PTAD::MapEvent::FLAGS_DOOR) != 0)
          ++data->events[eventID].spriteID;
        eventID = 255;
        state = State::MoveRight;
      }
      PTAD::Game::player.dir = PTAD::DIR_RIGHT;
    }
    else if (PTAD::justPressed(PTAD::BTN_MASK_A))
    {
      uint8_t x = PTAD::Game::player.x / 16;
      uint8_t y = PTAD::Game::player.y / 16;
      if (PTAD::Game::player.dir == PTAD::DIR_UP)
        eventID = eventAtLocation(x, y - 1);
      else if (PTAD::Game::player.dir == PTAD::DIR_DOWN)
        eventID = eventAtLocation(x, y + 1);
      else if (PTAD::Game::player.dir == PTAD::DIR_LEFT)
        eventID = eventAtLocation(x - 1, y);
      else if (PTAD::Game::player.dir == PTAD::DIR_RIGHT)
        eventID = eventAtLocation(x + 1, y);
      if (eventID != 255 && (data->events[eventID].flags & PTAD::MapEvent::FLAGS_ACTIVATE_BUTTON) == 0)
        eventID = 255;
      else
        PTAD::MapEvent::begin(data->events[eventID].offset);
    }
    else if (PTAD::justPressed(PTAD::BTN_MASK_C))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      drawMainMenu();
      state = State::Menu;
      menuState = MenuState::MainMenu;
      PTAD::globalCounter = 0;
      lastPress = 0;
    }
    else
      playerFrame = 1;
  }
  else if (state == State::Menu)
  {
    PTAD::Dialog::updateMessage(false);
    switch (menuState)
    {
      case MenuState::MainMenu:
        updateMainMenu();
        break;
      case MenuState::StatsMenu:
        updateStatsMenu();
        break;
      case MenuState::EquipMenu:
        updateEquipMenu();
        break;
      case MenuState::ItemsMenu:
        updateItemsMenu();
        break;
      case MenuState::SkillsMenu:
        updateSkillsMenu();
        break;
      case MenuState::SpellsMenu:
        updateSpellsMenu();
        break;
      case MenuState::DialogMenu:
        updateDialogMenu();
        break;
      case MenuState::BattleMenu:
        updateBattleMenu();
        break;
      case MenuState::VolumeMenu:
        updateVolumeMenu();
        break;
      case MenuState::BeastsMenu:
        updateBeastsMenu();
        break;
      case MenuState::SaveMenu:
        updateSaveMenu();
        break;
      case MenuState::ShopMenu:
        updateShopMenu();
				break;
      case MenuState::ShopMenu_BuyWhat:
        updateShopMenu_BuyWhat();
				break;
      case MenuState::ShopMenu_BuyItems:
        updateShopMenu_BuyItems();
				break;
      case MenuState::ShopMenu_BuyEquipment:
        updateShopMenu_BuyEquipment();
				break;
      case MenuState::ShopMenu_BuyQty:
        updateShopMenu_BuyQty();
				break;
      case MenuState::ShopMenu_SellWhat:
        updateShopMenu_SellWhat();
				break;
      case MenuState::ShopMenu_SellQty:
        updateShopMenu_SellQty();
				break;
    }
  }
  if (shakeScreen > 0 && PC::frameCount % shakeRate == 0)
  {
    if (shakeScreen != 255)
      --shakeScreen;
    shakeOffsetX = ((int8_t)random(0, 8)) - 4;
    shakeOffsetY = ((int8_t)random(0, 8)) - 4;
  }
  else if (shakeScreen == 0)
  {
    shakeOffsetX = 0;
    shakeOffsetY = 0;
  }
  if (state == State::MoveUp)
    PTAD::Game::player.y -= movement[PTAD::globalCounter];
  else if (state == State::MoveDown)
    PTAD::Game::player.y += movement[PTAD::globalCounter];
  else if (state == State::MoveLeft)
    PTAD::Game::player.x -= movement[PTAD::globalCounter];
  else if (state == State::MoveRight)
    PTAD::Game::player.x += movement[PTAD::globalCounter];
  /*if (state == State::MoveUp)
    --PTAD::Game::player.y;
  else if (state == State::MoveDown)
    ++PTAD::Game::player.y;
  else if (state == State::MoveLeft)
    --PTAD::Game::player.x;
  else if (state == State::MoveRight)
    ++PTAD::Game::player.x;*/
  if (PTAD::Game::state != PTAD::Game::State::MapInit)
  {
    screenX = PTAD::Game::player.x - 102 + shakeOffsetX;
    screenY = PTAD::Game::player.y - 80 + shakeOffsetY;
  }
  PD::shiftTilemap((screenX + 128) % 8, (screenY + 128) % 8);
  if (state != State::Idle && state != State::Menu)
  {
    ++PTAD::globalCounter;
    //if (PTAD::globalCounter % 8 == 1)
    if (PTAD::globalCounter % 6 == 1)
      playerFrame = (playerFrame + 1) % 4;
    updateChunks();
    //if (PTAD::globalCounter == 16)
    if (PTAD::globalCounter == 12)
    {
      ++steps;
      state = State::Idle;
      PTAD::globalCounter = 0;
      if ((eventID = eventAtLocation(PTAD::Game::player.x / 16, PTAD::Game::player.y / 16)) == 255)
        randomBattle();
      else if ((data->events[eventID].flags & PTAD::MapEvent::FLAGS_ACTIVATE_BUTTON) != 0)
        eventID = 255;
      else
        PTAD::MapEvent::begin(data->events[eventID].offset);
    }
  }
  for (int i = 0; i < 29; ++i)
  {
    if (data->events[i].spriteID < 16)
      PD::drawSpriteBitmap(data->events[i].x * 16 - screenX, data->events[i].y * 16 - screenY - (data->events[i].flags & PTAD::MapEvent::FLAGS_OFFSET), 16, 24, sprites + data->events[i].spriteID * 384);
  }
  PD::drawSpriteBitmap(102, 72, 16, 24, playerSprite + PTAD::Game::player.dir * 1152 + playerFrames[playerFrame] * 384);
}

void PTAD::Map::loadSprites()
{
  DataPack::PackedFile file;
  PTAD::dataFile->getPackedFile(DataPack::hash("sprites/sprites.gfx"), &file);
  for (int i = 0; i < 16; ++i)
  {
    if (data->sprites[i] != 255)
    {
      file.seek(data->sprites[i] * 384);
      PTAD::dataFile->readBytes(&file, sprites + i * 384, 384);
    }
  }
}

//1 2 3 4 5 6 7 8 9 A B C D E F
//2 2 2 2 2 2 2 2 3 3 3 3 3 3 3

void PTAD::Map::drawChunk(const uint8_t *chunk, int x, int y, bool bg)
{
  for (int cy = 0; cy < 32; ++cy)
    PTAD::drawTiles(chunk + cy * 32, 32, x, y + cy, bg);
}

void PTAD::Map::updateChunks()
{
  int8_t chunkXOffset, chunkYOffset;
  chunkX[0] = screenX / 256;
  chunkY[0] = screenY / 256;
  if (screenX < 0)
    chunkX[0] = -1;
  if (screenY < 0)
    chunkY[0] = -1;
  if (screenX < 0 && screenX % 8 != 0)
    chunkXOffset = (screenX / 8 - 1) - (chunkX[0] * 32);
  else
    chunkXOffset = (screenX / 8) - (chunkX[0] * 32);
  if (screenY < 0 && screenY % 8 != 0)
    chunkYOffset = (screenY / 8 - 1) - (chunkY[0] * 32);
  else
    chunkYOffset = (screenY / 8) - (chunkY[0] * 32);
  for (int i = 1; i < 4; ++i)
  {
    chunkX[i] = chunkX[0] + (i % 2);
    chunkY[i] = chunkY[0] + (i / 2);
  }
  PTAD::clearTiles(screenX, screenY);
  for (int i = 0; i < 4; ++i)
  {
    if (chunkX[i] < 0 || chunkX[i] >= data->width || chunkY[i] < 0 || chunkY[i] >= data->height)
      chunkID[i] = -1;
    else
    {
      if (chunkID[i] != chunkY[i] * data->width + chunkX[i])
      {
        chunkID[i] = chunkY[i] * data->width + chunkX[i];
        mapFile.seek(sizeof(MapData) + PTAD::MEMORY_MAP_PASSABILITY_SIZE + chunkID[i] * 2048);
        PTAD::dataFile->readBytes(&mapFile, chunksBG + i * 1024, 1024);
        PTAD::dataFile->readBytes(&mapFile, chunksFG + i * 1024, 1024);
      }
      drawChunk(chunksBG + i * 1024, (i % 2) * 32 - chunkXOffset, (i / 2) * 32 - chunkYOffset, true);
      drawChunk(chunksFG + i * 1024, (i % 2) * 32 - chunkXOffset, (i / 2) * 32 - chunkYOffset, false);
    }
  }
}

bool PTAD::Map::canMove(int x, int y)
{
  if (x < 0)
    return false;
  if (x >= data->width * 16)
    return false;
  if (y < 0)
    return false;
  if (y >= data->height * 16)
    return false;
  return (passability[(y / 8) * data->width * 16 + x] >> (y % 8)) & 1;
}

uint8_t PTAD::Map::eventAtLocation(uint8_t x, uint8_t y)
{
  if (x < 0)
    return 255;
  if (x >= data->width * 16)
    return 255;
  if (y < 0)
    return 255;
  if (y >= data->height * 16)
    return 255;
  for (uint8_t i = 0; data->events[i].flags != PTAD::MapEvent::FLAGS_NULL; ++i)
  {
    if (data->events[i].x == x && data->events[i].y == y)
    {
      PTAD::MapEvent::setup(PTAD::Game::player.mapHash);
      //PTAD::MapEvent::setup(maps[PTAD::Game::player.mapID].eventsFile);
      return i;
    }
  }
  return 255;
}

void PTAD::Map::randomBattle()
{
  uint8_t tileIndex = PTAD::tilemapBG[303];
  uint8_t encounterRate = tiledata[tileIndex * 2 + 1];
  if (steps < data->minSteps)
    return;
  PTAD::battleMonsterID = 255;
  PTAD::battleBG = tiledata[tileIndex * 2];
  PTAD::battleBGM = 2;
  if (encounterRate != 0 && (random(0, encounterRate) == 0) || (steps >= data->maxSteps && data->maxSteps != 0))
  {
    PTAD::battleMonsterID = data->regions[(PTAD::Game::player.y / 128) * data->width * 2 + (PTAD::Game::player.x / 128)][random(0, 4)];
    if (PTAD::battleMonsterID != 255)
      PTAD::Game::state = PTAD::Game::State::BattleInit;
  }
}

void PTAD::Map::drawMainMenu()
{
  PTAD::Ui::drawFrame(2, 1, 9, 12);
  for (int i = 0; i < 10; ++i)
    PTAD::Ui::drawText(txtOptions + i * 6, 6, 3, i + 2);
}

void PTAD::Map::drawStatsMenu()
{
  uint32_t h, m, s = PTAD::Game::player.playTime / 1000;
  uint16_t playerStats[4] = {PTAD::Game::player.stats.attack, PTAD::Game::player.stats.defense, PTAD::Game::player.stats.agility, PTAD::Game::player.stats.magic};
  uint16_t spellResistance = PTAD::Game::calculateSpellResistance();
  PTAD::Ui::drawFrame(9, 0, 18, 1);
  m = s / 60;
  h = m / 60;
  m = m % 60;
  s = s % 60;
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + (h / 10) % 10, 10, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + h % 10, 11, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + (m / 10) % 10, 13, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + m % 10, 14, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + (s / 10) % 10, 16, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + s % 10, 17, 0);
  PTAD::Ui::fixFrameHeader(9, 18, 0);
  PTAD::Ui::drawFrame(1, 1, 26, 20);
  for (int i = 0; i < 18; ++i)
    PTAD::Ui::drawText(statsMenu + i * 24, 24, 2, i + 2);
  PTAD::Ui::drawText(PTAD::Game::player.name, 8, 2, 2);
  PTAD::Ui::drawNumber(PTAD::Game::player.level, 24, 2, 10);
  PTAD::Ui::drawNumber(PTAD::Game::player.hp, 6, 3, 1000);
  PTAD::Ui::drawNumber(PTAD::Game::player.maxHP, 11, 3, 1000);
  PTAD::Ui::drawNumber(PTAD::Game::player.mp, 19, 3, 100);
  PTAD::Ui::drawNumber(PTAD::Game::player.maxMP, 23, 3, 100);
  PTAD::Ui::drawNumber(PTAD::Game::player.nextLevel - PTAD::Game::player.experience, 6, 4, 1000);
  PTAD::Ui::drawNumber(PTAD::Game::player.gold, 21, 4, 10000);
  PTAD::Ui::drawNumber(PTAD::Game::player.stats.attack, 8, 7, 100);
  PTAD::Ui::drawNumber(PTAD::Game::player.stats.defense, 13, 7, 100);
  PTAD::Ui::drawNumber(PTAD::Game::player.stats.agility, 18, 7, 100);
  PTAD::Ui::drawNumber(PTAD::Game::player.stats.magic, 23, 7, 100);
  for (int i = 0; i < 4; ++i)
  {
    PTAD::Ui::drawNumber(playerStats[i], 8 + i * 5, 7, 100);
    for (int j = 0; j < 6; ++j)
    {
      if (PTAD::Game::player.equippedItems[j] == 255)
      {
        PTAD::Ui::drawCharacter(PTAD::FONT_0, 10, 9 + j);
        PTAD::Ui::drawCharacter(PTAD::FONT_0, 15, 9 + j);
        PTAD::Ui::drawCharacter(PTAD::FONT_0, 20, 9 + j);
        PTAD::Ui::drawCharacter(PTAD::FONT_0, 25, 9 + j);
      }
      else
      {
        playerStats[i] += PTAD::equipmentStats[j][PTAD::Game::player.equippedItems[j]].stats[i];
        if (PTAD::equipmentStats[j][PTAD::Game::player.equippedItems[j]].stats[i] < 0)
        {
          PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 7 + i * 5, 9 + j);
          PTAD::Ui::drawNumber(PTAD::equipmentStats[j][PTAD::Game::player.equippedItems[j]].stats[i] * -1, 8 + i * 5, 9 + j, 100);
        }
        else if (PTAD::equipmentStats[j][PTAD::Game::player.equippedItems[j]].stats[i] == 0)
          PTAD::Ui::drawCharacter(PTAD::FONT_0, 10 + i * 5, 9 + j);
        else
        {
          PTAD::Ui::drawCharacter(PTAD::FONT_PLUS, 7 + i * 5, 9 + j);
          PTAD::Ui::drawNumber(PTAD::equipmentStats[j][PTAD::Game::player.equippedItems[j]].stats[i], 8 + i * 5, 9 + j, 100);
        }
      }
    }
    PTAD::Ui::drawNumber(playerStats[i], 8 + i * 5, 16, 100);
  }
  for (int i = 0; i < 7; ++i)
    PTAD::Ui::drawCharacter(PTAD::FONT_DMG_0 + ((spellResistance >> (i * 2)) & 3), 3 + (i % 4) * 2, 18 + (i / 4));
  PTAD::Ui::drawNumber(PTAD::Game::getPoisonResistance(), 23, 18, 100);
  PTAD::Ui::drawNumber(PTAD::Game::getSlowResistance(), 23, 19, 100);
}

void PTAD::Map::drawItemsMenu()
{
  bool hasItems = false;
  PTAD::Ui::drawFrame(13, 1, 26, 4);
  PTAD::Ui::drawFrame(1, 1, 13, 18);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  PTAD::Ui::drawCharacter(PTAD::FONT_H, 14, 2);
  PTAD::Ui::drawCharacter(PTAD::FONT_P, 15, 2);
  PTAD::Ui::drawNumber(PTAD::Game::player.hp, 17, 2, 1000);
  PTAD::Ui::drawCharacter(PTAD::FONT_SLASH, 21, 2);
  PTAD::Ui::drawNumber(PTAD::Game::player.maxHP, 22, 2, 1000);
  PTAD::Ui::drawCharacter(PTAD::FONT_M, 14, 3);
  PTAD::Ui::drawCharacter(PTAD::FONT_a, 15, 3);
  PTAD::Ui::drawCharacter(PTAD::FONT_n, 16, 3);
  PTAD::Ui::drawCharacter(PTAD::FONT_a, 17, 3);
  PTAD::Ui::drawNumber(PTAD::Game::player.mp, 19, 3, 100);
  PTAD::Ui::drawCharacter(PTAD::FONT_SLASH, 23, 3);
  PTAD::Ui::drawNumber(PTAD::Game::player.maxMP, 23, 3, 100);
  for (int i = 0; i < 16; ++i)
  {
    if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][i] > 0)
    {
      PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][i].name, 8, 2, i + 2);
      PTAD::Ui::drawNumber(PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][i], 11, i + 2, 10);
      hasItems = true;
    }
    else
      PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 11, 2, i + 2);
  }
  if (!hasItems)
    PTAD::Ui::drawText(infoText, 24, 2, 19);
  else if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][0] > 0)
    PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][0].description, 24, 2, 19);
  else
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
}

void PTAD::Map::drawSkillsMenu()
{
  PTAD::Ui::drawFrame(3, 1, 15, 18);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  for (int i = 0; i < 16; ++i)
  {
    if (((PTAD::Game::player.skills >> i) & 1) != 0)
    {
      PTAD::Ui::drawText(PTAD::skills[i].name, 8, 4, i + 2);
      PTAD::Ui::drawNumber(PTAD::skills[i].price, 13, i + 2, 10);
    }
  }
  if (PTAD::Game::player.skills == 0)
    PTAD::Ui::drawText(infoText + 72, 24, 2, 19);
  else if ((PTAD::Game::player.skills & 1) != 0)
    PTAD::Ui::drawText(PTAD::skills[0].description, 24, 2, 19);
  else
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
}

void PTAD::Map::drawSpellsMenu()
{
  PTAD::Ui::drawFrame(3, 1, 15, 18);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  for (int i = 0; i < 16; ++i)
  {
    if (((PTAD::Game::player.spells >> i) & 1) != 0)
    {
      PTAD::Ui::drawText(PTAD::spells[i].name, 8, 4, i + 2);
      PTAD::Ui::drawNumber(PTAD::spells[i].price, 13, i + 2, 10);
    }
  }
  if (PTAD::Game::player.spells == 0)
    PTAD::Ui::drawText(infoText + 96, 24, 2, 19);
  else if ((PTAD::Game::player.spells & 1) != 0)
    PTAD::Ui::drawText(PTAD::spells[0].description, 24, 2, 19);
  else
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
}

void PTAD::Map::drawDialogMenu()
{
  PTAD::Ui::drawFrame(3, 6, 11, 13);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_MENU_DIALOG_TEST);
  PTAD::Dialog::beginMessage(false);
  for (int i = 0; i < 6; ++i)
    PTAD::Ui::drawText(PTAD::Game::messageSpeeds + i * 8, 8, 4, i + 7);
}

void PTAD::Map::drawBattleMenu()
{
  PTAD::Ui::drawFrame(3, 7, 11, 14);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_MENU_DIALOG_TEST);
  PTAD::Dialog::beginMessage(false);
  for (int i = 0; i < 6; ++i)
    PTAD::Ui::drawText(PTAD::Game::messageSpeeds + i * 8, 8, 4, i + 8);
}

void PTAD::Map::drawVolumeMenu()
{
  PTAD::Ui::drawFrame(10, 8, 12, 15);
  for (uint8_t i = 0; i < Pokitto::discrete_vol; ++i)
    PTAD::Ui::drawCharacter(PTAD::FONT_SOLID, 11, 14 - i);
}

void PTAD::Map::drawShopMenu()
{
  PTAD::Ui::drawFrame(1, 1, 6, 4);
  PTAD::Ui::drawCharacter(PTAD::FONT_B, 2, 2);
  PTAD::Ui::drawCharacter(PTAD::FONT_u, 3, 2);
  PTAD::Ui::drawCharacter(PTAD::FONT_y, 4, 2);
  PTAD::Ui::drawCharacter(PTAD::FONT_S, 2, 3);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 3, 3);
  PTAD::Ui::drawCharacter(PTAD::FONT_l, 4, 3);
  PTAD::Ui::drawCharacter(PTAD::FONT_l, 5, 3);
  PTAD::Ui::drawFrame(3, 15, 9, 17);
  PTAD::Ui::drawCharacter(PTAD::FONT_G, 4, 15);
  PTAD::Ui::drawCharacter(PTAD::FONT_o, 5, 15);
  PTAD::Ui::drawCharacter(PTAD::FONT_l, 6, 15);
  PTAD::Ui::drawCharacter(PTAD::FONT_d, 7, 15);
  PTAD::Ui::drawNumber(PTAD::Game::player.gold, 4, 16, 10000);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  PTAD::Ui::drawText(shopMessage, 24, 2, 19);
}

void PTAD::Map::drawShopMenu_BuyWhat()
{
  PTAD::Ui::drawFrame(1, 1, 9, 9);
  PTAD::Ui::drawFrame(12, 1, 25, 18);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  for (int i = 0; i < 7; ++i)
    PTAD::Ui::drawText(shopBuyOptions[i], 7, 2, 2 + i);
  PTAD::Ui::drawText(shopMessage, 24, 2, 19);
  drawShopMenu_BuyItems(0);
}

void PTAD::Map::drawShopMenu_BuyItems(uint8_t type)
{
  if (type > 0)
    ++type;
  for (int i = 0; i < 16; ++i)
  {
    if (type == 0 && ((shopItems >> i) & 1) == 0)
      PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 12, 13, 2 + i);
    else if (type > 0 && ((shopEquipment[type - 2] >> i) & 1) == 0)
      PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 12, 13, 2 + i);
    else if (type > 0 && ((shopEquipment[type - 2] >> i) & 1) == 1 && ((PTAD::Game::player.equippableItems[type - 2] >> i) & 1) == 1)
      PTAD::Ui::drawText(shopSoldOut, 12, 13, 2 + i);
    else
    {
      PTAD::Ui::drawText(PTAD::items[type][i].name, 8, 13, 2 + i);
      PTAD::Ui::drawNumber(PTAD::items[type][i].price, 22, 2 + i, 100);
    }
  }
}

void PTAD::Map::drawShopMenu_BuyQty()
{
  PTAD::Ui::drawFrame(1, 10, 13, 14);
  PTAD::Ui::drawCharacter(PTAD::FONT_Y, 2, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_o, 3, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_u, 4, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_H, 6, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_a, 7, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_v, 8, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 9, 11);
  PTAD::Ui::drawNumber(PTAD::Game::player.items[0][itemID], 11, 11, 10);
  PTAD::Ui::drawCharacter(PTAD::FONT_P, 2, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_u, 3, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_r, 4, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_c, 5, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_h, 6, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_a, 7, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_s, 8, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 9, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_1, 12, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_P, 2, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_r, 3, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_i, 4, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_c, 5, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 6, 13);
  PTAD::Ui::drawNumber(PTAD::items[0][itemID].price, 8, 13, 10000);
  PTAD::Ui::drawText(shopHowMany, 24, 2, 19);
}

void PTAD::Map::drawShopMenu_SellWhat()
{
  PTAD::Ui::drawFrame(12, 1, 25, 18);
  PTAD::Ui::drawFrame(1, 18, 26, 20);
  for (int i = 0; i < 16; ++i)
  {
    if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_TREASURES][i] > 0)
    {
      PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_TREASURES][i].name, 8, 13, 2 + i);
      PTAD::Ui::drawNumber(PTAD::items[PTAD::Game::ITEM_TYPE_TREASURES][i].price, 22, 2 + i, 100);
    }
    else
      PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 12, 13, 2 + i);
  }
  if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_TREASURES][PTAD::globalCounter] > 0)
    PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_TREASURES][PTAD::globalCounter].description, 24, 2, 19);
  else
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
}

void PTAD::Map::drawShopMenu_SellQty()
{
  PTAD::Ui::drawFrame(1, 10, 13, 14);
  PTAD::Ui::drawCharacter(PTAD::FONT_Y, 2, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_o, 3, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_u, 4, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_H, 6, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_a, 7, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_v, 8, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 9, 11);
  PTAD::Ui::drawNumber(PTAD::Game::player.items[1][itemID], 11, 11, 10);
  PTAD::Ui::drawCharacter(PTAD::FONT_Y, 2, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_o, 3, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_u, 4, 11);
  PTAD::Ui::drawCharacter(PTAD::FONT_S, 6, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 7, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_l, 8, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_l, 9, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_1, 12, 12);
  PTAD::Ui::drawCharacter(PTAD::FONT_P, 2, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_r, 3, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_i, 4, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_c, 5, 13);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 6, 13);
  PTAD::Ui::drawNumber(PTAD::items[1][itemID].price, 8, 13, 10000);
  PTAD::Ui::drawText(shopSellQtyMessage, 24, 2, 19);
}

void PTAD::Map::updateMainMenu()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 9;
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 10)
        PTAD::globalCounter = 0;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
    if (PTAD::globalCounter == 0) //Stats Menu
    {
      menuState = MenuState::StatsMenu;
      drawStatsMenu();
    }
    else if (PTAD::globalCounter == 1) //Equip Menu
    {
      PTAD::Ui::clear();
      PTAD::Menu::setup(PTAD::Menu::State::EquipMenu);
      PTAD::Game::state = PTAD::Game::State::MenuInit;
      menuState = MenuState::EquipMenu;
      lastPress = PTAD::globalCounter;
    }
    else if (PTAD::globalCounter == 2) //Items Menu
    {
      menuState = MenuState::ItemsMenu;
      PTAD::globalCounter = -4;
      for (int i = 0; i < 16; ++i)
      {
        if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][i] > 0)
        {
          PTAD::globalCounter = 0;
          break;
        }
      }
      drawItemsMenu();
    }
    else if (PTAD::globalCounter == 3) //Skills Menu
    {
      menuState = MenuState::SkillsMenu;
      if (PTAD::Game::player.skills == 0)
        PTAD::globalCounter = -4;
      else
        PTAD::globalCounter = 0;
      drawSkillsMenu();
    }
    else if (PTAD::globalCounter == 4) //Spells Menu
    {
      menuState = MenuState::SpellsMenu;
      if (PTAD::Game::player.spells == 0)
        PTAD::globalCounter = -4;
      else
        PTAD::globalCounter = 0;
      drawSpellsMenu();
    }
    else if (PTAD::globalCounter == 5) //Dialog Menu
    {
      menuState = MenuState::DialogMenu;
      drawDialogMenu();
    }
    else if (PTAD::globalCounter == 6) //Battle Menu
    {
      menuState = MenuState::BattleMenu;
      PTAD::Dialog::setMessageSpeed((PTAD::Game::player.messageSpeed >> 4) & 0xF);
      drawBattleMenu();
    }
    else if (PTAD::globalCounter == 7) //Volume Menu
    {
      menuState = MenuState::VolumeMenu;
      drawVolumeMenu();
    }
    else if (PTAD::globalCounter == 8) //Beasts Menu
    {
      PTAD::Ui::clear();
      PTAD::Menu::setup(PTAD::Menu::State::BeastsMenu);
      PTAD::Game::state = PTAD::Game::State::MenuInit;
      menuState = MenuState::BeastsMenu;
      lastPress = PTAD::globalCounter;
      chunkID[0] = -1;
      chunkID[1] = -1;
      chunkID[2] = -1;
      chunkID[3] = -1;
    }
    else if (PTAD::globalCounter == 9) //Save Menu
    {
      PTAD::Menu::setup(PTAD::Menu::State::SaveMenu);
      PTAD::Game::state = PTAD::Game::State::MenuInit;
      menuState = MenuState::SaveMenu;
      lastPress = PTAD::globalCounter;
    }
    return;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    state = State::Idle;
  }
  else
    lastPress = 0;
  if (state == State::Menu)
  {
    PTAD::Ui::cursorX = 6;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
  else
  {
    PTAD::Ui::clear();
    PTAD::globalCounter = 0;
    PTAD::Ui::cursorY = -16;
  }
}

void PTAD::Map::updateStatsMenu()
{
  uint32_t h, m, s = PTAD::Game::player.playTime / 1000;
  m = s / 60;
  h = m / 60;
  m = m % 60;
  s = s % 60;
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + (h / 10) % 10, 10, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + h % 10, 11, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + (m / 10) % 10, 13, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + m % 10, 14, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + (s / 10) % 10, 16, 0);
  PTAD::Ui::drawCharacter(PTAD::FONT_0 + s % 10, 17, 0);
  if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::MainMenu;
    PTAD::Ui::clear();
    drawMainMenu();
  }
}

void PTAD::Map::updateEquipMenu()
{
  menuState = MenuState::MainMenu;
  PTAD::Ui::clear();
  drawMainMenu();
  PTAD::globalCounter = 1;
  PD::lineFillers[0] = &PTAD::tileFillerBG;
  PD::lineFillers[1] = &TAS::SpriteFiller;
  PD::lineFillers[2] = &PTAD::tileFillerFG;
  PD::lineFillers[3] = &PTAD::Ui::lineFiller;
}

void PTAD::Map::updateItemsMenu()
{
  if (PB::upBtn() && PTAD::globalCounter >= 0)
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
  else if (PB::downBtn() && PTAD::globalCounter >= 0)
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
    if (PTAD::globalCounter < 0)
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    else if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter] == 0)
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    else if (PTAD::globalCounter >= PTAD::ITEMS_BATTLE_ONLY)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(infoText + 24, 24, 2, 19);
    }
    else
    {
      if (hpRecovered[PTAD::globalCounter] > 0 && mpRecovered[PTAD::globalCounter] == 0 && PTAD::Game::player.hp == PTAD::Game::player.maxHP)
      {
        PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
        PTAD::Ui::drawText(infoText + 48, 24, 2, 19);
      }
      else if (hpRecovered[PTAD::globalCounter] == 0 && mpRecovered[PTAD::globalCounter] > 0 && PTAD::Game::player.mp == PTAD::Game::player.maxMP)
      {
        PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
        PTAD::Ui::drawText(infoText + 48, 24, 2, 19);
      }
      else if (hpRecovered[PTAD::globalCounter] > 0 && mpRecovered[PTAD::globalCounter] > 0 && PTAD::Game::player.hp == PTAD::Game::player.maxHP && PTAD::Game::player.mp == PTAD::Game::player.maxMP)
      {
        PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
        PTAD::Ui::drawText(infoText + 48, 24, 2, 19);
      }
      else
      {
        PTAD::Music::playSFX(PTAD::Music::SFX_USEITEM);
        --PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter];
        PTAD::Game::player.hp += hpRecovered[PTAD::globalCounter];
        PTAD::Game::player.mp += mpRecovered[PTAD::globalCounter];
        if (PTAD::Game::player.hp > PTAD::Game::player.maxHP)
          PTAD::Game::player.hp = PTAD::Game::player.maxHP;
        if (PTAD::Game::player.mp > PTAD::Game::player.maxMP)
          PTAD::Game::player.mp = PTAD::Game::player.maxMP;
        if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter] > 0)
          PTAD::Ui::drawNumber(PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter], 11, PTAD::globalCounter + 2, 10);
        else
        {
          PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 11, 2, PTAD::globalCounter + 2);
          PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
        }
        if (PTAD::Game::player.hp > PTAD::Game::player.maxHP / 4)
          PTAD::Ui::fgColor = 7;
        else
          PTAD::Ui::fgColor = 95;
      }
    }
    PTAD::Ui::drawNumber(PTAD::Game::player.hp, 17, 2, 1000);
    PTAD::Ui::drawNumber(PTAD::Game::player.mp, 19, 3, 100);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::MainMenu;
    PTAD::globalCounter = 2;
  }
  if (menuState == MenuState::ItemsMenu)
  {
    PTAD::Ui::cursorX = -2;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
  else
  {
    PTAD::Ui::clear();
    drawMainMenu();
  }
}

void PTAD::Map::updateSkillsMenu()
{
  if (PB::upBtn() && PTAD::globalCounter >= 0)
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
  else if (PB::downBtn() && PTAD::globalCounter >= 0)
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
    PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    PTAD::Ui::drawText(infoText + 24, 24, 2, 19);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::MainMenu;
    PTAD::globalCounter = 3;
  }
  if (menuState == MenuState::SkillsMenu)
  {
    PTAD::Ui::cursorX = 14;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
  else
  {
    PTAD::Ui::clear();
    drawMainMenu();
  }
}

void PTAD::Map::updateSpellsMenu()
{
  if (PB::upBtn() && PTAD::globalCounter >= 0)
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
  else if (PB::downBtn() && PTAD::globalCounter >= 0)
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
    PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    PTAD::Ui::drawText(infoText + 24, 24, 2, 19);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::MainMenu;
    PTAD::globalCounter = 4;
  }
  if (menuState == MenuState::SpellsMenu)
  {
    PTAD::Ui::cursorX = 14;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
  else
  {
    PTAD::Ui::clear();
    drawMainMenu();
  }
}

void PTAD::Map::updateDialogMenu()
{
  int8_t speed = PTAD::Dialog::getMessageSpeed();
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --speed;
      if (speed == -1)
        speed = 5;
      PTAD::Dialog::setMessageSpeed(speed);
      PTAD::Dialog::stopMessage();
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_MENU_DIALOG_TEST);
      PTAD::Dialog::beginMessage(false);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++speed;
      if (speed == 6)
        speed = 0;
      PTAD::Dialog::setMessageSpeed(speed);
      PTAD::Dialog::stopMessage();
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_MENU_DIALOG_TEST);
      PTAD::Dialog::beginMessage(false);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
    PTAD::Game::player.messageSpeed = (PTAD::Game::player.messageSpeed & 0xF0) | PTAD::Dialog::getMessageSpeed();
    menuState = MenuState::MainMenu;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    PTAD::Dialog::setMessageSpeed(PTAD::Game::player.messageSpeed & 0xF);
    menuState = MenuState::MainMenu;
  }
  if (menuState == MenuState::DialogMenu)
  {
    PTAD::Ui::cursorX = 14;
    PTAD::Ui::cursorY = 56 + speed * 8;
  }
  else
  {
    PTAD::Dialog::stopMessage();
    PTAD::Ui::clear();
    drawMainMenu();
  }
}

void PTAD::Map::updateBattleMenu()
{
  int8_t speed = PTAD::Dialog::getMessageSpeed();
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --speed;
      if (speed == -1)
        speed = 5;
      PTAD::Dialog::setMessageSpeed(speed);
      PTAD::Dialog::stopMessage();
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_MENU_DIALOG_TEST);
      PTAD::Dialog::beginMessage(false);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++speed;
      if (speed == 6)
        speed = 0;
      PTAD::Dialog::setMessageSpeed(speed);
      PTAD::Dialog::stopMessage();
      PTAD::Dialog::addMessage(PTAD::Dialog::MESSAGES_MENU_DIALOG_TEST);
      PTAD::Dialog::beginMessage(false);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Game::player.messageSpeed = (PTAD::Game::player.messageSpeed & 0xF) | (PTAD::Dialog::getMessageSpeed() << 4);
    PTAD::Dialog::setMessageSpeed(PTAD::Game::player.messageSpeed & 0xF);
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
    menuState = MenuState::MainMenu;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    PTAD::Dialog::setMessageSpeed(PTAD::Game::player.messageSpeed & 0xF);
    menuState = MenuState::MainMenu;
  }
  if (menuState == MenuState::BattleMenu)
  {
    PTAD::Ui::cursorX = 14;
    PTAD::Ui::cursorY = 64 + speed * 8;
  }
  else
  {
    PTAD::Dialog::stopMessage();
    PTAD::Ui::clear();
    drawMainMenu();
  }
}

void PTAD::Map::updateVolumeMenu()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      lastPress = PC::getTime();
      if (Pokitto::discrete_vol < 6)
      {
        PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
        ++Pokitto::discrete_vol;
        PTAD::Ui::drawCharacter(PTAD::FONT_SOLID, 11, 15 - Pokitto::discrete_vol);
      }
      Audio::setVolume(Pokitto::discrete_vol_levels[Pokitto::discrete_vol]);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      lastPress = PC::getTime();
      if (Pokitto::discrete_vol > 0)
      {
        PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
        --Pokitto::discrete_vol;
        PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 11, 14 - Pokitto::discrete_vol);
      }
      Audio::setVolume(Pokitto::discrete_vol_levels[Pokitto::discrete_vol]);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
    menuState = MenuState::MainMenu;
    PTAD::Game::volume = Pokitto::discrete_vol;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::MainMenu;
    Pokitto::discrete_vol = PTAD::Game::volume;
    Audio::setVolume(Pokitto::discrete_vol_levels[Pokitto::discrete_vol]);
  }
  if (menuState == MenuState::VolumeMenu)
  {
    PTAD::Ui::cursorX = 70;
    PTAD::Ui::cursorY = 116 - 8 * Pokitto::discrete_vol;
  }
  else
  {
    PTAD::Ui::clear();
    drawMainMenu();
  }
}

void PTAD::Map::updateBeastsMenu()
{
  menuState = MenuState::MainMenu;
  PTAD::Ui::clear();
  updateChunks();
  drawMainMenu();
  PTAD::globalCounter = 8;
  PD::lineFillers[0] = &PTAD::tileFillerBG;
  PD::lineFillers[1] = &TAS::SpriteFiller;
  PD::lineFillers[2] = &PTAD::tileFillerFG;
  PD::lineFillers[3] = &PTAD::Ui::lineFiller;
}

void PTAD::Map::updateSaveMenu()
{
  menuState = MenuState::MainMenu;
  PTAD::Ui::clear();
  drawMainMenu();
  PTAD::globalCounter = 9;
  PD::lineFillers[0] = &PTAD::tileFillerBG;
  PD::lineFillers[1] = &TAS::SpriteFiller;
  PD::lineFillers[2] = &PTAD::tileFillerFG;
  PD::lineFillers[3] = &PTAD::Ui::lineFiller;
}

void PTAD::Map::updateShopMenu()
{
  if (PB::upBtn() || PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP) || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      PTAD::globalCounter ^= 8;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
    if (PTAD::globalCounter == 0) //Buy
    {
      drawShopMenu_BuyWhat();
      menuState = MenuState::ShopMenu_BuyWhat;
    }
    else if (PTAD::globalCounter == 8) //Sell
    {
      PTAD::globalCounter = 0;
      drawShopMenu_SellWhat();
      menuState = MenuState::ShopMenu_SellWhat;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    state = State::Idle;
  }
  if (state == State::Menu && menuState == MenuState::ShopMenu)
  {
    PTAD::Ui::cursorX = -2;
    PTAD::Ui::cursorY = 16 + PTAD::globalCounter;
  }
  else if (state != State::Menu)
  {
    PTAD::Ui::clear();
    PTAD::globalCounter = 0;
    PTAD::Ui::cursorY = -16;
  }
}

void PTAD::Map::updateShopMenu_BuyWhat()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      PTAD::Ui::drawText(shopMessage, 24, 2, 19);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 6;
      drawShopMenu_BuyItems(PTAD::globalCounter);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      PTAD::Ui::drawText(shopMessage, 24, 2, 19);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 7)
        PTAD::globalCounter = 0;
      drawShopMenu_BuyItems(PTAD::globalCounter);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::globalCounter == 0 && shopItems == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(shopNoneAvailable, 24, 2, 19);
    }
    else if (PTAD::globalCounter > 0 && shopEquipment[PTAD::globalCounter - 1] == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(shopNoneAvailable, 24, 2, 19);
    }
    else if (PTAD::globalCounter == 0) //Items
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      menuState = MenuState::ShopMenu_BuyItems;
      PTAD::globalCounter = 0;
      while (((shopItems >> PTAD::globalCounter) & 1) == 0)
        ++PTAD::globalCounter;
      PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_CONSUMABLES][PTAD::globalCounter].description, 24, 2, 19);
    }
    else //Equipment
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      menuState = MenuState::ShopMenu_BuyEquipment;
      itemID = PTAD::globalCounter - 1;
      PTAD::globalCounter = 0;
      while (((shopEquipment[itemID] >> PTAD::globalCounter) & 1) == 0)
        ++PTAD::globalCounter;
      if (((PTAD::Game::player.equippableItems[itemID] >> PTAD::globalCounter) & 1) != 0)
        PTAD::Ui::drawText(shopSoldOutMessage, 24, 2, 19);
      else
        PTAD::Ui::drawText(PTAD::items[itemID + 2][PTAD::globalCounter].description, 24, 2, 19);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    PTAD::Ui::clear();
    drawShopMenu();
    menuState = MenuState::ShopMenu;
    PTAD::globalCounter = 0;
  }
  if (menuState == MenuState::ShopMenu_BuyWhat)
  {
    PTAD::Ui::cursorX = -2;
    PTAD::Ui::cursorY = 16 + PTAD::globalCounter * 8;
  }
}

void PTAD::Map::updateShopMenu_BuyItems()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      PTAD::globalCounter = (PTAD::globalCounter + 15) % 16;
      while (((shopItems >> PTAD::globalCounter) & 1) == 0)
        PTAD::globalCounter = (PTAD::globalCounter + 15) % 16;
      PTAD::Ui::drawText(PTAD::items[0][PTAD::globalCounter].description, 24, 2, 19);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      PTAD::globalCounter = (PTAD::globalCounter + 1) % 16;
      while (((shopItems >> PTAD::globalCounter) & 1) == 0)
        PTAD::globalCounter = (PTAD::globalCounter + 1) % 16;
      PTAD::Ui::drawText(PTAD::items[0][PTAD::globalCounter].description, 24, 2, 19);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::items[0][PTAD::globalCounter].price > PTAD::Game::player.gold)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(shopNotEnoughMoney, 24, 2, 19);
    }
    else if (PTAD::Game::player.items[0][PTAD::globalCounter] == 99)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(shopNotEnoughRoom, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      itemID = PTAD::globalCounter;
      PTAD::globalCounter = 1;
      menuState = MenuState::ShopMenu_BuyQty;
      PTAD::Ui::dialogCursorX = 102;
      PTAD::Ui::dialogCursorY = 97;
      PTAD::Ui::dialogCursor = PTAD::Ui::DIALOG_CURSOR_BLINK;
      drawShopMenu_BuyQty();
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::ShopMenu_BuyWhat;
    PTAD::globalCounter = 0;
    PTAD::Ui::drawText(shopMessage, 24, 2, 19);
  }
  if (menuState == MenuState::ShopMenu_BuyItems)
  {
    PTAD::Ui::cursorX = 86;
    PTAD::Ui::cursorY = 16 + PTAD::globalCounter * 8;
  }
}

void PTAD::Map::updateShopMenu_BuyEquipment()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      PTAD::globalCounter = (PTAD::globalCounter + 15) % 16;
      while (((shopEquipment[itemID] >> PTAD::globalCounter) & 1) == 0)
        PTAD::globalCounter = (PTAD::globalCounter + 15) % 16;
      if (((PTAD::Game::player.equippableItems[itemID] >> PTAD::globalCounter) & 1) == 1)
        PTAD::Ui::drawText(shopSoldOutMessage, 24, 2, 19);
      else
        PTAD::Ui::drawText(PTAD::items[itemID + 2][PTAD::globalCounter].description, 24, 2, 19);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      PTAD::globalCounter = (PTAD::globalCounter + 1) % 16;
      while (((shopEquipment[itemID] >> PTAD::globalCounter) & 1) == 0)
        PTAD::globalCounter = (PTAD::globalCounter + 1) % 16;
      if (((PTAD::Game::player.equippableItems[itemID] >> PTAD::globalCounter) & 1) == 1)
        PTAD::Ui::drawText(shopSoldOutMessage, 24, 2, 19);
      else
        PTAD::Ui::drawText(PTAD::items[itemID + 2][PTAD::globalCounter].description, 24, 2, 19);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (((PTAD::Game::player.equippableItems[itemID] >> PTAD::globalCounter) & 1) == 1)
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
    else if (PTAD::Game::player.gold < PTAD::items[itemID + 2][PTAD::globalCounter].price)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(shopNotEnoughMoney, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_COIN);
      PTAD::Game::player.gold -= PTAD::items[itemID + 2][PTAD::globalCounter].price;
      PTAD::Game::player.equippableItems[itemID] |= 1 << PTAD::globalCounter;
      PTAD::Ui::drawText(shopSoldOut, 12, 13, 2 + PTAD::globalCounter);
      PTAD::Ui::drawText(shopSoldOutMessage, 24, 2, 19);
      PTAD::Ui::drawNumber(PTAD::Game::player.gold, 4, 16, 10000);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::ShopMenu_BuyWhat;
    PTAD::globalCounter = itemID + 1;
    PTAD::Ui::drawText(shopMessage, 24, 2, 19);
  }
  if (menuState == MenuState::ShopMenu_BuyEquipment)
  {
    PTAD::Ui::cursorX = 86;
    PTAD::Ui::cursorY = 16 + PTAD::globalCounter * 8;
  }
}

void PTAD::Map::updateShopMenu_BuyQty()
{
  if (PB::leftBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_LEFT))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter < 1)
        PTAD::globalCounter = 1;
      PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
      PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[0][itemID].price, 8, 13, 10000);
    }
  }
  else if (PB::rightBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_RIGHT))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter > 99)
        PTAD::globalCounter = 99;
      PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
      PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[0][itemID].price, 8, 13, 10000);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_UP))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    PTAD::globalCounter = std::min(PTAD::Game::player.gold / PTAD::items[0][itemID].price, 99 - PTAD::Game::player.items[0][itemID]);
    PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
    PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[0][itemID].price, 8, 13, 10000);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_DOWN))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    PTAD::globalCounter = 1;
    PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
    PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[0][itemID].price, 8, 13, 10000);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::globalCounter * PTAD::items[0][itemID].price > PTAD::Game::player.gold)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(shopNotEnoughMoney, 24, 2, 19);
    }
    else if (PTAD::globalCounter + PTAD::Game::player.items[0][itemID] > 99)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(shopNotEnoughRoom, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_COIN);
      PTAD::Game::player.gold -= PTAD::globalCounter * PTAD::items[0][itemID].price;
      PTAD::Game::player.items[0][itemID] += PTAD::globalCounter;
      menuState = MenuState::ShopMenu_BuyItems;
      PTAD::Ui::drawNumber(PTAD::Game::player.gold, 4, 16, 10000);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::ShopMenu_BuyItems;
  }
  if (menuState != MenuState::ShopMenu_BuyQty)
  {
    PTAD::Ui::dialogCursor = 0;
    PTAD::Ui::fill(1, 10, 11, 14, 255);
    PTAD::Ui::fill(12, 10, 12, 14, PTAD::FONT_FRAME_L);
    PTAD::globalCounter = itemID;
    drawShopMenu_BuyItems(0);
  }
}

void PTAD::Map::updateShopMenu_SellWhat()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      PTAD::globalCounter = (PTAD::globalCounter + 15) % 16;
      if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_TREASURES][PTAD::globalCounter] > 0)
        PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_TREASURES][PTAD::globalCounter].description, 24, 2, 19);
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
      PTAD::globalCounter = (PTAD::globalCounter + 1) % 16;
      if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_TREASURES][PTAD::globalCounter] > 0)
        PTAD::Ui::drawText(PTAD::items[PTAD::Game::ITEM_TYPE_TREASURES][PTAD::globalCounter].description, 24, 2, 19);
      else
        PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::Game::player.items[PTAD::Game::ITEM_TYPE_TREASURES][PTAD::globalCounter] == 0)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(infoText + 5 * 24, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_SELECT);
      itemID = PTAD::globalCounter;
      PTAD::globalCounter = 1;
      menuState = MenuState::ShopMenu_SellQty;
      PTAD::Ui::dialogCursorX = 102;
      PTAD::Ui::dialogCursorY = 97;
      PTAD::Ui::dialogCursor = PTAD::Ui::DIALOG_CURSOR_BLINK;
      drawShopMenu_SellQty();
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    PTAD::Ui::clear();
    drawShopMenu();
    menuState = MenuState::ShopMenu;
    PTAD::globalCounter = 8;
  }
  if (menuState == MenuState::ShopMenu_SellWhat)
  {
    PTAD::Ui::cursorX = 86;
    PTAD::Ui::cursorY = 16 + PTAD::globalCounter * 8;
  }
}

void PTAD::Map::updateShopMenu_SellQty()
{
  if (PB::leftBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_LEFT))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter < 1)
        PTAD::globalCounter = 1;
      PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
      PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[1][itemID].price, 8, 13, 10000);
    }
  }
  else if (PB::rightBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_RIGHT))
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter > 99)
        PTAD::globalCounter = 99;
      PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
      PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[1][itemID].price, 8, 13, 10000);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_UP))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    PTAD::globalCounter = PTAD::Game::player.items[1][itemID];
    PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
    PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[1][itemID].price, 8, 13, 10000);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_DOWN))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CURSOR);
    PTAD::globalCounter = 1;
    PTAD::Ui::drawNumber(PTAD::globalCounter, 11, 12, 10);
    PTAD::Ui::drawNumber(PTAD::globalCounter * PTAD::items[1][itemID].price, 8, 13, 10000);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::Game::player.items[1][itemID] < PTAD::globalCounter)
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_INVALID);
      PTAD::Ui::drawText(infoText + 6 * 24, 24, 2, 19);
    }
    else
    {
      PTAD::Music::playSFX(PTAD::Music::SFX_COIN);
      PTAD::Game::player.gold += PTAD::globalCounter * PTAD::items[1][itemID].price;
      PTAD::Game::player.items[1][itemID] -= PTAD::globalCounter;
      menuState = MenuState::ShopMenu_SellWhat;
      PTAD::Ui::drawNumber(PTAD::Game::player.gold, 4, 16, 10000);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Music::SFX_CANCEL);
    menuState = MenuState::ShopMenu_SellWhat;
  }
  if (menuState != MenuState::ShopMenu_SellQty)
  {
    PTAD::Ui::dialogCursor = 0;
    PTAD::Ui::fill(1, 10, 11, 14, 255);
    PTAD::Ui::fill(12, 10, 12, 14, PTAD::FONT_FRAME_L);
    PTAD::globalCounter = itemID;
    drawShopMenu_SellWhat();
  }
}
