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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QList>
#include <QMap>
#include <QString>
#include <QFileSystemWatcher>
#include <QMediaPlayer>
#include <QProgressBar>
#include <cmath>

class Image;
class Map;
class MainWindow;
class Tileset;

class Globals
{
  public:
    struct MapData
    {
      struct EventData
      {
        uint8_t x;
        uint8_t y;
        uint8_t flags;
        uint8_t spriteID;
        uint32_t offset;
      };
      uint8_t tilesetID;
      uint8_t bgmID;
      uint8_t width;
      uint8_t height;
      uint8_t sprites[16];
      uint8_t regions[64][4];
      EventData events[29];
      uint8_t minSteps;
      uint8_t maxSteps;
      uint8_t flags;
      uint8_t unused;
    };
    union Value32
    {
      int32_t sValue;
      uint32_t uValue;
      uint8_t bytes[4];
    };
    union Value16
    {
      int16_t sValue;
      uint16_t uValue;
      uint8_t bytes[2];
    };
    struct Item
    {
      QString name;
      QString description;
      uint16_t price;
    };
    struct EquipmentStats
    {
      int8_t stats[4];
      uint16_t spellResistance;
      int8_t statusResistance[2];
    };
    struct StatGrowth
    {
      int start;
      double base;
      double exponent;
    };
    static void setupData();
    static void saveData();
    static void freeData();
    static uint32_t xorshift32(uint32_t value);
    static uint32_t hash(const char block[], size_t size);
    static uint8_t charToCode(char ch);
    static void refreshBackdrops();
    static void refreshMusic();
    static void refreshSFX();
    static QByteArray convertAudio(QString fileLocation, QProgressBar *progress=nullptr);
    static constexpr int getStatForLevel(int level, const StatGrowth &growth)
    {
      return growth.start + (int)(growth.base * pow(level, growth.exponent));
    }
    static QFileSystemWatcher *watchdog;
    static QMediaPlayer *audio;
    static QList<Tileset*> tilesets;
    static MainWindow *mainwindow;
    static Map *map;
    static Image *animations;
    static Image *battlers;
    static Image *cursor;
    static Image *font;
    static Image *player;
    static Image *sprites;
    static Item items[8][16];
    static Item skills[16];
    static Item spells[16];
    static EquipmentStats equipmentStats[6][16];
    static StatGrowth statsGrowth[7];
    static QString datadir;
    static QMap<int, QString> mapNames;
    static QMap<QString, QString> messages;
    static QStringList backdrops;
    static QStringList bgms;
    static QStringList sfx;
    static const uint8_t codes[];
    static const char *itemTypes[8];
    static const char *statNames[7];
    static const char *spellIcons[4];
  private:
    static void loadTilesets();
    static void saveTilesets();
    static void freeTilesets();
};

#endif //GLOBALS_H
