#pragma once

#include "headers.h"

namespace PTAD
{
  class Map
  {
    friend class MapEvent;
    friend class Menu;
  public:
    struct MapData
    {
      uint8_t tilesetID;
      uint8_t bgmID;
      uint8_t width;
      uint8_t height;
      uint8_t sprites[16];
      uint8_t regions[64][4];
      MapEvent::Event events[29];
      uint8_t minSteps;
      uint8_t maxSteps;
      uint8_t flags;
      uint8_t unused;
    };
    enum class State
    {
      Idle,
      Menu,
      MoveUp,
      MoveDown,
      MoveLeft,
      MoveRight
    };
    enum class MenuState
    {
      MainMenu,
      StatsMenu,
      EquipMenu,
      ItemsMenu,
      SkillsMenu,
      SpellsMenu,
      DialogMenu,
      BattleMenu,
      VolumeMenu,
      BeastsMenu,
      SaveMenu,
      ShopMenu,
      ShopMenu_BuyWhat,
      ShopMenu_BuyItems,
      ShopMenu_BuyEquipment,
      ShopMenu_BuyQty,
      ShopMenu_SellWhat,
      ShopMenu_SellQty
    };
    static void setup();
    static void update();
    static bool isDark() {return (data->flags & FLAG_DARK) != 0;}
  private:
    static void loadSprites();
    static void drawChunk(const uint8_t *chunk, int x, int y, bool bg);
    static void updateChunks();
    static bool canMove(int x, int y);
    static uint8_t eventAtLocation(int x, int y);
    static void randomBattle();
    static void clipScreen();
    static void drawMainMenu();
    static void drawStatsMenu();
    static void drawItemsMenu();
    static void drawSkillsMenu();
    static void drawSpellsMenu();
    static void drawDialogMenu();
    static void drawBattleMenu();
    static void drawVolumeMenu();
    static void drawShopMenu();
    static void drawShopMenu_BuyWhat();
    static void drawShopMenu_BuyItems(uint8_t type);
    static void drawShopMenu_BuyQty();
    static void drawShopMenu_SellWhat();
    static void drawShopMenu_SellQty();
    static void updateMainMenu();
    static void updateStatsMenu();
    static void updateEquipMenu();
    static void updateItemsMenu();
    static void updateSkillsMenu();
    static void updateSpellsMenu();
    static void updateDialogMenu();
    static void updateBattleMenu();
    static void updateVolumeMenu();
    static void updateBeastsMenu();
    static void updateSaveMenu();
    static void updateShopMenu();
    static void updateShopMenu_BuyWhat();
    static void updateShopMenu_BuyItems();
    static void updateShopMenu_BuyEquipment();
    static void updateShopMenu_BuyQty();
    static void updateShopMenu_SellWhat();
    static void updateShopMenu_SellQty();
    static DataPack::PackedFile mapFile;
    static State state;
    static MenuState menuState;
    static int8_t chunkID[4];
    static int8_t chunkX[4];
    static int8_t chunkY[4];
    static int32_t screenX;
    static int32_t screenY;
    static uint32_t lastPress;
    static int8_t shakeOffsetX;
    static int8_t shakeOffsetY;
    static uint8_t shakeScreen;
    static uint8_t shakeMagnitude;
    static uint8_t playerFrame;
    static uint8_t eventID;
    static uint8_t shopMessage[24];
    static uint16_t shopItems;
    static uint16_t shopEquipment[6];
    static uint8_t itemID;
    static uint8_t steps;
    static uint8_t exitEventID;
    static MapData *data;
    #ifndef POK_SIM
    static uint8_t *passability;
    #else
    static uint8_t passability[];
    #endif
    static uint8_t *chunksBG;
    static uint8_t *chunksFG;
    static uint8_t *sprites;
    static uint8_t *tiledata;
    static const uint8_t playerFrames[];
    static const uint8_t movement[];
    //static const uint32_t tiledataFiles[];
    static const uint8_t txtOptions[];
    static const uint8_t statsMenu[];
    static const uint8_t infoText[];
    static const uint8_t hpRecovered[8];
    static const uint8_t mpRecovered[8];
    static const uint8_t shopSoldOut[];
    static const uint8_t shopBuyOptions[7][7];
    static const uint8_t shopNoneAvailable[];
    static const uint8_t shopNotEnoughMoney[];
    static const uint8_t shopHowMany[];
    static const uint8_t shopNotEnoughRoom[];
    static const uint8_t shopSoldOutMessage[];
    static const uint8_t shopSellQtyMessage[];
    static constexpr uint8_t FLAG_CLIP_UP = 1;
    static constexpr uint8_t FLAG_CLIP_DOWN = 2;
    static constexpr uint8_t FLAG_CLIP_LEFT = 4;
    static constexpr uint8_t FLAG_CLIP_RIGHT = 8;
    static constexpr uint8_t FLAG_DARK = 16;
  };
}
