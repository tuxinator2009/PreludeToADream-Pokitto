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

#include "globals.h"
#include "image.h"
#include "tileset.h"
#include "xmlParser.h"

QStringList Tileset::backdrops;

Tileset::Tileset(QString n, QString imageLocation)
{
  name = n;
  location = imageLocation;
  image = new Image(QString("%1/tilesets/%2").arg(Globals::datadir).arg(location));
  for (int i = 0; i < 128; ++i)
  {
    battleBG[i] = 0;
    encounterRate[i] = 0;
  }
}

Tileset::Tileset(XMLNode tilesetNode)
{
  QStringList text = QString(tilesetNode.getText()).split(',');
  name = tilesetNode.getAttribute("name");
  location = tilesetNode.getAttribute("file");
  image = new Image(QString("%1/tilesets/%2").arg(Globals::datadir).arg(location));
  for (int i = 0; i < 128; ++i)
  {
    battleBG[i] = text[i * 2].toUInt();
    battleBG[i] = Globals::backdrops.indexOf(backdrops[battleBG[i]]);
    encounterRate[i] = text[i * 2 + 1].toUInt();
  }
}

Tileset::~Tileset()
{
  delete image;
}

XMLNode Tileset::getXMLNode()
{
  XMLNode tilesetNode = XMLNode::createXMLTopNode("tileset");
  QString text;
  tilesetNode.addAttribute("name", name.toLocal8Bit().data());
  tilesetNode.addAttribute("file", location.toLocal8Bit().data());
  for (int i = 0; i < 128; ++i)
  {
    text += QString("%1,%2").arg(battleBG[i]).arg(encounterRate[i]);
    if (i != 127)
    {
      text += ",";
      if (i % 16 == 15)
        text += "\n\t\t";
    }
  }
  tilesetNode.addText(text.toLocal8Bit().data());
  return tilesetNode;
}

QByteArray Tileset::compile()
{
  QByteArray bytes;
  for (int i = 0; i < 128; ++i)
  {
    bytes += battleBG[i];
    bytes += encounterRate[i];
  }
  return bytes;
}

void Tileset::refreshBackdrops()
{
  for (int i = 0; i < 128; ++i)
    battleBG[i] = Globals::backdrops.indexOf(backdrops[battleBG[i]]);
}
