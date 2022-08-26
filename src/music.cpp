#include "headers.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

Audio::RAWFileSource *PTAD::Music::backgroundMusic[2] = {nullptr, nullptr};
Audio::RAWFileSource *PTAD::Music::sfxSource = nullptr;
Audio::Sink<3, PROJ_AUD_FREQ> PTAD::Music::audio;
const uint32_t PTAD::Music::music[] =
{
	DataPack::hash("music/JRPG_battleBoss_loop.raw"),    //0x00 ( 0)
	DataPack::hash("music/JRPG_battleFinal.raw"),        //0x01 ( 1)
	DataPack::hash("music/JRPG_battle_loop.raw"),        //0x02 ( 2)
	DataPack::hash("music/JRPG_characterRevived.raw"),   //0x03 ( 3)
	DataPack::hash("music/JRPG_discovery.raw"),          //0x04 ( 4)
	DataPack::hash("music/JRPG_docks_loop.raw"),         //0x05 ( 5)
	DataPack::hash("music/JRPG_dungeon_loop.raw"),       //0x06 ( 6)
	DataPack::hash("music/JRPG_fields_loop.raw"),        //0x07 ( 7)
	DataPack::hash("music/JRPG_gameOver.raw"),           //0x08 ( 8)
	DataPack::hash("music/JRPG_goodMorning.raw"),        //0x09 ( 9)
	DataPack::hash("music/JRPG_goodNight.raw"),          //0x0A (10)
	DataPack::hash("music/JRPG_inn_loop.raw"),           //0x0B (11)
	DataPack::hash("music/JRPG_joinParty.raw"),          //0x0C (12)
	DataPack::hash("music/JRPG_labyrinth_loop.raw"),     //0x0D (13)
	DataPack::hash("music/JRPG_levelUp.raw"),            //0x0E (14)
	DataPack::hash("music/JRPG_machineRoom_loop.raw"),   //0x0F (15)
	DataPack::hash("music/JRPG_mainTheme.raw"),          //0x10 (16)
	DataPack::hash("music/JRPG_mysticIsle.raw"),         //0x11 (17)
	DataPack::hash("music/JRPG_princess.raw"),           //0x12 (18)
	DataPack::hash("music/JRPG_royalCourt_loop.raw"),    //0x13 (19)
	DataPack::hash("music/JRPG_shop_loop.raw"),          //0x14 (20)
	DataPack::hash("music/JRPG_tavern.raw"),             //0x15 (21)
	DataPack::hash("music/JRPG_temple.raw"),             //0x16 (22)
	DataPack::hash("music/JRPG_town_loop.raw"),          //0x17 (23)
	DataPack::hash("music/JRPG_winBattleBig.raw"),       //0x18 (24)
	DataPack::hash("music/JRPG_winBattleBoss.raw"),      //0x19 (25)
	DataPack::hash("music/JRPG_winBattleFinal.raw"),     //0x1A (26)
	DataPack::hash("music/JRPG_winBattle.raw")           //0x1B (27)
};
const uint32_t PTAD::Music::sfx[]
{
  DataPack::hash("sfx/hit.raw"),     //0x00 ( 0)
  DataPack::hash("sfx/miss.raw"),    //0x01 ( 1)
  DataPack::hash("sfx/poison.raw"),  //0x02 ( 2)
  DataPack::hash("sfx/slow.raw"),    //0x03 ( 3)
  DataPack::hash("sfx/getItem.raw"), //0x04 ( 4)
  DataPack::hash("sfx/cancel.raw"),  //0x05 ( 5)
  DataPack::hash("sfx/invalid.raw"), //0x06 ( 6)
  DataPack::hash("sfx/select.raw"),  //0x07 ( 7)
  DataPack::hash("sfx/useitem.raw"), //0x08 ( 8)
  DataPack::hash("sfx/cursor.raw"),  //0x09 ( 9)
  DataPack::hash("sfx/heal.raw"),    //0x0A (10)
  DataPack::hash("sfx/run.raw"),     //0x0B (11)
  DataPack::hash("sfx/quake.raw"),   //0x0C (12)
  DataPack::hash("sfx/dialog.raw"),  //0x0D (13)
  DataPack::hash("sfx/coin.raw"),    //0x0E (14)
  DataPack::hash("sfx/berserk.raw"), //0x0F (15)
  DataPack::hash("sfx/rune.raw"),    //0x10 (16)
  DataPack::hash("sfx/dream.raw"),   //0x11 (17)
  DataPack::hash("sfx/gate.raw"),    //0x12 (18)
  DataPack::hash("sfx/claw.raw"),    //0x13 (19)
  DataPack::hash("sfx/droplet.raw"), //0x14 (20)
  DataPack::hash("sfx/punch.raw"),   //0x15 (21)
  DataPack::hash("sfx/sword.raw"),   //0x16 (22)
};
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
      PTAD::dataFile->getPackedFile(music[bgm], &file);
      PTAD::dataFile->readyRead(file);
      backgroundMusic[0] = &Audio::play<0>(PTAD::dataFile->getFile(), file.size());
    }
    currentBGM = bgm;
  }
  else
  {
    if (backgroundMusic[0] != nullptr)
      backgroundMusic[0]->setPaused(true);
    PTAD::dataFile->getPackedFile(music[bgm], &file);
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
  PTAD::dataFile->getPackedFile(sfx[sfxID & 0x7F], &file);
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
