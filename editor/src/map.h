/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2020 Justin (tuxinator2009) Davis                                *
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

#ifndef MAP_H
#define MAP_H

#include "globals.h"
#include "mapevent.h"
#include "xmlParser.h"

class Map
{
  public:
    static constexpr uint8_t FLAG_CLIP_UP = 1;
    static constexpr uint8_t FLAG_CLIP_DOWN = 2;
    static constexpr uint8_t FLAG_CLIP_LEFT = 4;
    static constexpr uint8_t FLAG_CLIP_RIGHT = 8;
    static constexpr uint8_t FLAG_DARK = 16;
    static constexpr uint8_t FLAG_DREAM = 32;
    static constexpr uint8_t FLAG_SPIRIT = 64;
    static constexpr int LAYER_GRID = 0;
    static constexpr int LAYER_PASSABILITY = 1;
    static constexpr int LAYER_REGIONS = 2;
    static constexpr int LAYER_FOREGROUND = 3;
    static constexpr int LAYER_EVENTS = 4;
    static constexpr int LAYER_BACKGROUND = 5;
    Map(XMLNode mapNode);
    Map(uint8_t width, uint8_t height);
    ~Map();
    XMLNode toXMLNode();
    uint8_t getTilesetID() const {return data.tilesetID;}
    void setTilesetID(uint8_t value) {data.tilesetID = value;}
    QString getBGM() const {return bgm;}
    void setBGM(QString value) {bgm = value;}
    uint8_t getWidth() const {return data.width;}
    uint8_t getHeight() const {return data.height;}
    void resize(uint8_t width, uint8_t height);
    uint8_t getSpriteID(uint8_t num) const {return (num == 255) ? 255:data.sprites[num];}
    void setSpriteID(uint8_t num, uint8_t id) {data.sprites[num] = id;}
    uint8_t getRegionMonster(uint8_t region, uint8_t num) const {return data.regions[region][num];}
    void setRegionMonster(uint8_t region, uint8_t num, uint8_t monsterID) {data.regions[region][num] = monsterID;}
    uint8_t getMinSteps() const {return data.minSteps;}
    void setMinSteps(uint8_t value) {data.minSteps = value;}
    uint8_t getMaxSteps() const {return data.maxSteps;}
    void setMaxSteps(uint8_t value) {data.maxSteps = value;}
    uint8_t getFlags() const {return data.flags;}
    void setFlags(uint8_t value) {data.flags = value;}
    MapEvent *getOnLoadEvent() {return onLoadEvent;}
    MapEvent *getMapEvent(uint8_t num) {return events[num];}
    void setMapEvent(uint8_t num, MapEvent *event) {events[num] = event;}
    int getMapEventID(MapEvent *event) {for (int i = 0; i < 29; ++i) {if (events[i] == event) return i;} return -1;}
    int numMapEvents() {for (int i = 0; i < 29; ++i) {if (events[i] == nullptr) return i;} return 29;}
    uint8_t getBGTile(int x, int y) {return bg[y * 256 + x];}
    void setBGTile(int x, int y, uint8_t tile) {bg[y * 256 + x] = tile;}
    uint8_t getFGTile(int x, int y) {return fg[y * 256 + x];}
    void setFGTile(int x, int y, uint8_t tile) {fg[y * 256 + x] = tile;}
    bool getPassability(int x, int y) {return (passability[(y / 8) * 64 + x] >> (y % 8)) & 1;}
    void setPassability(int x, int y) {passability[(y / 8) * 64 + x] |= 1 << (y % 8);}
    void clearPassability(int x, int y) {passability[(y / 8) * 64 + x] &= ~(1 << (y % 8));}
    void togglePassability(int x, int y) {passability[(y / 8) * 64 + x] ^= 1 << (y % 8);}
  private:
    Globals::MapData data;
    QString bgm;
    MapEvent *onLoadEvent;
    MapEvent *events[29];
    //Max Map Size: 256x256
    //Chunk Size: 32x32
    uint8_t bg[256*256];
    uint8_t fg[256*256];
    uint8_t passability[64*64/8];
};

#endif //MAP_H
