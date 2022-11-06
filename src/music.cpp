#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

Audio::RAWFileSource *PTAD::Music::backgroundMusic[2] = {nullptr, nullptr};
Audio::RAWFileSource *PTAD::Music::sfxSource = nullptr;
Audio::Sink<3, PROJ_AUD_FREQ> PTAD::Music::audio;
uint8_t PTAD::Music::currentBGM = 255;
uint8_t PTAD::Music::currentChannel = 0;

void PTAD::Music::playMusic(uint8_t bgm, uint8_t channel, bool loop)
{
	DataPack::PackedFile file;
  if (channel == 0)
  {
    Audio::stop<1>();
    if (bgm != currentBGM)
    {
      PTAD::dataFile->getPackedFile(PTAD::Resources::music[bgm], &file);
      PTAD::dataFile->readyRead(file);
      backgroundMusic[0] = &Audio::play<0>(PTAD::dataFile->getFile(), file.size());
    }
    currentBGM = bgm;
  }
  else
  {
    if (backgroundMusic[0] != nullptr)
      backgroundMusic[0]->setPaused(true);
    PTAD::dataFile->getPackedFile(PTAD::Resources::music[bgm], &file);
    PTAD::dataFile->readyRead(file);
    backgroundMusic[1] = &Audio::play<1>(PTAD::dataFile->getFile(), file.size());
  }
  backgroundMusic[channel]->setPaused(false);
  backgroundMusic[channel]->setLoop(loop);
  currentChannel = channel;
}

void PTAD::Music::pauseMusic()
{
  if (backgroundMusic[0] != nullptr)
    backgroundMusic[0]->setPaused(true);
  if (backgroundMusic[1] != nullptr)
    backgroundMusic[1]->setPaused(true);
}

void PTAD::Music::resumeMusic()
{
  if (backgroundMusic[currentChannel] != nullptr)
    backgroundMusic[currentChannel]->setPaused(false);
}

void PTAD::Music::stopMusic()
{
	Audio::stop();
}

void PTAD::Music::playSFX(uint8_t sfxID, bool priority)
{
  if (isSFXPlaying() && !priority)
    return;
  DataPack::PackedFile file;
  Audio::stop<2>();
  PTAD::dataFile->getPackedFile(PTAD::Resources::sfx[sfxID & 0x7F], &file);
  PTAD::dataFile->readyRead(file);
  sfxSource = &Audio::play<2>(PTAD::dataFile->getFile(), file.size());
  sfxSource->setLoop(sfxID & 0x80);
}

void PTAD::Music::stopSFX()
{
  Audio::stop<2>();
}

bool PTAD::Music::isMusicPlaying()
{
  if (backgroundMusic[currentChannel] == nullptr)
    return false;
  return !backgroundMusic[currentChannel]->ended();
}

bool PTAD::Music::isSFXPlaying()
{
  if (sfxSource == nullptr)
    return false;
  return !sfxSource->ended();
}
