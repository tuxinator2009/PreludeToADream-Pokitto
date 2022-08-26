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

#ifndef TILESET_H
#define TILESET_H

#include "image.h"
#include "xmlParser.h"

class Tileset
{
  public:
    Tileset(QString n, QString imageLocation);
    Tileset(XMLNode tilesetNode);
    ~Tileset();
    XMLNode getXMLNode();
    QString getName() {return name;}
    void setName(QString value) {name = value;}
    QString getLocation() {return location;}
    Image *getImage() {return image;}
    uint8_t getBattleBG(int tileID) {return battleBG[tileID];}
    uint8_t getEncounterRate(int tileID) {return encounterRate[tileID];}
    void setBattleBG(int tileID, uint8_t value) {battleBG[tileID] = value;}
    void setEncounterRate(int tileID, uint8_t value) {encounterRate[tileID] = value;}
  private:
    QString name;
    QString location;
    Image *image;
    uint8_t battleBG[128];
    uint8_t encounterRate[128];
};

#endif //TILESET_H
