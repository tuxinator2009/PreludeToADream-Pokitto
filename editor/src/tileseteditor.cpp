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
#include "tileset.h"
#include "tileseteditor.h"

TilesetEditor::TilesetEditor(Tileset *set, QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  tileset = set;
  numbers = new Image(":/images/numbers.png", nullptr, false);
  image = new Image(QSize(128, 64));
  imgTileset->setTransparent(true);
  imgTileset->setImage(image);
  redrawImage();
  optBackdrops->addItems(Globals::backdrops);
}

TilesetEditor::~TilesetEditor()
{
}

void TilesetEditor::on_imgTileset_mousePressed(Qt::MouseButton button, QPoint pos)
{
  int tileX = pos.x() / 16;
  int tileY = pos.y() / 16;
  tileX &= ~1;
  tileY &= ~1;
  if (button == Qt::LeftButton)
  {
    tileset->setBattleBG(tileY * 16 + tileX, optBackdrops->currentIndex());
    tileset->setBattleBG(tileY * 16 + tileX + 1, optBackdrops->currentIndex());
    tileset->setBattleBG((tileY + 1) * 16 + tileX, optBackdrops->currentIndex());
    tileset->setBattleBG((tileY + 1) * 16 + tileX + 1, optBackdrops->currentIndex());
    tileset->setEncounterRate(tileY * 16 + tileX, numRate->value());
    tileset->setEncounterRate(tileY * 16 + tileX + 1, numRate->value());
    tileset->setEncounterRate((tileY + 1) * 16 + tileX, numRate->value());
    tileset->setEncounterRate((tileY + 1) * 16 + tileX + 1, numRate->value());
    redrawImage();
  }
  else if (button == Qt::RightButton)
  {
    optBackdrops->setCurrentIndex(tileset->getBattleBG(tileY * 16 + tileX));
    numRate->setValue(tileset->getEncounterRate(tileY * 16 + tileX));
  }
}

void TilesetEditor::on_imgTileset_mouseMoved(Qt::MouseButtons buttons, QPoint pos)
{
  if ((buttons & Qt::LeftButton) != 0)
  {
    int tileX = pos.x() / 16;
    int tileY = pos.y() / 16;
    tileX &= ~1;
    tileY &= ~1;
    tileset->setBattleBG(tileY * 16 + tileX, optBackdrops->currentIndex());
    tileset->setBattleBG(tileY * 16 + tileX + 1, optBackdrops->currentIndex());
    tileset->setBattleBG((tileY + 1) * 16 + tileX, optBackdrops->currentIndex());
    tileset->setBattleBG((tileY + 1) * 16 + tileX + 1, optBackdrops->currentIndex());
    tileset->setEncounterRate(tileY * 16 + tileX, numRate->value());
    tileset->setEncounterRate(tileY * 16 + tileX + 1, numRate->value());
    tileset->setEncounterRate((tileY + 1) * 16 + tileX, numRate->value());
    tileset->setEncounterRate((tileY + 1) * 16 + tileX + 1, numRate->value());
    redrawImage();
  }
}

void TilesetEditor::redrawImage()
{
  image->blitImage(*tileset->getImage(), QRect(0, 0, 128, 64), QPoint(0, 0), false);
  for (int i = 0; i < 128; ++i)
  {
    int tileX = i % 16;
    int tileY = i / 16;
    if (tileX % 2 == 0 && tileY % 2 == 0)
    {
      drawNumber(tileset->getBattleBG(i), tileX * 8 + 2, tileY * 8);
      drawNumber(tileset->getEncounterRate(i), tileX * 8 + 2, tileY * 8 + 8);
    }
  }
  imgTileset->imageChanged();
}

void TilesetEditor::drawNumber(uint8_t value, int x, int y)
{
  if (value > 100)
    image->blitImage(*numbers, QRect(value / 100 % 10 * 4, 0, 4, 8), QPoint(x, y), true);
  if (value > 10)
    image->blitImage(*numbers, QRect(value / 10 % 10 * 4, 0, 4, 8), QPoint(x + 4, y), true);
  image->blitImage(*numbers, QRect(value % 10 * 4, 0, 4, 8), QPoint(x + 8, y), true);
}
