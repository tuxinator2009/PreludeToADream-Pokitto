#pragma once

#include "headers.h"

namespace PTAD
{
	class Music
	{
		public:
			static void playMusic(uint8_t bgm, bool loop=true);
			static void pauseMusic();
			static void resumeMusic();
			static void stopMusic();
      static void playSFX(uint8_t sfxID);
      static void stopSFX();
      static constexpr uint8_t MUSIC_BOSS_BATTLE = 0;
      static constexpr uint8_t MUSIC_FINAL_BATTLE = 1;
      static constexpr uint8_t MUSIC_BATTLE = 2;
      static constexpr uint8_t MUSIC_REVIVED = 3;
      static constexpr uint8_t MUSIC_DISCOVERY = 4;
      static constexpr uint8_t MUSIC_DOCKS = 5;
      static constexpr uint8_t MUSIC_DUNGEON = 6;
      static constexpr uint8_t MUSIC_FIELDS = 7;
      static constexpr uint8_t MUSIC_GAME_OVER = 8;
      static constexpr uint8_t MUSIC_GOOD_MORNING = 9;
      static constexpr uint8_t MUSIC_GOOD_NIGHT = 10;
      static constexpr uint8_t MUSIC_INN = 11;
      static constexpr uint8_t MUSIC_JOIN = 12;
      static constexpr uint8_t MUSIC_LABYRINTH = 13;
      static constexpr uint8_t MUSIC_LEVEL_UP = 14;
      static constexpr uint8_t MUSIC_MACHINE_ROOM = 15;
      static constexpr uint8_t MUSIC_MAIN_THEME = 16;
      static constexpr uint8_t MUSIC_MYSTIC_ISLE = 17;
      static constexpr uint8_t MUSIC_PRINCESS = 18;
      static constexpr uint8_t MUSIC_ROYAL_COURT = 19;
      static constexpr uint8_t MUSIC_SHOP = 20;
      static constexpr uint8_t MUSIC_TAVERN = 21;
      static constexpr uint8_t MUSIC_TEMPLE = 22;
      static constexpr uint8_t MUSIC_TOWN = 23;
      static constexpr uint8_t MUSIC_WIN_BATTLE_BIG = 24;
      static constexpr uint8_t MUSIC_WIN_BATTLE_BOSS = 25;
      static constexpr uint8_t MUSIC_WIN_BATTLE_FINAL = 26;
      static constexpr uint8_t MUSIC_WIN_BATTLE = 27;
      static constexpr uint8_t SFX_HIT = 0;
      static constexpr uint8_t SFX_MISS = 1;
      static constexpr uint8_t SFX_POISON = 2;
      static constexpr uint8_t SFX_SLOW = 3;
      static constexpr uint8_t SFX_GETITEM = 4;
      static constexpr uint8_t SFX_CANCEL = 5;
      static constexpr uint8_t SFX_INVALID = 6;
      static constexpr uint8_t SFX_SELECT = 7;
      static constexpr uint8_t SFX_USEITEM = 8;
      static constexpr uint8_t SFX_CURSOR = 9;
      static constexpr uint8_t SFX_HEAL = 10;
      static constexpr uint8_t SFX_RUN = 11;
      static constexpr uint8_t SFX_QUAKE = 12;
      static constexpr uint8_t SFX_DIALOG = 13;
      static constexpr uint8_t SFX_COIN = 14;
      static constexpr uint8_t SFX_BERSERK = 15;
		private:
			static Audio::RAWFileSource *backgroundMusic;
      static Audio::RAWFileSource *sfxSource;
			static Audio::Sink<2, PROJ_AUD_FREQ> audio;
			static const uint32_t music[];
      static const uint32_t sfx[];
      static uint8_t currentBGM;
	};
}
