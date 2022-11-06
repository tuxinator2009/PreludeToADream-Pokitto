#pragma once

#include "headers.h"

namespace PTAD
{
	class Music
	{
		public:
			static void playMusic(uint8_t bgm, uint8_t channel, bool loop=true);
			static void pauseMusic();
			static void resumeMusic();
			static void stopMusic();
      static void playSFX(uint8_t sfxID, bool priority=true);
      static void stopSFX();
      static bool isMusicPlaying();
      static bool isSFXPlaying();
		private:
			static Audio::RAWFileSource *backgroundMusic[2];
      static Audio::RAWFileSource *sfxSource;
			static Audio::Sink<3, PROJ_AUD_FREQ> audio;
      static uint8_t currentBGM;
      static uint8_t currentChannel;
	};
}
