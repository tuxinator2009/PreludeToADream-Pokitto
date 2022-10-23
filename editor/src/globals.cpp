/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2022 Justin (tuxinator2009) Davis                                *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

#include <QAudioBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QDir>
#include <QMessageBox>
#include <QList>
#include <QProgressBar>
#include <QString>
#include "globals.h"
#include "image.h"
#include "map.h"
#include "mainwindow.h"
#include "tileset.h"

QFileSystemWatcher *Globals::watchdog;
QMediaPlayer *Globals::audio;
QList<Tileset*> Globals::tilesets;
MainWindow *Globals::mainwindow = nullptr;
Map *Globals::map = nullptr;
Image *Globals::animations;
Image *Globals::battlers;
Image *Globals::cursor;
Image *Globals::font;
Image *Globals::player;
Image *Globals::sprites;
QString Globals::datadir;
QMap<int, QString> Globals::mapNames;
QMap<QString, QString> Globals::messages;
QStringList Globals::backdrops;
QStringList Globals::bgms;
QStringList Globals::sfx;
const uint8_t Globals::codes[] =
{
  '!',',','.','/','\'','?',201,'=',187,199,' ',182,200,207,188,'\n',
  '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',
  'G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V',
  'W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l',
  'm','n','o','p','q','r','s','t','u','v','w','x','y','z','-','+',
  '<','>',';',':','"','[',']','{','}','|','@','#','$','%','^','&',
  '*','(',')','`','~',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
};
const char *Globals::items[8][16] =
{
  {//consumables
    "Potion","H-Potion","S-Potion","Ether","H-Ether","S-Ether" ,"Elixir","Antidote","Stimulnt","Herbs","Remedy","X-Attack","X-Defend","X-Speed","X-Magic" ,"Smoke B"
  },
  {//tradeables
    "Crystal","Obsidian","","","","","","","","","","","","","",""
  },
  {//weapons
    "Rusty S","Dagger","","","","","","","","","","","","","",""
  },
  {//helmets
    "Beret","Miner's","","","","","","","","","","","","","",""
  },
  {//armor
    "Jerkin","","","","","","","","","","","","","","",""
  },
  {//boots
    "Moccasin","Galoshes","","","","","","","","","","","","","",""
  },
  {//rings
    "","","","","","","","","","","","","","","",""
  },
  {//amulets
    "","","","","","","","","","","","","","","",""
  }
};
const char *Globals::skills[16] =
{
  "Examine",
  "Focus 1",
  "Focus 2",
  "Focus 3",
  "Dbl Hit",
  "Pwr Shot",
  "Dash",
  "Enrage 1",
  "Enrage 2",
  "Enrage 3",
  "Meditate",
  "",
  "",
  "",
  "",
  ""
};
const char *Globals::spells[16] =
{
  "Fireball",
  "Starfire",
  "Waterjet",
  "Tsunami",
  "Quicksnd",
  "Upheaval",
  "Flurry",
  "Tornado",
  "Illumine",
  "Starbeam",
  "Shadow",
  "Eclipse",
  "Envenom",
  "Ensnare",
  "Recovery",
  "Cleanser"
};

void Globals::setupData()
{
  watchdog = new QFileSystemWatcher;
  audio = new QMediaPlayer();
  audio->setVolume(100);
  animations = new Image(datadir + "/animations/animations.png");
  battlers = new Image(datadir + "/battlers/battlers.png");
  cursor = new Image(datadir + "/cursor.png");
  font = new Image(datadir + "/font.png");
  font->recolor(1, 7);
  player = new Image(datadir + "/sprites/player.png");
  sprites = new Image(datadir + "/sprites/sprites.png");
  refreshBackdrops();
  refreshMusic();
  refreshSFX();
  loadTilesets();
}

void Globals::saveData()
{
  saveTilesets();
}

void Globals::freeData()
{
  freeTilesets();
  if (map != nullptr)
    delete map;
  mapNames.clear();
}

uint32_t Globals::xorshift32(uint32_t value)
{
  value ^= (value << 13);
  value ^= (value >> 17);
  value ^= (value << 5);
  return value;
}

uint32_t Globals::hash(const char block[], size_t size)
{
  constexpr size_t resultSize = sizeof(uint32_t);
  uint32_t sum = static_cast<uint32_t>(size);
  for (size_t i = 0; i < size; i += resultSize)
  {
    uint32_t value = 0;
    for (size_t j = 0; j < resultSize; ++j)
    {
      value <<= 8;
      const size_t index = (i + j);
      if(index < size)
        value |= block[index];
    }
    sum = xorshift32(sum + value);
  }
  return sum;
}

uint8_t Globals::charToCode(char ch)
{
  for (uint8_t i = 0; i < 80; ++i)
  {
    if (codes[i] == ch)
      return i;
  }
  return 0x0A;
}

void Globals::refreshBackdrops()
{
  QDir folder(QString("%1/backdrops").arg(Globals::datadir));
  backdrops = folder.entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
  backdrops.replaceInStrings(".png", "", Qt::CaseInsensitive);
  for (auto tileset : tilesets)
    tileset->refreshBackdrops();
  //printf("backdrops:\n");
  //for (int i = 0; i < backdrops.count(); ++i)
  //  printf("  %d: %s\n", i, backdrops[i].toLocal8Bit().data());
  Tileset::backdrops = backdrops;
}

void Globals::refreshMusic()
{
  QDir folder(QString("%1/music").arg(Globals::datadir));
  bgms = folder.entryList(QStringList() << "*.ogg", QDir::Files, QDir::Name);
  bgms.replaceInStrings(".ogg", "", Qt::CaseInsensitive);
}

void Globals::refreshSFX()
{
  QDir folder(QString("%1/sfx").arg(Globals::datadir));
  sfx = folder.entryList(QStringList() << "*.wav", QDir::Files, QDir::Name);
  sfx.replaceInStrings(".wav", "", Qt::CaseInsensitive);
}

QByteArray Globals::convertAudio(QString fileLocation, QProgressBar *progress)
{
  QAudioDecoder *decoder = new QAudioDecoder();
  QAudioFormat format;
  QByteArray bytes;
  format.setChannelCount(1);
  format.setCodec("audio/x-raw");
  format.setSampleType(QAudioFormat::UnSignedInt);
  format.setSampleRate(8000);
  format.setSampleSize(8);
  decoder->setAudioFormat(format);
  printf("decoding: \"%s\"\n", fileLocation.toLocal8Bit().data());
  decoder->setSourceFilename(fileLocation);
  decoder->start();
  printf("length: %lld\n", decoder->duration());
  if (progress != nullptr)
  {
    progress->setRange(0, decoder->duration());
    progress->setValue(0);
  }
  QCoreApplication::processEvents();
  while (decoder->position() != decoder->duration())
  {
    if (decoder->bufferAvailable())
    {
      QAudioBuffer buffer = decoder->read();
      bytes.append(buffer.data<char>(), buffer.byteCount());
      if (progress != nullptr)
        progress->setValue(decoder->position());
      QCoreApplication::processEvents();
    }
  }
  return bytes;
}

void Globals::loadTilesets()
{
  XMLNode tilesetsNode;
  XMLResults results;
  tilesetsNode = XMLNode::parseFile(QString("%1/tilesets/tilesets.xml").arg(Globals::datadir).toLocal8Bit().data(), "ptad-tilesets", &results);
  Tileset::backdrops = QString(tilesetsNode.getChildNode("backdrops").getText()).split('\n');
  Tileset::backdrops.replaceInStrings(QRegExp("\\s"), "");
  if (results.error != XMLError::eXMLErrorNone)
    QMessageBox::critical(nullptr, "XML Parser Error", QString("An error occurred when parsing tilesets.xml on line %1 column %2.\nError: %3").arg(results.nLine).arg(results.nColumn).arg(XMLNode::getError(results.error)));
  for (int i = 1; i < tilesetsNode.nChildNode(); ++i)
    tilesets += new Tileset(tilesetsNode.getChildNode(i));
  //for (int i = 0; i < Tileset::backdrops.count(); ++i)
  //  printf("%d (%s) -> %d\n", i, Tileset::backdrops[i].toLocal8Bit().data(), backdrops.indexOf(Tileset::backdrops[i]));
  Tileset::backdrops = backdrops;
}

void Globals::saveTilesets()
{
  XMLNode tilesetsNode = XMLNode::createXMLTopNode("ptad-tilesets");
  tilesetsNode.addChild("backdrops").addText(backdrops.join("\n\t\t").toLocal8Bit().data());
  for (int i = 0; i < tilesets.size(); ++i)
    tilesetsNode.addChild(tilesets[i]->getXMLNode());
  tilesetsNode.writeToFile(QString("%1/tilesets/tilesets.xml").arg(Globals::datadir).toLocal8Bit().data());
}

void Globals::freeTilesets()
{
  for (int i = 0; i < tilesets.size(); ++i)
    delete tilesets[i];
  tilesets.clear();
}
