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

#include <QScrollBar>
#include <QTimer>
#include "globals.h"
#include "image.h"
#include "imageview.h"
#include "map.h"
#include "mapevent.h"
#include "shiftmap.h"
#include "tileset.h"

const int ShiftMap::zoom[5] = {64, 128, 256, 512, 1024};

ShiftMap::ShiftMap(QWidget *parent) : QDialog(parent)
{
  int width = Globals::map->getWidth() * 32 - 1;
  int height = Globals::map->getHeight() * 32 - 1;
  setupUi(this);
  mapImage = new Image(QSize(Globals::map->getWidth() * 256, Globals::map->getHeight() * 256));
  imgMapView->setImage(mapImage);
  imgMapView->setSize(Globals::map->getWidth() * zoom[optZoom->currentIndex()], Globals::map->getHeight() * zoom[optZoom->currentIndex()]);
  connect(numXShift, SIGNAL(valueChanged(int)), this, SLOT(updateMapView()));
  connect(numYShift, SIGNAL(valueChanged(int)), this, SLOT(updateMapView()));
  connect(optWrap, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMapView()));
  updateMapView();
  numXShift->setRange(width * -1, width);
  numYShift->setRange(height * -1, height);
}

ShiftMap::~ShiftMap()
{
  delete mapImage;
}

void ShiftMap::shiftMap()
{
  int xShift = numXShift->value();
  int yShift = numYShift->value();
  int wrap = optWrap->currentIndex();
  uint8_t bg[128*128];
  uint8_t fg[128*128];
  for (int y = 0; y < 128; ++y)
  {
    for (int x = 0; x < 128; ++x)
    {
      bg[y * 128 + x] = Globals::map->getBGTile(x, y);
      fg[y * 128 + x] = Globals::map->getFGTile(x, y);
    }
  }
  for (int y = 0; y < Globals::map->getHeight() * 16; ++y)
  {
    for (int x = 0; x < Globals::map->getWidth() * 16; ++x)
    {
      int tileX = x - xShift;
      int tileY = y - yShift;
      if (tileX < 0)
        tileX += Globals::map->getWidth() * 64;
      if (tileY < 0)
        tileY += Globals::map->getHeight() * 64;
      if ((wrap & WRAP_HORIZONTALLY) != 0)
        tileX %= Globals::map->getWidth() * 32;
      if ((wrap & WRAP_VERTICALLY) != 0)
        tileY %= Globals::map->getHeight() * 32;
      if (tileX < Globals::map->getWidth() * 32 && tileY < Globals::map->getHeight() * 32)
      {
        Globals::map->setBGTile(x, y, bg[tileY * 256 + tileX]);
        Globals::map->setFGTile(x, y, fg[tileY * 256 + tileX]);
      }
      else
      {
        Globals::map->setBGTile(x, y, (y % 2) * 16 + (x % 2));
        Globals::map->setFGTile(x, y, 255);
      }
    }
  }
}

void ShiftMap::on_optZoom_currentIndexChanged(int index)
{
  imgMapView->setSize(Globals::map->getWidth() * zoom[index], Globals::map->getHeight() * zoom[index]);
  imgMapView->imageChanged();
}

void ShiftMap::on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta)
{
  Q_UNUSED(pos);
  if (buttons & Qt::RightButton)
  {
    QScrollBar *scrollBar = panMapArea->horizontalScrollBar();
    scrollBar->setValue(scrollBar->value() - delta.x());
    scrollBar = panMapArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->value() - delta.y());
  }
}

void ShiftMap::on_imgMapView_zoomOut(QPoint pos)
{
  if (optZoom->currentIndex() == 0)
    return;
  optZoom->setCurrentIndex(optZoom->currentIndex() - 1);
  zoomPos = (pos / 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void ShiftMap::on_imgMapView_zoomIn(QPoint pos)
{
  if (optZoom->currentIndex() == 4)
    return;
  optZoom->setCurrentIndex(optZoom->currentIndex() + 1);
  zoomPos = (pos * 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void ShiftMap::updateZoom()
{
  panMapArea->horizontalScrollBar()->setValue(zoomPos.x());
  panMapArea->verticalScrollBar()->setValue(zoomPos.y());
}

void ShiftMap::updateMapView()
{
  Image *tilesetImage = Globals::tilesets[Globals::map->getTilesetID()]->getImage();
  int xShift = numXShift->value();
  int yShift = numYShift->value();
  int wrap = optWrap->currentIndex();
  for (int y = 0; y < Globals::map->getHeight() * 32; ++y)
  {
    for (int x = 0; x < Globals::map->getWidth() * 32; ++x)
    {
      int tileX = x - xShift;
      int tileY = y - yShift;
      uint8_t tileID = 255;
      if (tileX < 0)
        tileX += Globals::map->getWidth() * 64;
      if (tileY < 0)
        tileY += Globals::map->getHeight() * 64;
      if ((wrap & WRAP_HORIZONTALLY) != 0)
        tileX %= Globals::map->getWidth() * 32;
      if ((wrap & WRAP_VERTICALLY) != 0)
        tileY %= Globals::map->getHeight() * 32;
      if (tileX < Globals::map->getWidth() * 32 && tileY < Globals::map->getHeight() * 32)
        tileID = Globals::map->getBGTile(tileX, tileY);
      else
        tileID = (y % 2) * 16 + (x % 2);
      mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), false);
      if (tileX < Globals::map->getWidth() * 32 && tileY < Globals::map->getHeight() * 32)
        tileID = Globals::map->getFGTile(tileX, tileY);
      else
        tileID = 255;
      if (tileID < 128)
        mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), true);
    }
  }
  imgMapView->imageChanged();
}
