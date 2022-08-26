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
#include "configureevent_teleport.h"
#include "mainwindow.h"
#include "tileset.h"

const int ConfigureEvent_Teleport::zoom[5] = {64, 128, 256, 512, 1024};

ConfigureEvent_Teleport::ConfigureEvent_Teleport(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  map = nullptr;
  mapImage = nullptr;
  buildMapTree(treeMaps->invisibleRootItem(), Globals::mainwindow->treeMaps->invisibleRootItem());
  location = QPoint(0, 0);
  currentZoom = 2;
}

ConfigureEvent_Teleport::~ConfigureEvent_Teleport()
{
  if (map != nullptr)
    delete map;
  if (mapImage != nullptr)
    delete mapImage;
}

void ConfigureEvent_Teleport::setMapID(int value)
{
  QList<QTreeWidgetItem*> items = treeMaps->findItems(QString("map%1.xml").arg(value, 4, 10, QChar('0')), Qt::MatchFixedString | Qt::MatchRecursive, 1);
  if (items.count() == 0)
    return;
  items[0]->setSelected(true);
  treeMaps->setCurrentItem(items[0]);
  on_treeMaps_itemClicked(items[0]);
}

int ConfigureEvent_Teleport::getMapID()
{
  return treeMaps->currentItem()->text(1).mid(3, 4).toInt();
}

void ConfigureEvent_Teleport::setLocation(int x, int y)
{
  location = QPoint(x, y);
  panMapArea->ensureVisible(x * 16, y * 16);
  updateMapView();
}

int ConfigureEvent_Teleport::getX()
{
  return location.x();
}

int ConfigureEvent_Teleport::getY()
{
  return location.y();
}

void ConfigureEvent_Teleport::setFacing(int facing)
{
  if (facing == 0)
    btnFacingUp->setChecked(true);
  else if (facing == 1)
    btnFacingRight->setChecked(true);
  else if (facing == 2)
    btnFacingDown->setChecked(true);
  else if (facing == 3)
    btnFacingLeft->setChecked(true);
  else
    btnFacingRetain->setChecked(true);
}

int ConfigureEvent_Teleport::getFacing()
{
  if (btnFacingUp->isChecked())
    return 0;
  else if (btnFacingRight->isChecked())
    return 1;
  else if (btnFacingDown->isChecked())
    return 2;
  else if (btnFacingLeft->isChecked())
    return 3;
  return 4;
}

void ConfigureEvent_Teleport::on_treeMaps_itemClicked(QTreeWidgetItem *item)
{
  XMLNode mapNode = XMLNode::openFileHelper(QString("%1/maps/%2").arg(Globals::datadir).arg(item->text(1)).toLocal8Bit().data(), "ptad-map");
  if (map != nullptr)
    delete map;
  map = new Map(mapNode);
  if (mapImage != nullptr)
    delete mapImage;
  mapImage = new Image(QSize(map->getWidth() * 256, map->getHeight() * 256));
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->setImage(mapImage);
  location = QPoint(0, 0);
  updateMapView();
}

void ConfigureEvent_Teleport::on_imgMapView_mousePressed(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    location = QPoint(pos.x() / (zoom[currentZoom] / 16), pos.y() / (zoom[currentZoom] / 16));
    updateMapView();
  }
}

void ConfigureEvent_Teleport::on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta)
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

void ConfigureEvent_Teleport::on_imgMapView_mouseReleased(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    location = QPoint(pos.x() / (zoom[currentZoom] / 16), pos.y() / (zoom[currentZoom] / 16));
    updateMapView();
  }
}

void ConfigureEvent_Teleport::on_imgMapView_zoomOut(QPoint pos)
{
  if (currentZoom == 0)
    return;
  --currentZoom;
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->imageChanged();
  zoomPos = (pos / 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void ConfigureEvent_Teleport::on_imgMapView_zoomIn(QPoint pos)
{
  if (currentZoom == 4)
    return;
  ++currentZoom;
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->imageChanged();
  zoomPos = (pos * 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void ConfigureEvent_Teleport::updateZoom()
{
  panMapArea->horizontalScrollBar()->setValue(zoomPos.x());
  panMapArea->verticalScrollBar()->setValue(zoomPos.y());
}

void ConfigureEvent_Teleport::buildMapTree(QTreeWidgetItem *parent, QTreeWidgetItem *itemToCopy)
{
  for (int i = 0; i < itemToCopy->childCount(); ++i)
  {
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QTreeWidgetItem *child = itemToCopy->child(i);
    item->setText(0, child->text(0));
    item->setText(1, child->text(1));
    item->setText(2, child->text(2));
    parent->addChild(item);
    if (child->childCount() > 0)
      buildMapTree(item, child);
  }
}

void ConfigureEvent_Teleport::updateMapView()
{
  if (map == nullptr)
    return;
  if (mapImage == nullptr)
    return;
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
