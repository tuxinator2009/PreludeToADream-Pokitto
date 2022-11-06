#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

PTAD::Menu::State PTAD::Menu::state = PTAD::Menu::State::EquipMenu;
uint32_t PTAD::Menu::lastPress = 0;
int32_t PTAD::Menu::tempValue = 0;

const uint8_t PTAD::Menu::equipSlots[] =
{
  PTAD::FONT_W, PTAD::FONT_E, PTAD::FONT_A, PTAD::FONT_P, PTAD::FONT_O, PTAD::FONT_N,
  PTAD::FONT_H, PTAD::FONT_E, PTAD::FONT_L, PTAD::FONT_M, PTAD::FONT_E, PTAD::FONT_T,
  PTAD::FONT_A, PTAD::FONT_R, PTAD::FONT_M, PTAD::FONT_O, PTAD::FONT_R, PTAD::FONT_FRAME_U,
  PTAD::FONT_B, PTAD::FONT_O, PTAD::FONT_O, PTAD::FONT_T, PTAD::FONT_S, PTAD::FONT_FRAME_U,
  PTAD::FONT_R, PTAD::FONT_I, PTAD::FONT_N, PTAD::FONT_G, PTAD::FONT_FRAME_U, PTAD::FONT_FRAME_U,
  PTAD::FONT_A, PTAD::FONT_M, PTAD::FONT_U, PTAD::FONT_L, PTAD::FONT_E, PTAD::FONT_T
};

const uint8_t PTAD::Menu::unequipped[] = {PTAD::FONT_MINUS, PTAD::FONT_MINUS, PTAD::FONT_N, PTAD::FONT_O, PTAD::FONT_N, PTAD::FONT_E, PTAD::FONT_MINUS, PTAD::FONT_MINUS};
const uint8_t PTAD::Menu::unequip[] = {PTAD::FONT_SPACE, PTAD::FONT_U, PTAD::FONT_N, PTAD::FONT_E, PTAD::FONT_Q, PTAD::FONT_U, PTAD::FONT_I, PTAD::FONT_P};

const uint8_t PTAD::Menu::noneAvailable[7][24] = 
{
  //You have no weapons.____
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x4A,0x38,0x34,0x43,0x42,0x41,0x46,0x02,0x0A,0x0A,0x0A,0x0A,
  //You have no helmets.____
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x3B,0x38,0x3F,0x40,0x38,0x47,0x46,0x02,0x0A,0x0A,0x0A,0x0A,
  //You have no armor.______
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x34,0x45,0x40,0x42,0x45,0x02,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  //You have no boots.______
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x35,0x42,0x42,0x47,0x46,0x02,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  //You have no rings.______
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x45,0x3C,0x41,0x3A,0x46,0x02,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  //You have no amulets.____
  0x32,0x42,0x48,0x0A,0x3B,0x34,0x49,0x38,0x0A,0x41,0x42,0x0A,0x34,0x40,0x48,0x3F,0x38,0x47,0x46,0x02,0x0A,0x0A,0x0A,0x0A,
};

const uint8_t PTAD::Menu::statsText[] =
{
  PTAD::FONT_A, PTAD::FONT_T, PTAD::FONT_K,
  PTAD::FONT_D, PTAD::FONT_E, PTAD::FONT_F,
  PTAD::FONT_A, PTAD::FONT_G, PTAD::FONT_L,
  PTAD::FONT_M, PTAD::FONT_A, PTAD::FONT_G,
};

const uint8_t PTAD::Menu::spelText[] = {PTAD::FONT_S, PTAD::FONT_P, PTAD::FONT_E, PTAD::FONT_L};
const uint8_t PTAD::Menu::statText[] = {PTAD::FONT_S, PTAD::FONT_T, PTAD::FONT_A, PTAD::FONT_T};

const uint8_t PTAD::Menu::beastsMenu[]
{
  //......________
  //......HP_00000
  //......MP___000
  //......Exp_0000
  //......Gold_000
  //......________
  //______________
  //--------------
  //Attack_____000
  //Defense____000
  //Agility____000
  //Magic______000
  //--------------
  //F_E_W_A_L_D_S_
  //--------------
  //Poison_____000
  //Slowness___000
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x21,0x29,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x26,0x29,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x1E,0x4B,0x43,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x20,0x42,0x3F,0x37,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
  0x1A,0x47,0x47,0x34,0x36,0x3E,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x1D,0x38,0x39,0x38,0x41,0x46,0x38,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x1A,0x3A,0x3C,0x3F,0x3C,0x47,0x4C,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x26,0x34,0x3A,0x3C,0x36,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
  0x50,0x0A,0x51,0x0A,0x52,0x0A,0x53,0x0A,0x54,0x0A,0x55,0x0A,0x56,0x0A,
  0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
  0x29,0x42,0x3C,0x46,0x42,0x41,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x2C,0x3F,0x42,0x4A,0x41,0x38,0x46,0x46,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A
};

const uint8_t PTAD::Menu::nameEntryChars[] =
{
  PTAD::FONT_A, PTAD::FONT_B, PTAD::FONT_C, PTAD::FONT_D, PTAD::FONT_E, PTAD::FONT_F, PTAD::FONT_a, PTAD::FONT_b, PTAD::FONT_c, PTAD::FONT_d, PTAD::FONT_e, PTAD::FONT_f,
  PTAD::FONT_G, PTAD::FONT_H, PTAD::FONT_I, PTAD::FONT_J, PTAD::FONT_K, PTAD::FONT_L, PTAD::FONT_g, PTAD::FONT_h, PTAD::FONT_i, PTAD::FONT_j, PTAD::FONT_k, PTAD::FONT_l,
  PTAD::FONT_M, PTAD::FONT_N, PTAD::FONT_O, PTAD::FONT_P, PTAD::FONT_Q, PTAD::FONT_R, PTAD::FONT_m, PTAD::FONT_n, PTAD::FONT_o, PTAD::FONT_p, PTAD::FONT_q, PTAD::FONT_r,
  PTAD::FONT_S, PTAD::FONT_T, PTAD::FONT_U, PTAD::FONT_V, PTAD::FONT_W, PTAD::FONT_X, PTAD::FONT_s, PTAD::FONT_t, PTAD::FONT_u, PTAD::FONT_v, PTAD::FONT_w, PTAD::FONT_x,
  PTAD::FONT_Y, PTAD::FONT_Z, PTAD::FONT_EXCLAMATION, PTAD::FONT_COMMA, PTAD::FONT_PERIOD, PTAD::FONT_SLASH, PTAD::FONT_y, PTAD::FONT_z, PTAD::FONT_QUOTE, PTAD::FONT_QUESTION, PTAD::FONT_MINUS, PTAD::FONT_Ok
};

const uint8_t PTAD::Menu::emptySlotText[] = {PTAD::FONT_E, PTAD::FONT_m, PTAD::FONT_p, PTAD::FONT_t, PTAD::FONT_y, PTAD::FONT_SPACE, PTAD::FONT_S, PTAD::FONT_l, PTAD::FONT_o, PTAD::FONT_t};

uint8_t *PTAD::Menu::enemyImage = PTAD::memory + PTAD::MEMORY_MENU_BATTLER_OFFSET;

void PTAD::Menu::setup(State s)
{
  state = s;
  lastPress = 0;
  tempValue = 0;
  PTAD::globalCounter = 0;
  //TODO: Draw appropriate menu frames
  switch (state)
  {
    case State::EquipMenu:
      for (int i = 0; i < 6; ++i)
      {
        PTAD::Ui::drawFrame(1, i * 3 + 1, 10, i * 3 + 3);
        PTAD::Ui::drawText(equipSlots + i * 6, 6, 2, i * 3 + 1);
        PTAD::Ui::fixFrameHeader(1, 10, i * 3 + 1);
        if (PTAD::Game::player.equippedItems[i] != 255)
          PTAD::Ui::drawText(PTAD::Resources::items[i + 2][PTAD::Game::player.equippedItems[i]].name, 8, 2, i * 3 + 2);
        else
          PTAD::Ui::drawText(unequipped, 8, 2, i * 3 + 2);
      }
      PTAD::Ui::drawFrame(11, 1, 20, 18);
      PTAD::Ui::drawFrame(21, 1, 26, 18);
      PTAD::Ui::drawFrame(1, 18, 26, 20);
      PTAD::Ui::drawText(statsText, 3, 23, 2);
      PTAD::Ui::drawText(statsText + 3, 3, 23, 4);
      PTAD::Ui::drawText(statsText + 6, 3, 23, 6);
      PTAD::Ui::drawText(statsText + 9, 3, 23, 8);
      PTAD::Ui::drawText(spelText, 4, 22, 10);
      PTAD::Ui::drawText(statText, 4, 22, 15);
      drawEquipMenu();
      break;
    case State::EquipMenu_ChooseItem:
      //Nothing to draw
      break;
    case State::BeastsMenu:
      PTAD::Ui::drawFrame(1, 1, 10, 20);
      PTAD::Ui::drawFrame(11, 1, 26, 20);
      for (int i = 0; i < 17; ++i)
        PTAD::Ui::drawText(beastsMenu + i * 14, 14, 12, i + 3);
      drawBeastsMenu();
      break;
    case State::NameEntryMenu:
      drawNameEntryMenu();
      break;
    case State::LoadMenu:
      [[fallthrough]];
    case State::SaveMenu:
      drawSaveLoadMenu();
      break;
    case State::EraseSlot:
      //Nothing to draw
      break;
    case State::SaveMenuEnd:
      //Nothing to draw
      break;
  }
}

void PTAD::Menu::update()
{
  switch (state)
  {
    case State::EquipMenu:
      updateEquipMenu();
      break;
    case State::EquipMenu_ChooseItem:
      updateEquipMenu_ChooseItem();
      break;
    case State::BeastsMenu:
      updateBeastsMenu();
      break;
    case State::NameEntryMenu:
      updateNameEntryMenu();
      break;
    case State::LoadMenu:
      [[fallthrough]];
    case State::SaveMenu:
      updateSaveLoadMenu();
      break;
    case State::EraseSlot:
      updateEraseSlot();
      break;
    case State::SaveMenuEnd:
      updateSaveMenuEnd();
  }
}

void PTAD::Menu::drawItemNames(uint8_t x, uint8_t y, uint8_t itemType)
{
  for (int i = 0; i < 16; ++i)
  {
    if (itemType < 2)
      PTAD::Ui::drawText(PTAD::Resources::items[itemType][i].name, 8, x, y + i);
    else if (PTAD::Game::player.equippedItems[itemType - 2] == i)
      PTAD::Ui::drawText(unequip, 8, x, y + i);
    else if (((PTAD::Game::player.equippableItems[itemType - 2] >> i) & 1) != 0)
      PTAD::Ui::drawText(PTAD::Resources::items[itemType][i].name, 8, x, y + i);
    else
      PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 8, x, y + i);
  }
}

void PTAD::Menu::drawItemQuantity(uint8_t x, uint8_t y, uint8_t itemType)
{
  for (int i = 0; i < 16; ++i)
  {
    if (PTAD::Game::player.items[itemType][i] > 0)
      PTAD::Ui::drawNumber(PTAD::Game::player.items[itemType][i], x, y + i, 10);
  }
}

void PTAD::Menu::drawEquipmentStats(uint8_t type)
{
  uint16_t spellResistance = PTAD::Game::calculateSpellResistance();
  int8_t stats[4] = {0, 0, 0, 0};
  uint8_t statusResistance[2] = {0, 0};
  for (int i = 0; i < 6; ++i)
  {
    if (PTAD::Game::player.equippedItems[i] != 255)
    {
      stats[0] += PTAD::Resources::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[0];
      stats[1] += PTAD::Resources::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[1];
      stats[2] += PTAD::Resources::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[2];
      stats[3] += PTAD::Resources::equipmentStats[i][PTAD::Game::player.equippedItems[i]].stats[3];
      statusResistance[0] = PTAD::Resources::equipmentStats[i][PTAD::Game::player.equippedItems[i]].statusResistance[0];
      statusResistance[1] = PTAD::Resources::equipmentStats[i][PTAD::Game::player.equippedItems[i]].statusResistance[1];
    }
  }
  //Draw values to UI
  for (int i = 0; i < 4; ++i)
  {
    if (stats[i] < 0)
    {
      PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 22, i * 2 + 3);
      stats[i] *= -1;
    }
    else if (stats[i] > 0)
      PTAD::Ui::drawCharacter(PTAD::FONT_PLUS, 22, i * 2 + 3);
    else
      PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 22, i * 2 + 3);
    PTAD::Ui::drawNumber(stats[i], 23, i * 2 + 3, 100);
  }
  for (int i = 0; i < 7; ++i)
  {
    PTAD::Ui::drawCharacter(PTAD::FONT_ICON_FIRE + i, 22 + ((i % 2) * 2), 11 + (i / 2));
    PTAD::Ui::drawCharacter(PTAD::FONT_DMG_0 + ((spellResistance >> (i * 2)) & 3), 23 + ((i % 2) * 2), 11 + (i / 2));
  }
  if (statusResistance[0] < 0)
  {
    PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 22, 16);
    statusResistance[0] *= -1;
  }
  else if (statusResistance[0] > 0)
    PTAD::Ui::drawCharacter(PTAD::FONT_PLUS, 22, 16);
  else
    PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 22, 16);
  PTAD::Ui::drawNumber(statusResistance[0], 23, 16, 100);
  if (statusResistance[1] < 0)
  {
    PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 22, 17);
    statusResistance[1] *= -1;
  }
  else if (statusResistance[1] > 0)
    PTAD::Ui::drawCharacter(PTAD::FONT_PLUS, 22, 17);
  else
    PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 22, 17);
  PTAD::Ui::drawNumber(statusResistance[1], 23, 17, 100);
}

void PTAD::Menu::drawEquipmentStatsDifference(uint8_t type, uint8_t selection)
{
  int8_t statDiff[4] = {0, 0, 0, 0};
  int8_t spellResistance[7] = {-12, -12, -12, -12, -12, -12, -12};
  int16_t statusDifference[2] = {0, 0};
  for (int i = 0; i < 7; ++i)
  {
    for (int j = 0; j < 6; ++j)
    {
      //if player has equipment type 'j' equipped then add that equipment's resistance to spell 'i'
      if (PTAD::Game::player.equippedItems[j] != 255)
        spellResistance[i] += (PTAD::Resources::equipmentStats[j][PTAD::Game::player.equippedItems[j]].spellResistance >> (i * 2)) & 3;
      else
        spellResistance[i] += 2;
    }
    //subtract oldEquipment's resistance to spell 'i'
    if (PTAD::Game::player.equippedItems[type] != 255)
      spellResistance[i] -= (PTAD::Resources::equipmentStats[type][PTAD::Game::player.equippedItems[type]].spellResistance >> (i * 2)) & 3;
    else
      spellResistance[i] -= 2;
    //add newEquipment's resistance to spell 'i'
    if (selection != PTAD::Game::player.equippedItems[type])
      spellResistance[i] += (PTAD::Resources::equipmentStats[type][selection].spellResistance >> (i * 2)) & 3;
    else
      spellResistance[i] += 2;
    //Cap resistance between -2 and 1
    if (spellResistance[i] < -2)
      spellResistance[i] = -2;
    else if (spellResistance[i] > 1)
      spellResistance[i] = 1;
  }
  //Start statDiff with the stats of the newEquipment
  if (selection != PTAD::Game::player.equippedItems[type])
  {
    statDiff[0] = PTAD::Resources::equipmentStats[type][selection].stats[0];
    statDiff[1] = PTAD::Resources::equipmentStats[type][selection].stats[1];
    statDiff[2] = PTAD::Resources::equipmentStats[type][selection].stats[2];
    statDiff[3] = PTAD::Resources::equipmentStats[type][selection].stats[3];
    statusDifference[0] = PTAD::Resources::equipmentStats[type][selection].statusResistance[0];
    statusDifference[1] = PTAD::Resources::equipmentStats[type][selection].statusResistance[1];
  }
  //Determine the net loss/gain from the oldEquipment
  if (PTAD::Game::player.equippedItems[type] != 255)
  {
    statDiff[0] -= PTAD::Resources::equipmentStats[type][PTAD::Game::player.equippedItems[type]].stats[0];
    statDiff[1] -= PTAD::Resources::equipmentStats[type][PTAD::Game::player.equippedItems[type]].stats[1];
    statDiff[2] -= PTAD::Resources::equipmentStats[type][PTAD::Game::player.equippedItems[type]].stats[2];
    statDiff[3] -= PTAD::Resources::equipmentStats[type][PTAD::Game::player.equippedItems[type]].stats[3];
    statusDifference[0] -= PTAD::Resources::equipmentStats[type][PTAD::Game::player.equippedItems[type]].statusResistance[0];
    statusDifference[1] -= PTAD::Resources::equipmentStats[type][PTAD::Game::player.equippedItems[type]].statusResistance[1];
  }
  //Draw values to UI
  for (int i = 0; i < 4; ++i)
  {
    if (statDiff[i] < 0)
    {
      PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 22, i * 2 + 3);
      statDiff[i] *= -1;
    }
    else if (statDiff[i] > 0)
      PTAD::Ui::drawCharacter(PTAD::FONT_PLUS, 22, i * 2 + 3);
    else
      PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 22, i * 2 + 3);
    PTAD::Ui::drawNumber(statDiff[i], 23, i * 2 + 3, 100);
  }
  for (int i = 0; i < 7; ++i)
  {
    PTAD::Ui::drawCharacter(PTAD::FONT_ICON_FIRE + i, 22 + ((i % 2) * 2), 11 + (i / 2));
    PTAD::Ui::drawCharacter(PTAD::FONT_DMG_100 + spellResistance[i], 23 + ((i % 2) * 2), 11 + (i / 2));
  }
  if (statusDifference[0] < 0)
  {
    PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 22, 16);
    statusDifference[0] *= -1;
  }
  else if (statusDifference[0] > 0)
    PTAD::Ui::drawCharacter(PTAD::FONT_PLUS, 22, 16);
  else
    PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 22, 16);
  PTAD::Ui::drawNumber(statusDifference[0], 23, 16, 100);
  if (statusDifference[1] < 0)
  {
    PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 22, 17);
    statusDifference[1] *= -1;
  }
  else if (statusDifference[1] > 0)
    PTAD::Ui::drawCharacter(PTAD::FONT_PLUS, 22, 17);
  else
    PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 22, 17);
  PTAD::Ui::drawNumber(statusDifference[1], 23, 17, 100);
}

void PTAD::Menu::updateEquipMenu()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 5;
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 6)
        PTAD::globalCounter = 0;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::Game::player.equippableItems[PTAD::globalCounter] != 0)
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_select);
      state = State::EquipMenu_ChooseItem;
      tempValue = PTAD::globalCounter;
      if (PTAD::Game::player.equippedItems[tempValue] == 255)
      {
        PTAD::globalCounter = 0;
        while (((PTAD::Game::player.equippableItems[tempValue] >> PTAD::globalCounter) & 1) == 0)
          ++PTAD::globalCounter;
      }
      else
        PTAD::globalCounter = PTAD::Game::player.equippedItems[tempValue];
      drawEquipMenu_ChooseItem();
      return;
    }
    else
      PTAD::Music::playSFX(PTAD::Resources::sfx_invalid);
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cancel);
    PTAD::Game::state = PTAD::Game::previousState;
    return;
  }
  if (state == State::EquipMenu)
  {
    drawEquipMenu();
    PTAD::Ui::cursorX = -2;
    PTAD::Ui::cursorY = 24 * PTAD::globalCounter + 16;
  }
}

void PTAD::Menu::updateEquipMenu_ChooseItem()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250)
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 15;
      while (((PTAD::Game::player.equippableItems[tempValue] >> PTAD::globalCounter) & 1) == 0)
      {
        --PTAD::globalCounter;
        if (PTAD::globalCounter == -1)
          PTAD::globalCounter = 15;
      }
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250)
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 16)
        PTAD::globalCounter = 0;
      while (((PTAD::Game::player.equippableItems[tempValue] >> PTAD::globalCounter) & 1) == 0)
      {
        ++PTAD::globalCounter;
        if (PTAD::globalCounter == 16)
          PTAD::globalCounter = 0;
      }
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_select);
    if (PTAD::Game::player.equippedItems[tempValue] == PTAD::globalCounter)
      PTAD::Game::player.equippedItems[tempValue] = 255;
    else
      PTAD::Game::player.equippedItems[tempValue] = PTAD::globalCounter;
    PTAD::globalCounter = tempValue;
    state = State::EquipMenu;
    if (PTAD::Game::player.equippedItems[tempValue] != 255)
      PTAD::Ui::drawText(PTAD::Resources::items[tempValue + PTAD::Game::ITEM_TYPE_WEAPONS][PTAD::Game::player.equippedItems[tempValue]].name, 8, 2, tempValue * 3 + 2);
    else
      PTAD::Ui::drawText(unequipped, 8, 2, tempValue * 3 + 2);
    drawEquipMenu();
    return;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cancel);
    PTAD::globalCounter = tempValue;
    state = State::EquipMenu;
    drawEquipMenu();
    return;
  }
  if (state == State::EquipMenu_ChooseItem)
  {
    drawEquipMenu_ChooseItem();
    PTAD::Ui::cursorX = 78;
    PTAD::Ui::cursorY = PTAD::globalCounter * 8 + 16;
  }
}

void PTAD::Menu::updateBeastsMenu()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter += PTAD::Resources::numEnemies;
      if (PTAD::globalCounter < tempValue)
        tempValue = PTAD::globalCounter;
      else if (PTAD::globalCounter > tempValue + 18)
        tempValue = PTAD::globalCounter - 18;
      drawBeastsMenu();
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == PTAD::Resources::numEnemies)
        PTAD::globalCounter -= PTAD::Resources::numEnemies;
      if (PTAD::globalCounter < tempValue)
        tempValue = PTAD::globalCounter;
      else if (PTAD::globalCounter > tempValue + 18)
        tempValue = PTAD::globalCounter - 18;
      drawBeastsMenu();
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cancel);
    PTAD::Game::state = PTAD::Game::previousState;
    return;
  }
  PD::drawSpriteBitmap(91, 24, 48, 48, enemyImage);
  PTAD::Ui::cursorX = -2;
  PTAD::Ui::cursorY = (PTAD::globalCounter - tempValue) * 8 + 16;
}

void PTAD::Menu::updateNameEntryMenu()
{
  //TODO: A btn -> add character (buzz if max characters reached)
  //TODO: B btn -> remove character (buzz if name is empty)
  //TODO: C btn -> jump cursor to "ok" button
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      nameEntryClearCursor(PTAD::globalCounter);
      PTAD::globalCounter -= 12;
      if (PTAD::globalCounter < 0)
        PTAD::globalCounter += 60;
      nameEntryDrawCursor(PTAD::globalCounter);
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      nameEntryClearCursor(PTAD::globalCounter);
      PTAD::globalCounter += 12;
      if (PTAD::globalCounter >= 60)
        PTAD::globalCounter -= 60;
      nameEntryDrawCursor(PTAD::globalCounter);
    }
  }
  else if (PB::leftBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_LEFT))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      nameEntryClearCursor(PTAD::globalCounter);
      if (PTAD::globalCounter % 12 == 0)
        PTAD::globalCounter += 11;
      else
        --PTAD::globalCounter;
      nameEntryDrawCursor(PTAD::globalCounter);
    }
  }
  else if (PB::rightBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_RIGHT))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      nameEntryClearCursor(PTAD::globalCounter);
      if (PTAD::globalCounter % 12 == 11)
        PTAD::globalCounter = 0;
      else
        ++PTAD::globalCounter;
      nameEntryDrawCursor(PTAD::globalCounter);
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (PTAD::globalCounter == 59)
    {
      if (tempValue > 0)
      {
        PTAD::Game::player.playTime = 0;
        PTAD::Ui::dialogCursor = 0;
        PTAD::Game::state = PTAD::Game::State::MapInit;
        PTAD::Map::eventID = 0;
        PTAD::MapEvent::setup(DataPack::hash("/newGameEvent.dat"));
        PTAD::MapEvent::begin(0);
      }
      else
        PTAD::Music::playSFX(PTAD::Resources::sfx_invalid);
    }
    else if (tempValue < 8)
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_select);
      PTAD::Game::player.name[tempValue] = nameEntryChars[PTAD::globalCounter];
      PTAD::Ui::drawCharacter(nameEntryChars[PTAD::globalCounter], tempValue + 2, 2);
      PTAD::Ui::dialogCursorX += 8;
      ++tempValue;
    }
    else
      PTAD::Music::playSFX(PTAD::Resources::sfx_invalid);
  }
  else if (PB::bBtn())
  {
    if (tempValue == 0 && PTAD::justPressed(PTAD::BTN_MASK_B))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cancel);
      PTAD::Game::state = PTAD::Game::State::TitleInit;
    }
    else if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_B))
    {
      lastPress = PC::getTime();
      if (tempValue > 0)
      {
        PTAD::Music::playSFX(PTAD::Resources::sfx_cancel);
        --tempValue;
        PTAD::Game::player.name[tempValue] = PTAD::FONT_SPACE;
        PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, tempValue + 2, 2);
        PTAD::Ui::dialogCursorX -= 8;
      }
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_C) && PTAD::globalCounter != 59)
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
    nameEntryClearCursor(PTAD::globalCounter);
    PTAD::globalCounter = 59;
    nameEntryDrawCursor(PTAD::globalCounter);
  }
}

void PTAD::Menu::updateSaveLoadMenu()
{
  if (PB::upBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_UP))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      --PTAD::globalCounter;
      if (PTAD::globalCounter == -1)
        PTAD::globalCounter = 2;
    }
  }
  else if (PB::downBtn())
  {
    if (PC::getTime() - lastPress >= 250 || PTAD::justPressed(PTAD::BTN_MASK_DOWN))
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
      lastPress = PC::getTime();
      ++PTAD::globalCounter;
      if (PTAD::globalCounter == 3)
        PTAD::globalCounter = 0;
    }
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    if (state == State::LoadMenu)
    {
      if (PTAD::Ui::getCharacter(2, 2 + PTAD::globalCounter * 7) == PTAD::FONT_SPACE)
        PTAD::Music::playSFX(PTAD::Resources::sfx_invalid);
      else
      {
        PTAD::Music::playSFX(PTAD::Resources::sfx_select);
        PTAD::Game::loadGame(PTAD::globalCounter);
        PTAD::Game::state = PTAD::Game::State::MapInit;
        PTAD::Map::eventID = 0;
        PTAD::MapEvent::setup(DataPack::hash("/newGameEvent.dat"));
        PTAD::MapEvent::begin(0);
      }
    }
    else if (state == State::SaveMenu)
    {
      uint32_t s = PTAD::Game::player.playTime / 1000;
      uint32_t m = s / 60;
      uint32_t h = m / 60;
      s %= 60;
      m %= 60;
      if (h > 99)
      {
        h = 99;
        m = 99;
        s = 99;
      }
      PTAD::Music::playSFX(PTAD::Resources::sfx_select);
      PTAD::Game::saveGame(PTAD::globalCounter);
      drawSaveLoadMenu();
    }
    state = State::SaveMenuEnd;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cancel);
    PTAD::Game::state = PTAD::Game::previousState;
    PTAD::globalCounter = 0;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_C))
  {
    if (PTAD::Ui::getCharacter(2, 2 + PTAD::globalCounter * 7) == PTAD::FONT_SPACE)
      PTAD::Music::playSFX(PTAD::Resources::sfx_invalid);
    else
    {
      PTAD::Music::playSFX(PTAD::Resources::sfx_select);
      tempValue = PTAD::globalCounter;
      lastPress = (uint32_t)state;
      PTAD::globalCounter = 0;
      drawEraseSlot();
      state = State::EraseSlot;
    }
  }
  if (state == State::LoadMenu || state == State::SaveMenu)
  {
    PTAD::Ui::cursorX = -2;
    PTAD::Ui::cursorY = 8 + PTAD::globalCounter * 56;
  }
}

void PTAD::Menu::updateEraseSlot()
{
  if (PTAD::justPressed(PTAD::BTN_MASK_LEFT) || PTAD::justPressed(PTAD::BTN_MASK_RIGHT))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cursor);
    PTAD::globalCounter ^= 1;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_A))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_select);
    if (PTAD::globalCounter == 0)
      PTAD::Game::eraseGame(tempValue);
    state = (State)lastPress;
  }
  else if (PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Music::playSFX(PTAD::Resources::sfx_cancel);
    state = (State)lastPress;
  }
  if (state == State::EraseSlot)
  {
    PTAD::Ui::cursorX = 14 + PTAD::globalCounter * 40;
    PTAD::Ui::cursorY = 16 + tempValue * 56;
  }
  else
  {
    PTAD::globalCounter = tempValue;
    drawSaveLoadMenu();
  }
}

void PTAD::Menu::updateSaveMenuEnd()
{
  if (PTAD::justPressed(PTAD::BTN_MASK_A) || PTAD::justPressed(PTAD::BTN_MASK_B))
  {
    PTAD::Game::state = PTAD::Game::previousState;
    PTAD::globalCounter = 0;
  }
}

void PTAD::Menu::drawEquipMenu()
{
  drawItemNames(12, 2, PTAD::Game::ITEM_TYPE_WEAPONS + PTAD::globalCounter);
  drawEquipmentStats(PTAD::globalCounter);
  if (PTAD::Game::player.equippableItems[PTAD::globalCounter] == 0)
    PTAD::Ui::drawText(noneAvailable[PTAD::globalCounter], 24, 2, 19);
  else if (PTAD::Game::player.equippedItems[PTAD::globalCounter] != 255)
    PTAD::Ui::drawText(PTAD::Resources::items[PTAD::globalCounter + PTAD::Game::ITEM_TYPE_WEAPONS][PTAD::Game::player.equippedItems[PTAD::globalCounter]].description, 24, 2, 19);
  else
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
}

void PTAD::Menu::drawEquipMenu_ChooseItem()
{
  drawEquipmentStatsDifference(tempValue, PTAD::globalCounter);
  if (PTAD::globalCounter == PTAD::Game::player.equippedItems[tempValue])
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 24, 2, 19);
  else
    PTAD::Ui::drawText(PTAD::Resources::items[tempValue + PTAD::Game::ITEM_TYPE_WEAPONS][PTAD::globalCounter].description, 24, 2, 19);
}

void PTAD::Menu::drawBeastsMenu()
{
  if (((PTAD::Game::player.enemiesScanned[PTAD::globalCounter / 32] >> (PTAD::globalCounter % 32)) & 1) != 0)
  {
    DataPack::PackedFile file;
    PTAD::Battle::EnemyData enemyData;
    PTAD::dataFile->getPackedFile(DataPack::hash("/battlers.png"), &file);
    file.seek(PTAD::MEMORY_BATTLE_BATTLER_SIZE * PTAD::globalCounter);
    PTAD::dataFile->readBytes(&file, enemyImage, PTAD::MEMORY_BATTLE_BATTLER_SIZE);
    PTAD::dataFile->getPackedFile(PTAD::Resources::enemies[PTAD::globalCounter], &file);
    PTAD::dataFile->readBytes(&file, (void*)&enemyData, sizeof(PTAD::Battle::EnemyData));
    PTAD::Ui::drawNumber(enemyData.hp, 21, 4, 10000);
    PTAD::Ui::drawNumber(enemyData.mp, 23, 5, 100);
    PTAD::Ui::drawNumber(enemyData.experience, 22, 6, 1000);
    PTAD::Ui::drawNumber(enemyData.gold, 23, 7, 100);
    PTAD::Ui::drawNumber(enemyData.stats.attack, 23, 11, 100);
    PTAD::Ui::drawNumber(enemyData.stats.defense, 23, 12, 100);
    PTAD::Ui::drawNumber(enemyData.stats.agility, 23, 13, 100);
    PTAD::Ui::drawNumber(enemyData.stats.magic, 23, 14, 100);
    for (int i = 0; i < 7; ++i)
      PTAD::Ui::drawCharacter(PTAD::FONT_DMG_0 + ((enemyData.spellResistance >> (i * 2)) & 0x3), 13 + (i * 2), 16);
    PTAD::Ui::drawNumber(enemyData.statusResistance[0], 23, 18, 100);
    PTAD::Ui::drawNumber(enemyData.statusResistance[1], 23, 19, 100);
  }
  else
  {
    memset(enemyImage, 0, PTAD::MEMORY_BATTLE_BATTLER_SIZE);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 5, 21, 4);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 2, 24, 5);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 4, 22, 6);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 3, 23, 7);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 3, 23, 11);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 3, 23, 12);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 3, 23, 13);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 3, 23, 14);
    for (int i = 0; i < 7; ++i)
      PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 13 + (i * 2), 16);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 3, 23, 18);
    PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 3, 23, 19);
  }
  if (tempValue > 0)
    PTAD::Ui::drawCharacter(PTAD::FONT_ARROW_UP, 9, 1);
  if (tempValue + 18 < PTAD::Resources::numEnemies)
    PTAD::Ui::drawCharacter(PTAD::FONT_ARROW_DOWN, 9, 20);
  for (uint8_t i = 0; i < 18; ++i)
  {
    uint8_t id = i + tempValue;
    if (((PTAD::Game::player.enemiesScanned[id / 32] >> (i % 32)) & 1) != 0)
      PTAD::Ui::drawText(PTAD::Resources::enemyNames + id * 8, 8, 2, 2 + i);
    else if (id < PTAD::Resources::numEnemies)
      PTAD::Ui::fillCharacter(PTAD::FONT_MINUS, 8, 2, 2 + i);
    else
      PTAD::Ui::fillCharacter(PTAD::FONT_SPACE, 8, 2, 2 + i);
  }
}

void PTAD::Menu::drawNameEntryMenu()
{
  PTAD::Ui::fill(0, 0, PTAD::tilemapWidth - 1, PTAD::tilemapHeight - 1, PTAD::FONT_SPACE);
  PTAD::Ui::drawFrame(1, 1, 10, 3);
  PTAD::Ui::drawCharacter(PTAD::FONT_N, 2, 1);
  PTAD::Ui::drawCharacter(PTAD::FONT_a, 3, 1);
  PTAD::Ui::drawCharacter(PTAD::FONT_m, 4, 1);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 5, 1);
  PTAD::Ui::fixFrameHeader(1, 10, 1);
  PTAD::Ui::drawFrame(1, 7, 3, 9);
  for (uint8_t i = 0; i < 60; ++i)
    PTAD::Ui::drawCharacter(nameEntryChars[i], (i % 12) * 2 + ((i % 12) / 6) + 2, (i / 12) * 2 + 8);
  PTAD::Ui::dialogCursorX = 15;
  PTAD::Ui::dialogCursorY = 17;
  PTAD::Ui::dialogCursor = PTAD::Ui::DIALOG_CURSOR_BLINK;
  for (tempValue = 0; tempValue < 8; ++tempValue)
  {
    if (PTAD::Game::player.name[tempValue] == PTAD::FONT_SPACE)
      break;
    PTAD::Ui::drawCharacter(PTAD::Game::player.name[tempValue], tempValue + 2, 2);
    PTAD::Ui::dialogCursorX += 8;
  }
}

void PTAD::Menu::drawSaveLoadMenu()
{
  PTAD::Game::SaveData saveData = PTAD::Game::getSaveData();
  PTAD::Ui::fill(0, 0, 27, 21, PTAD::FONT_SPACE);
  for (int i = 0; i < 3; ++i)
  {
    int y = i * 7;
    uint32_t s = saveData.slots[i].playTime / 1000;
    uint32_t m = s / 60;
    uint32_t h = m / 60;
    s %= 60;
    m %= 60;
    if (h > 99)
    {
      h = 99;
      m = 99;
      s = 99;
    }
    PTAD::Ui::drawFrame(1, 1 + i * 7, 26, 6 + y);
    PTAD::Ui::drawCharacter(PTAD::FONT_S, 2, 1 + y);
    PTAD::Ui::drawCharacter(PTAD::FONT_l, 3, 1 + y);
    PTAD::Ui::drawCharacter(PTAD::FONT_o, 4, 1 + y);
    PTAD::Ui::drawCharacter(PTAD::FONT_t, 5, 1 + y);
    PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 6, 1 + y);
    PTAD::Ui::drawCharacter(PTAD::FONT_1 + i, 7, 1 + y);
    PTAD::Ui::fixFrameHeader(1, 8, 1 + i * 7);
    if (saveData.slots[i].level > 0)
    {
      PTAD::Ui::drawText(saveData.slots[i].name, 8, 2, 2 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_L, 16, 2 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_e, 17, 2 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_v, 18, 2 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_e, 19, 2 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_l, 20, 2 + y);
      PTAD::Ui::drawNumber(saveData.slots[i].level, 24, 2 + y, 10);
      PTAD::Ui::drawCharacter(PTAD::FONT_H, 2, 3 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_P, 3, 3 + y);
      PTAD::Ui::drawNumber(saveData.slots[i].hp, 6, 3 + y, 1000);
      PTAD::Ui::drawCharacter(PTAD::FONT_SLASH, 10, 3 + y);
      PTAD::Ui::drawNumber(saveData.slots[i].maxHP, 11, 3 + y, 1000);
      PTAD::Ui::drawCharacter(PTAD::FONT_M, 16, 3 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_P, 17, 3 + y);
      PTAD::Ui::drawNumber(saveData.slots[i].mp, 19, 3 + y, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_SLASH, 22, 3 + y);
      PTAD::Ui::drawNumber(saveData.slots[i].maxMP, 23, 3 + y, 100);
      PTAD::Ui::drawCharacter(PTAD::FONT_E, 2, 4 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_x, 3, 4 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_p, 4, 4 + y);
      PTAD::Ui::drawNumber(saveData.slots[i].nextLevel - saveData.slots[i].experience, 6, 4 + y, 1000);
      PTAD::Ui::drawCharacter(PTAD::FONT_G, 16, 4 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_o, 17, 4 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_l, 18, 4 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_d, 19, 4 + y);
      PTAD::Ui::drawNumber(saveData.slots[i].gold, 21, 4 + y, 10000);
      PTAD::Ui::drawCharacter(PTAD::FONT_P, 2, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_l, 3, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_a, 4, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_y, 5, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_T, 7, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_i, 8, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_m, 9, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_e, 10, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_0 + (h / 10) % 10, 18, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_0 + h % 10, 19, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 20, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_0 + (m / 10) % 10, 21, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_0 + m % 10, 22, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_MINUS, 23, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_0 + (s / 10) % 10, 24, 5 + y);
      PTAD::Ui::drawCharacter(PTAD::FONT_0 + s % 10, 25, 5 + y);
    }
    else
      PTAD::Ui::drawText(emptySlotText, 10, 9, 3 + y);
  }
}

void PTAD::Menu::drawEraseSlot()
{
  uint8_t y = tempValue * 7;
  PTAD::Ui::drawFrame(2, 1 + y, 13, 3 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_E, 3, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_r, 4, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_a, 5, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_s, 6, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 7, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_SPACE, 8, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_S, 9, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_l, 10, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_o, 11, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_t, 12, 1 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_Y, 4, 2 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_e, 5, 2 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_s, 6, 2 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_N, 9, 2 + y);
  PTAD::Ui::drawCharacter(PTAD::FONT_o, 10, 2 + y);
  PTAD::Ui::fixFrameHeader(2, 13, 1 + y);
}

void PTAD::Menu::nameEntryClearCursor(int loc)
{
  PTAD::Ui::fill(((loc % 12) * 2) + ((loc % 12) / 6) + 1, (loc / 12) * 2 + 7, ((loc % 12) * 2) + ((loc % 12) / 6) + 3, (loc / 12) * 2 + 9, PTAD::FONT_SPACE);
  PTAD::Ui::drawCharacter(nameEntryChars[loc], ((loc % 12) * 2) + ((loc % 12) / 6) + 2, (loc / 12) * 2 + 8);
}

void PTAD::Menu::nameEntryDrawCursor(int loc)
{
  PTAD::Ui::drawFrame(((loc % 12) * 2) + ((loc % 12) / 6) + 1, (loc / 12) * 2 + 7, ((loc % 12) * 2) + ((loc % 12) / 6) + 3, (loc / 12) * 2 + 9);
  PTAD::Ui::drawCharacter(nameEntryChars[loc], ((loc % 12) * 2) + ((loc % 12) / 6) + 2, (loc / 12) * 2 + 8);
}
