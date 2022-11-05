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

#include <QFileDialog>
#include <QScrollBar>
#include <QTimer>
#include "globals.h"
#include "image.h"
#include "imageview.h"
#include "map.h"
#include "mapevent.h"
#include "previewmap.h"
#include "tileset.h"

const int PreviewMap::zoom[5] = {64, 128, 256, 512, 1024};

PreviewMap::PreviewMap(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  mapImage = new Image(QSize(Globals::map->getWidth() * 256, Globals::map->getHeight() * 256));
  imgMapView->setImage(mapImage);
  imgMapView->setTransparent(true);
  imgMapView->setSize(Globals::map->getWidth() * zoom[optZoom->currentIndex()], Globals::map->getHeight() * zoom[optZoom->currentIndex()]);
  connect(chkGrid, SIGNAL(toggled(bool)), this, SLOT(updateMapView()));
  connect(chkPassability, SIGNAL(toggled(bool)), this, SLOT(updateMapView()));
  connect(chkRegions, SIGNAL(toggled(bool)), this, SLOT(updateMapView()));
  connect(chkForeground, SIGNAL(toggled(bool)), this, SLOT(updateMapView()));
  connect(chkEvents, SIGNAL(toggled(bool)), this, SLOT(updateMapView()));
  connect(chkBackground, SIGNAL(toggled(bool)), this, SLOT(updateMapView()));
  updateMapView();
}

PreviewMap::~PreviewMap()
{
  delete mapImage;
}

void PreviewMap::on_optZoom_currentIndexChanged(int index)
{
  imgMapView->setSize(Globals::map->getWidth() * zoom[index], Globals::map->getHeight() * zoom[index]);
  imgMapView->imageChanged();
}

void PreviewMap::on_btnExportImage_clicked()
{
  QString location = QFileDialog::getSaveFileName(this, "Export Map Image", Globals::datadir, "PNG Images (*.png)");
  int currentZoom = optZoom->currentIndex();
  if (!location.isEmpty())
    mapImage->toQImage().scaled(Globals::map->getWidth() * zoom[currentZoom], Globals::map->getHeight() * zoom[currentZoom]).save(location);
}

void PreviewMap::on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta)
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

void PreviewMap::on_imgMapView_zoomOut(QPoint pos)
{
  if (optZoom->currentIndex() == 0)
    return;
  optZoom->setCurrentIndex(optZoom->currentIndex() - 1);
  zoomPos = (pos / 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void PreviewMap::on_imgMapView_zoomIn(QPoint pos)
{
  if (optZoom->currentIndex() == 4)
    return;
  optZoom->setCurrentIndex(optZoom->currentIndex() + 1);
  zoomPos = (pos * 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void PreviewMap::updateZoom()
{
  panMapArea->horizontalScrollBar()->setValue(zoomPos.x());
  panMapArea->verticalScrollBar()->setValue(zoomPos.y());
}

void PreviewMap::updateMapView()
{
  Image *tilesetImage = Globals::tilesets[Globals::map->getTilesetID()]->getImage();
  Image passability(":/images/passability", nullptr, false);
  mapImage->fillRect(QRect(0, 0, mapImage->getWidth(), mapImage->getHeight()), 0);
  if (chkBackground->isChecked())
  {
    for (int y = 0; y < Globals::map->getHeight() * 32; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 32; ++x)
      {
        uint8_t tileID = Globals::map->getBGTile(x, y);
        if (tileID < 128)
          mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), false);
      }
    }
  }
  if (chkEvents->isChecked())
  {
    for (int i = 0; i < 29; ++i)
    {
      MapEvent *event = Globals::map->getMapEvent(i);
      if (event == nullptr)
        break;
      uint8_t spriteID = Globals::map->getSpriteID(event->getSpriteID());
      mapImage->blitImage(*Globals::sprites, QRect(spriteID % 16 * 16, spriteID / 16 * 24, 16, 24), QPoint(event->getX() * 16, event->getY() * 16 + (event->isFlagSet(MapEvent::FLAGS_OFFSET) ? 0:8) - 8), true);
    }
  }
  if (chkForeground->isChecked())
  {
    for (int y = 0; y < Globals::map->getHeight() * 32; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 32; ++x)
      {
        uint8_t tileID = Globals::map->getFGTile(x, y);
        if (tileID < 128)
          mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), true);
      }
    }
  }
  if (chkRegions->isChecked())
  {
    for (int y = 0; y < Globals::map->getHeight() * 2; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 2; ++x)
      {
        mapImage->drawRect(QRect(x * 128, y * 128, 128, 128), 4);
        for (int i = 0; i < 4; ++i)
        {
          uint8_t monsterID = Globals::map->getRegionMonster(y * 8 + x, i);
          if (monsterID != 255)
            mapImage->blitImage(*Globals::battlers, QRect(monsterID % 16 * 48, monsterID / 16 * 48, 48, 48), QPoint(x * 128 + i % 2 * 64 + 8, y * 128 + i / 2 * 64 + 8), true);
        }
      }
    }
  }
  if (chkPassability->isChecked())
  {
    for (int y = 0; y < Globals::map->getHeight() * 16; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 16; ++x)
      {
        if (Globals::map->getPassability(x, y))
          mapImage->blitImage(passability, QRect(0, 0, 32, 32), QPoint(x * 16, y * 16), true);
      }
    }
  }
  if (chkGrid->isChecked())
  {
    for (int x = 0; x < Globals::map->getWidth() * 16; ++x)
      mapImage->drawVLine(QPoint(x * 16 + 15, 0), mapImage->getHeight(), 231);
    for (int y = 0; y < Globals::map->getHeight() * 16; ++y)
      mapImage->drawHLine(QPoint(0, y * 16 + 15), mapImage->getWidth(), 231);
  }
  imgMapView->imageChanged();
}
