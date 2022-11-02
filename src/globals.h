#pragma once

#include "headers.h"

namespace PTAD
{
	bool justPressed(uint8_t mask);
	void loadTileset(uint8_t tilesetID);
	void drawTile(uint8_t x, uint8_t y, uint8_t tileID, bool bg=true);
  void drawTiles(const uint8_t *array, int numTiles, int x, int y, bool bg=true);
  void clearTiles(int32_t screenX, int32_t screenY);
  void tileFillerBG(uint8_t *line, uint32_t y, bool skip) noexcept;
  void tileFillerFG(uint8_t *line, uint32_t y, bool skip) noexcept;
  void setup_data_16(uint16_t data);
  void write_command_16(uint16_t data);
  void write_data_16(uint16_t data);
  void fillRegion(uint32_t count, uint16_t color);
  constexpr bool stringsEqual(const char *string1, const char *string2)
  {
    while (*string1 && *string2)
    {
      if (*string1++ != *string2++)
        return false;
    }
    return true;
  }
  constexpr int stringToID(const char *value, const char **lookupTable, int numValues)
  {
    for (int i = 0; i < numValues; ++i)
    {
      if (stringsEqual(value, lookupTable[i]))
        return i;
    }
    //assert(false, value);
  }
  struct Item
  {
    uint8_t name[8];
    uint8_t description[24];
    uint16_t price;
  };
  struct EquipmentStats
  {
    int8_t stats[4];
    uint16_t spellResistance;
    int8_t statusResistance[2];
  };
  extern DataPack *dataFile;
  extern uint32_t frameCount;
  extern uint32_t fpsStart;
  extern uint32_t fps;
	extern int32_t globalCounter;
	extern uint8_t buttonsPreviousState;
	extern uint8_t buttonsJustPressed;
	extern uint8_t battleMonsterID;
	extern uint8_t battleBG;
	extern uint8_t battleBGM;
  extern uint8_t darkness;
	extern uint8_t __attribute__((section (".bss"))) __attribute__ ((aligned)) memory[];
	extern uint8_t *tiles;
	#ifndef POK_SIM
	extern uint8_t *tilemapBG;
  extern uint8_t *tilemapFG;
	#else
	extern uint8_t tilemapBG[];
  extern uint8_t tilemapFG[];
	#endif
	extern const uint32_t tilesets[];
	extern const uint8_t playerSprite[];
	extern const uint8_t cursor[];
  extern const uint8_t statusIcons[];
  extern const Item items[8][16];
  extern const Item skills[16];
  extern const Item spells[16];
  extern const EquipmentStats equipmentStats[6][16];
  
  constexpr uint8_t ITEMS_BATTLE_ONLY = 7;
	
	constexpr uint32_t screenWidth = PROJ_LCDWIDTH;
	constexpr uint32_t screenHeight = PROJ_LCDHEIGHT;
	
	constexpr uint32_t tilemapWidth = PROJ_LCDWIDTH / POK_TILE_W + 2;
	constexpr uint32_t tilemapHeight = PROJ_LCDHEIGHT / POK_TILE_H + 2;
	
	constexpr uint32_t TILEMAP_MEMORY_SIZE = tilemapWidth * tilemapHeight;
	constexpr uint32_t UIMAP_MEMORY_SIZE = tilemapWidth * tilemapHeight;
	constexpr uint32_t HIGH_RAM1 = 0x20000000;
	constexpr uint32_t HIGH_RAM2 = 0x20004000;
	constexpr uint32_t TILEMAPBG_MEMORY_ADDRESS = HIGH_RAM1;
  constexpr uint32_t TILEMAPFG_MEMORY_ADDRESS = HIGH_RAM1 + TILEMAP_MEMORY_SIZE;
  
  constexpr uint32_t EVENT_BUFFER_MEMORY_ADDRESS = HIGH_RAM1 + TILEMAP_MEMORY_SIZE * 2;
  constexpr uint32_t EVENT_BUFFER_MEMORY_SIZE = 512;
  
  constexpr uint32_t BATTLE_ANIMATION_MEMORY_ADDRESS = HIGH_RAM2;
  constexpr uint32_t BATTLE_ANIMATION_SIZE = sizeof(PTAD::BattleAnimation::AnimationData);
  
  constexpr uint32_t MAP_PASSABILITY_MEMORY_ADDRESS = HIGH_RAM2;
  constexpr uint32_t MAP_PASSABILITY_SIZE = 2048;
	
	constexpr uint8_t BTN_MASK_UP = 1 << UPBIT;
	constexpr uint8_t BTN_MASK_DOWN = 1 << DOWNBIT;
	constexpr uint8_t BTN_MASK_LEFT = 1 << LEFTBIT;
	constexpr uint8_t BTN_MASK_RIGHT = 1 << RIGHTBIT;
	constexpr uint8_t BTN_MASK_A = 1 << ABIT;
	constexpr uint8_t BTN_MASK_B = 1 << BBIT;
	constexpr uint8_t BTN_MASK_C = 1 << CBIT;
	
	constexpr uint8_t DIR_UP = 0;
	constexpr uint8_t DIR_RIGHT = 1;
	constexpr uint8_t DIR_DOWN = 2;
	constexpr uint8_t DIR_LEFT = 3;
  
  constexpr uint8_t ITEM_POTION = 0;
  constexpr uint8_t ITEM_H_POTION = 1;
  constexpr uint8_t ITEM_S_POTION = 2;
  constexpr uint8_t ITEM_ETHER = 3;
  constexpr uint8_t ITEM_H_ETHER = 4;
  constexpr uint8_t ITEM_S_ETHER = 5;
  constexpr uint8_t ITEM_ELIXIR = 6;
  constexpr uint8_t ITEM_ANTIDOTE = 7;
  constexpr uint8_t ITEM_STIMULNT = 8;
  constexpr uint8_t ITEM_HERBS = 9;
  constexpr uint8_t ITEM_REMEDY = 10;
  constexpr uint8_t ITEM_X_ATTACK = 11;
  constexpr uint8_t ITEM_X_DEFEND = 12;
  constexpr uint8_t ITEM_X_SPEED = 13;
  constexpr uint8_t ITEM_X_MAGIC = 14;
  constexpr uint8_t ITEM_SMOKE_B = 15;
  
  constexpr uint8_t SKILL_EXAMINE = 0;
  constexpr uint8_t SKILL_FOCUS_1 = 1;
  constexpr uint8_t SKILL_FOCUS_2 = 2;
  constexpr uint8_t SKILL_FOCUS_3 = 3;
  constexpr uint8_t SKILL_DBL_HIT = 4;
  constexpr uint8_t SKILL_PWR_SHOT = 5;
  constexpr uint8_t SKILL_DASH = 6;
  constexpr uint8_t SKILL_ENRAGE_1 = 7;
  constexpr uint8_t SKILL_ENRAGE_2 = 8;
  constexpr uint8_t SKILL_ENRAGE_3 = 9;
  constexpr uint8_t SKILL_MEDITATE = 10;
  constexpr uint8_t SKILL_UNKNOWN1 = 11;
  constexpr uint8_t SKILL_UNKNOWN2 = 12;
  constexpr uint8_t SKILL_UNKNOWN3 = 13;
  constexpr uint8_t SKILL_UNKNOWN4 = 14;
  constexpr uint8_t SKILL_UNKNOWN5 = 15;
  
  constexpr uint8_t SPELL_FIREBALL = 0;
  constexpr uint8_t SPELL_STARFIRE = 1;
  constexpr uint8_t SPELL_WATERJET = 2;
  constexpr uint8_t SPELL_TSUNAMI = 3;
  constexpr uint8_t SPELL_QUICKSND = 4;
  constexpr uint8_t SPELL_UPHEAVAL = 5;
  constexpr uint8_t SPELL_FLURRY = 6;
  constexpr uint8_t SPELL_TORNADO = 7;
  constexpr uint8_t SPELL_ILLUMINE = 8;
  constexpr uint8_t SPELL_STARBEAM = 9;
  constexpr uint8_t SPELL_SHADOW = 10;
  constexpr uint8_t SPELL_ECLIPSE = 11;
  constexpr uint8_t SPELL_ENVENOM = 12;
  constexpr uint8_t SPELL_ENSNARE = 13;
  constexpr uint8_t SPELL_RECOVERY = 14;
  constexpr uint8_t SPELL_CLEANSER = 15;
	
	static constexpr uint32_t MEMORY_TILESET_SIZE = 8192;
  static constexpr uint32_t MEMORY_TITLE_WAVE_BUFFER_SIZE = 220*45;
  static constexpr uint32_t MEMORY_TITLE_MENU_BUFFER_SIZE = 220*36;
  static constexpr uint32_t MEMORY_TITLE_WAVE_SIZE = 45;
	static constexpr uint32_t MEMORY_BATTLE_BACKGROUND_SIZE = 6144;
	static constexpr uint32_t MEMORY_BATTLE_BATTLER_SIZE = 2304;
  static constexpr uint32_t MEMORY_BATTLE_PLAYER_SIZE = 256;
  static constexpr uint32_t MEMORY_BATTLE_ENEMY_SIZE = sizeof(PTAD::Battle::EnemyData); //currently 36
  static constexpr uint32_t MEMORY_BATTLE_ANIMATIONS_SIZE = 8192;
	static constexpr uint32_t MEMORY_MAP_DATA_SIZE = sizeof(PTAD::Map::MapData); //currently 260
	static constexpr uint32_t MEMORY_MAP_CHUNKS_SIZE = 4096;
	static constexpr uint32_t MEMORY_MAP_SPRITES_SIZE = 6144;
	static constexpr uint32_t MEMORY_MAP_TILEDATA_SIZE = 256;
  static constexpr uint32_t MEMORY_TITLE_SIZE = MEMORY_TITLE_WAVE_BUFFER_SIZE + MEMORY_TITLE_MENU_BUFFER_SIZE + MEMORY_TITLE_WAVE_SIZE;
  static constexpr uint32_t MEMORY_BATTLE_SIZE = MEMORY_BATTLE_BACKGROUND_SIZE + MEMORY_BATTLE_BATTLER_SIZE + MEMORY_BATTLE_PLAYER_SIZE + MEMORY_BATTLE_ENEMY_SIZE + MEMORY_BATTLE_ANIMATIONS_SIZE;
  static constexpr uint32_t MEMORY_MAP_SIZE = MEMORY_MAP_DATA_SIZE + MEMORY_MAP_CHUNKS_SIZE + MEMORY_MAP_CHUNKS_SIZE + MEMORY_MAP_SPRITES_SIZE + MEMORY_MAP_TILEDATA_SIZE;
	static constexpr uint32_t MEMORY_SIZE = std::max(MEMORY_TITLE_SIZE, MEMORY_TILESET_SIZE + std::max(MEMORY_BATTLE_SIZE, MEMORY_MAP_SIZE));
	
  static constexpr uint32_t MEMORY_TITLE_WAVE_BUFFER_OFFSET = 0;
  static constexpr uint32_t MEMORY_TITLE_MENU_BUFFER_OFFSET = MEMORY_TITLE_WAVE_BUFFER_SIZE;
  static constexpr uint32_t MEMORY_TITLE_WAVE_OFFSET = MEMORY_TITLE_MENU_BUFFER_OFFSET + MEMORY_TITLE_MENU_BUFFER_SIZE;
	static constexpr uint32_t MEMORY_TILESET_OFFSET = 0;
	static constexpr uint32_t MEMORY_BATTLE_BACKGROUND_OFFSET = MEMORY_TILESET_OFFSET + MEMORY_TILESET_SIZE;
	static constexpr uint32_t MEMORY_BATTLE_BATTLER_OFFSET = MEMORY_BATTLE_BACKGROUND_OFFSET + MEMORY_BATTLE_BACKGROUND_SIZE;
  static constexpr uint32_t MEMORY_BATTLE_PLAYER_OFFSET = MEMORY_BATTLE_BATTLER_OFFSET + MEMORY_BATTLE_BATTLER_SIZE;
  static constexpr uint32_t MEMORY_BATTLE_ENEMY_OFFSET = MEMORY_BATTLE_PLAYER_OFFSET + MEMORY_BATTLE_PLAYER_SIZE;
  static constexpr uint32_t MEMORY_BATTLE_ANIMATIONS_OFFSET = MEMORY_BATTLE_ENEMY_OFFSET + MEMORY_BATTLE_ENEMY_SIZE;
  static constexpr uint32_t MEMORY_MAP_CHUNKSBG_OFFSET = MEMORY_TILESET_OFFSET + MEMORY_TILESET_SIZE;
  static constexpr uint32_t MEMORY_MAP_CHUNKSFG_OFFSET = MEMORY_MAP_CHUNKSBG_OFFSET + MEMORY_MAP_CHUNKS_SIZE;
	static constexpr uint32_t MEMORY_MAP_SPRITES_OFFSET = MEMORY_MAP_CHUNKSFG_OFFSET + MEMORY_MAP_CHUNKS_SIZE;
  static constexpr uint32_t MEMORY_MAP_TILEDATA_OFFSET = MEMORY_MAP_SPRITES_OFFSET + MEMORY_MAP_SPRITES_SIZE;
  static constexpr uint32_t MEMORY_MAP_DATA_OFFSET = MEMORY_MAP_TILEDATA_OFFSET + MEMORY_MAP_TILEDATA_SIZE;
  static constexpr uint32_t MEMORY_MENU_BATTLER_OFFSET = MEMORY_MAP_CHUNKSBG_OFFSET;
  
  static constexpr uint16_t colorMask[] = {0x7BEF,0x39E7,0x18E3,0x0861,0x0020,0x0000};
}
