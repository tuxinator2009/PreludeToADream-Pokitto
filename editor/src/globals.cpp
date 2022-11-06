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

#include <QDir>
#include <QMessageBox>
#include <QList>
#include <QProgressBar>
#include <QString>
#include <QThread>
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
Globals::Item Globals::items[8][16];
Globals::Item Globals::skills[16];
Globals::Item Globals::spells[16];
Globals::EquipmentStats Globals::equipmentStats[6][16];
Globals::StatGrowth Globals::statsGrowth[7];
QString Globals::datadir;
QString Globals::femtoIDE;
QMap<int, QString> Globals::mapNames;
QMap<QString, QString> Globals::messages;
QStringList Globals::backdrops;
QStringList Globals::bgms;
QStringList Globals::sfx;
uint8_t Globals::skillLearned[16];
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
const char *Globals::itemTypes[8] = {"consumables","tradeables","weapons","helmets","armor","boots","rings","amulets"};
const char *Globals::statNames[7] = {"hp", "mp", "attack", "defense", "agility", "magic", "experience"};
const char *Globals::spellIcons[4] = {":/icons/spellResistance0.png",":/icons/spellResistance05.png",":/icons/spellResistance1.png",":/icons/spellResistance2.png"};

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
