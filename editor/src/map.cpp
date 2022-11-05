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

#include <cstdlib>
#include "globals.h"
#include "map.h"
#include "mapevent.h"

Map::Map(XMLNode mapNode)
{
  XMLNode tempNode;
  QStringList text = QString(mapNode.getAttribute("sprites")).split(',');
  for (int i = 0; i < 16; ++i)
    data.sprites[i] = text[i].toUInt();
  for (int i = 0; i < 64; ++i)
  {
    data.regions[i][0] = 255;
    data.regions[i][1] = 255;
    data.regions[i][2] = 255;
    data.regions[i][3] = 255;
  }
  for (int i = 0; i < 256*256; ++i)
  {
    bg[i] = 255;
    fg[i] = 255;
  }
  for (int i = 0; i < 64*64/8; ++i)
    passability[i] = 0;
  bgm = mapNode.getAttribute("bgm");
  data.tilesetID = atoi(mapNode.getAttribute("tileset"));
  data.width = atoi(mapNode.getAttribute("width"));
  data.height = atoi(mapNode.getAttribute("height"));
  data.minSteps = atoi(mapNode.getAttribute("minSteps"));
  data.maxSteps = atoi(mapNode.getAttribute("maxSteps"));
  data.flags = atoi(mapNode.getAttribute("flags"));
  data.unused = 0;
  tempNode = mapNode.getChildNode("bg");
  text = QString(tempNode.getText()).split(',');
  for (int y = 0; y < data.height * 32; ++y)
  {
    for (int x = 0; x < data.width * 32; ++x)
      bg[y * 256 + x] = text[y * (data.width * 32) + x].toUInt();
  }
  tempNode = mapNode.getChildNode("fg");
  text = QString(tempNode.getText()).split(',');
  for (int y = 0; y < data.height * 32; ++y)
  {
    for (int x = 0; x < data.width * 32; ++x)
      fg[y * 256 + x] = text[y * (data.width * 32) + x].toUInt();
  }
  tempNode = mapNode.getChildNode("passability");
  text = QString(tempNode.getText()).split(',');
  for (int y = 0; y < data.height * 2; ++y)
  {
    for (int x = 0; x < data.width * 16; ++x)
      passability[y * 64 + x] = text[y * data.width * 16 + x].toUInt();
  }
  tempNode = mapNode.getChildNode("regions");
  text = QString(tempNode.getText()).split(',');
  for (int y = 0; y < data.height * 2; ++y)
  {
    for (int x = 0; x < data.width * 2; ++x)
    {
      for (int i = 0; i < 4; ++i)
        data.regions[y * 8 + x][i] = text[y * data.width * 2 * 4 + x * 4 + i].toUInt();
    }
  }
  onLoadEvent = new MapEvent(this, mapNode.getChildNode("onLoadEvent"));
  tempNode = mapNode.getChildNode("events");
  for (int i = 0; i < 29; ++i)
  {
    if (i < tempNode.nChildNode("event"))
      events[i] = new MapEvent(this, tempNode.getChildNode("event", i));
    else
      events[i] = nullptr;
  }
}

Map::Map(uint8_t width, uint8_t height)
{
  bgm = Globals::bgms[0];
  data.tilesetID = 0;
  data.bgmID = 0;
  data.width = width;
  data.height = height;
  for (int i = 0; i < 16; ++i)
    data.sprites[i] = 255;
  for (int i = 0; i < 64; ++i)
  {
    data.regions[i][0] = 255;
    data.regions[i][1] = 255;
    data.regions[i][2] = 255;
    data.regions[i][3] = 255;
  }
  data.minSteps = 0;
  data.maxSteps = 8;
  data.flags = 0;
  data.unused = 0;
  for (int i = 0; i < 256*256; ++i)
  {
    bg[i] = 255;
    fg[i] = 255;
  }
  for (int y = 0; y < 128; ++y)
  {
    for (int x = 0; x < 128; ++x)
      bg[y * 256 + x] = (y % 2) * 16 + (x % 2);
  }
  for (int i = 0; i < 64*64/8; ++i)
    passability[i] = 0;
  onLoadEvent = new MapEvent(this);
  for (int i = 0; i < 29; ++i)
    events[i] = nullptr;
}

Map::~Map()
{
  delete onLoadEvent;
  for (int i = 0; i < 29; ++i)
  {
    if (events[i] != nullptr)
      delete events[i];
  }
}

XMLNode Map::toXMLNode()
{
  XMLNode mapNode = XMLNode::createXMLTopNode("ptad-map");
  XMLNode tempNode;
  QStringList text;
  mapNode.addAttribute("tileset", QString::number(data.tilesetID).toLocal8Bit().data());
  mapNode.addAttribute("bgm", bgm.toLocal8Bit().data());
  mapNode.addAttribute("width", QString::number(data.width).toLocal8Bit().data());
  mapNode.addAttribute("height", QString::number(data.height).toLocal8Bit().data());
  mapNode.addAttribute("minSteps", QString::number(data.minSteps).toLocal8Bit().data());
  mapNode.addAttribute("maxSteps", QString::number(data.maxSteps).toLocal8Bit().data());
  mapNode.addAttribute("flags", QString::number(data.flags).toLocal8Bit().data());
  for (int i = 0; i < 16; ++i)
    text += QString::number(data.sprites[i]);
  mapNode.addAttribute("sprites", text.join(',').toLocal8Bit().data());
  text.clear();
  for (int y = 0; y < data.height * 32; ++y)
  {
    for (int x = 0; x < data.width * 32; ++x)
      text += QString::number(bg[y * 256 + x]);
  }
  mapNode.addChild(onLoadEvent->toXMLNode(true));
  mapNode.addChild("bg").addText(text.join(',').toLocal8Bit().data());
  text.clear();
  for (int y = 0; y < data.height * 32; ++y)
  {
    for (int x = 0; x < data.width * 32; ++x)
      text += QString::number(fg[y * 256 + x]);
  }
  mapNode.addChild("fg").addText(text.join(',').toLocal8Bit().data());
  text.clear();
  for (int y = 0; y < data.height * 2; ++y)
  {
    for (int x = 0; x < data.width * 16; ++x)
      text += QString::number(passability[y * 64 + x]);
  }
  mapNode.addChild("passability").addText(text.join(',').toLocal8Bit().data());
  text.clear();
  for (int y = 0; y < data.height * 2; ++y)
  {
    for (int x = 0; x < data.width * 2; ++x)
    {
      for (int i = 0; i < 4; ++i)
        text += QString::number(data.regions[y * 8 + x][i]);
    }
  }
  mapNode.addChild("regions").addText(text.join(',').toLocal8Bit().data());
  tempNode = mapNode.addChild("events");
  for (int i = 0; i < 29; ++i)
  {
    if (events[i] != nullptr)
      tempNode.addChild(events[i]->toXMLNode(false));
  }
  return mapNode;
}

QByteArray Map::compile()
{
  QByteArray bytes;
  bytes.append(sizeof(Globals::MapData), 0);
  for (int y = 0; y < data.height * 2; ++y)
  {
    for (int x = 0; x < data.width * 16; ++x)
      bytes += passability[y * 64 + x];
  }
  for (int y = 0; y < data.height * 32; y += 32)
  {
    for (int x = 0; x < data.width * 32; x += 32)
    {
      for (int y2 = 0; y2 < 32; ++y2)
      {
        for (int x2 = 0; x2 < 32; ++x2)
          bytes += bg[(y + y2) * 256 + (x + x2)];
      }
      for (int y2 = 0; y2 < 32; ++y2)
      {
        for (int x2 = 0; x2 < 32; ++x2)
          bytes += fg[(y + y2) * 256 + (x + x2)];
      }
    }
  }
  onLoadEvent->compileEvent(&data, -1, &bytes);
  for (int i = 0; i < 29; ++i)
  {
    if (events[i] != nullptr)
      events[i]->compileEvent(&data, i, &bytes);
  }
  bytes.replace(0, sizeof(Globals::MapData), (char*)&data);
  return bytes;
}

void Map::resize(uint8_t width, uint8_t height)
{
  data.width = width;
  data.height = height;
}
