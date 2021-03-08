#pragma once

#include "headers.h"

namespace PTAD
{
  class Menu
  {
    public:
      enum class State
      {
        EquipMenu=0,
        EquipMenu_ChooseItem,
        BeastsMenu,
        NameEntryMenu,
        LoadMenu,
        SaveMenu,
        EraseSlot,
        SaveMenuEnd
      };
      static void setup(State s);
      static void update();
      static void drawItemNames(uint8_t x, uint8_t y, uint8_t itemType);
      static void drawItemQuantity(uint8_t x, uint8_t y, uint8_t itemType);
      static void drawEquipmentStats(uint8_t type);
      static void drawEquipmentStatsDifference(uint8_t type, uint8_t selection);
    private:
      static void updateEquipMenu();
      static void updateEquipMenu_ChooseItem();
      static void updateBeastsMenu();
      static void updateNameEntryMenu();
      static void updateSaveLoadMenu();
      static void updateEraseSlot();
      static void updateSaveMenuEnd();
      static void drawEquipMenu();
      static void drawEquipMenu_ChooseItem();
      static void drawBeastsMenu();
      static void drawNameEntryMenu();
      static void drawSaveLoadMenu();
      static void drawEraseSlot();
      static void nameEntryClearCursor(int loc);
      static void nameEntryDrawCursor(int loc);
      static State state;
      static uint32_t lastPress;
      static int32_t tempValue;
      static const uint8_t equipSlots[];
      static const uint8_t unequipped[];
      static const uint8_t unequip[];
      static const uint8_t noneAvailable[7][24];
      static const uint8_t statsText[];
      static const uint8_t spelText[];
      static const uint8_t statText[];
      static const uint8_t beastsMenu[];
      static const uint8_t nameEntryChars[];
      static const uint8_t emptySlotText[];
      static uint8_t *enemyImage;
  };
};
