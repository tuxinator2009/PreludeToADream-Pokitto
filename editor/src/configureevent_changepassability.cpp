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
#include "configureevent_changepassability.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"
#include "map.h"
#include "mapevent.h"
#include "tileset.h"

const int ConfigureEvent_ChangePassability::zoom[5] = {64, 128, 256, 512, 1024};

ConfigureEvent_ChangePassability::ConfigureEvent_ChangePassability(Map *m, QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  map = m;
  currentZoom = 2;
  mapImage = new Image(QSize(map->getWidth() * 256, map->getHeight() * 256), this);
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->setImage(mapImage);
  location = QPoint(0, 0);
}

ConfigureEvent_ChangePassability::~ConfigureEvent_ChangePassability()
{
}

void ConfigureEvent_ChangePassability::on_imgMapView_mousePressed(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    location = QPoint(pos.x() / (zoom[currentZoom] / 16), pos.y() / (zoom[currentZoom] / 16));
    updateMapView();
  }
}

void ConfigureEvent_ChangePassability::on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta)
{
  if (buttons & Qt::LeftButton)
  {
    location = QPoint(pos.x() / (zoom[currentZoom] / 16), pos.y() / (zoom[currentZoom] / 16));
    updateMapView();
  }
  else if (buttons & Qt::RightButton)
  {
    QScrollBar *scrollBar = panMapArea->horizontalScrollBar();
    scrollBar->setValue(scrollBar->value() - delta.x());
    scrollBar = panMapArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->value() - delta.y());
  }
}

void ConfigureEvent_ChangePassability::on_imgMapView_zoomOut(QPoint pos)
{
  if (currentZoom == 0)
    return;
  --currentZoom;
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->imageChanged();
  zoomPos = (pos / 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void ConfigureEvent_ChangePassability::on_imgMapView_zoomIn(QPoint pos)
{
  if (currentZoom == 4)
    return;
  ++currentZoom;
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->imageChanged();
  zoomPos = (pos * 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void ConfigureEvent_ChangePassability::updateZoom()
{
  panMapArea->horizontalScrollBar()->setValue(zoomPos.x());
  panMapArea->verticalScrollBar()->setValue(zoomPos.y());
}

void ConfigureEvent_ChangePassability::updateMapView()
{
  Image *tilesetImage = Globals::tilesets[map->getTilesetID()]->getImage();
  for (int y = 0; y < map->getHeight() * 32; ++y)
  {
    for (int x = 0; x < map->getWidth() * 32; ++x)
    {
      uint8_t tileID = map->getBGTile(x, y);
      if (tileID >= 128 && tileID <= 136)
      {
        //TODO: calculate auto-tile
      }
      if (tileID < 128)
        mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), false);
    }
  }
  for (int i = 0; i < 29; ++i)
  {
    MapEvent *event = map->getMapEvent(i);
    if (event == nullptr)
      break;
    uint8_t spriteID = map->getSpriteID(event->getSpriteID());
    mapImage->blitImage(*Globals::sprites, QRect(spriteID % 16 * 16, spriteID / 16 * 24, 16, 24), QPoint(event->getX() * 16, event->getY() * 16 + (event->isFlagSet(MapEvent::FLAGS_OFFSET) ? 0:8) - 8), true);
  }
  for (int y = 0; y < map->getHeight() * 32; ++y)
  {
    for (int x = 0; x < map->getWidth() * 32; ++x)
    {
      uint8_t tileID = map->getFGTile(x, y);
      if (tileID >= 128 && tileID <= 136)
      {
        //TODO: calculate auto-tile
      }
      if (tileID < 128)
        mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), true);
    }
  }
  mapImage->drawRect(QRect(location.x() * 16, location.y() * 16, 16, 16), 255);
  mapImage->drawRect(QRect(location.x() * 16 + 1, location.y() * 16 + 1, 14, 14), 255);
  imgMapView->imageChanged();
}
