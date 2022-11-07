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
#include "choosemapevent.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"
#include "mainwindow.h"
#include "mapevent.h"
#include "newgamedataeditor.h"
#include "tileset.h"

const int NewGameDataEditor::zoom[5] = {64, 128, 256, 512, 1024};

NewGameDataEditor::NewGameDataEditor(QWidget *parent) : QDialog(parent)
{
  QList<QTreeWidgetItem*> items;
  setupUi(this);
  map = nullptr;
  mapImage = nullptr;
  buildMapTree(treeMaps->invisibleRootItem(), Globals::mainwindow->treeMaps->invisibleRootItem());
  currentZoom = 2;
  leDefaultName->setText(Globals::defaultName);
  items = treeMaps->findItems(QString("map%1.xml").arg(Globals::startMapID, 4, 10, QChar('0')), Qt::MatchFixedString | Qt::MatchRecursive, 1);
  if (items.count() != 0)
  {
    items[0]->setSelected(true);
    treeMaps->setCurrentItem(items[0]);
    on_treeMaps_itemClicked(items[0]);
    panMapArea->ensureVisible(Globals::startLocation.x() * 16, Globals::startLocation.y() * 16);
    updateMapView();
  }
  if (Globals::startFacing == 0)
    btnFacingUp->setChecked(true);
  else if (Globals::startFacing == 1)
    btnFacingRight->setChecked(true);
  else if (Globals::startFacing == 2)
    btnFacingDown->setChecked(true);
  else if (Globals::startFacing == 3)
    btnFacingLeft->setChecked(true);
  Globals::newGameEvent->getTopLevelEvent()->setItem(treeEvents->invisibleRootItem());
  treeEvents->expandAll();
}

NewGameDataEditor::~NewGameDataEditor()
{
  if (map != nullptr)
    delete map;
  if (mapImage != nullptr)
    delete mapImage;
  treeEvents->invisibleRootItem()->takeChildren();
  Globals::newGameEvent->getTopLevelEvent()->setItem(nullptr);
}

void NewGameDataEditor::on_leDefaultName_textChanged(QString text)
{
  Globals::defaultName = text;
}

void NewGameDataEditor::on_btnFacingUp_toggled(bool on)
{
  if (on)
    Globals::startFacing = 0;
}

void NewGameDataEditor::on_btnFacingRight_toggled(bool on)
{
  if (on)
    Globals::startFacing = 1;
}

void NewGameDataEditor::on_btnFacingDown_toggled(bool on)
{
  if (on)
    Globals::startFacing = 2;
}

void NewGameDataEditor::on_btnFacingLeft_toggled(bool on)
{
  if (on)
    Globals::startFacing = 3;
}

void NewGameDataEditor::on_treeMaps_itemClicked(QTreeWidgetItem *item)
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
  Globals::startMapID = item->text(1).mid(3, 4).toInt();
  if (Globals::startLocation.x() >= map->getWidth() * 16)
    Globals::startLocation.setX(0);
  if (Globals::startLocation.y() >= map->getHeight() * 16)
    Globals::startLocation.setY(0);
  updateMapView();
}

void NewGameDataEditor::on_imgMapView_mousePressed(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    Globals::startLocation = QPoint(pos.x() / (zoom[currentZoom] / 16), pos.y() / (zoom[currentZoom] / 16));
    updateMapView();
  }
}

void NewGameDataEditor::on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta)
{
  if (buttons & Qt::LeftButton)
  {
    Globals::startLocation = QPoint(pos.x() / (zoom[currentZoom] / 16), pos.y() / (zoom[currentZoom] / 16));
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

void NewGameDataEditor::on_imgMapView_mouseReleased(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    Globals::startLocation = QPoint(pos.x() / (zoom[currentZoom] / 16), pos.y() / (zoom[currentZoom] / 16));
    updateMapView();
  }
}

void NewGameDataEditor::on_imgMapView_zoomOut(QPoint pos)
{
  if (currentZoom == 0)
    return;
  --currentZoom;
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->imageChanged();
  zoomPos = (pos / 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void NewGameDataEditor::on_imgMapView_zoomIn(QPoint pos)
{
  if (currentZoom == 4)
    return;
  ++currentZoom;
  imgMapView->setSize(map->getWidth() * zoom[currentZoom], map->getHeight() * zoom[currentZoom]);
  imgMapView->imageChanged();
  zoomPos = (pos * 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void NewGameDataEditor::updateZoom()
{
  panMapArea->horizontalScrollBar()->setValue(zoomPos.x());
  panMapArea->verticalScrollBar()->setValue(zoomPos.y());
}

void NewGameDataEditor::on_treeEvents_insertEvent(QTreeWidgetItem *item)
{
  MapEvent::Event *event = static_cast<MapEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  MapEvent::Event *parent = event->getParentEvent();
  MapEvent::Event *previousEvent = parent->previousChildEvent(event);
  MapEvent::Event *newEvent;
  ChooseMapEvent *eventChooser = new ChooseMapEvent((previousEvent != nullptr && previousEvent->isIf()), this);
  int type = eventChooser->exec();
  eventChooser->deleteLater();
  if (type == -1)
    return;
  newEvent = MapEvent::Event::newEvent(Globals::newGameEvent, parent, static_cast<MapEvent::Event::Type>(type));
  parent->addChildEvent(newEvent, parent->indexOfChildEvent(event));
  if (!newEvent->configureEvent(this))
    parent->deleteChildEvent(parent->indexOfChildEvent(newEvent));
  else
    newEvent->getItem();
}

void NewGameDataEditor::on_treeEvents_editEvent(QTreeWidgetItem *item)
{
  MapEvent::Event *event = static_cast<MapEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  event->configureEvent(this);
  event->getItem();
}

void NewGameDataEditor::on_treeEvents_deleteEvent(QTreeWidgetItem *item)
{
  MapEvent::Event *event = static_cast<MapEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  MapEvent::Event *parent = event->getParentEvent();
  if (event->getType() != MapEvent::Event::Type::End)
    parent->deleteChildEvent(parent->indexOfChildEvent(event));
}

void NewGameDataEditor::buildMapTree(QTreeWidgetItem *parent, QTreeWidgetItem *itemToCopy)
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

void NewGameDataEditor::updateMapView()
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
      if (tileID < 128)
        mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), true);
    }
  }
  mapImage->drawRect(QRect(Globals::startLocation.x() * 16, Globals::startLocation.y() * 16, 16, 16), 255);
  mapImage->drawRect(QRect(Globals::startLocation.x() * 16 + 1, Globals::startLocation.y() * 16 + 1, 14, 14), 255);
  imgMapView->imageChanged();
}
