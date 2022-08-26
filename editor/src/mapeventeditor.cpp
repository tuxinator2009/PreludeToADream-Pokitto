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

#include "choosemapevent.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"
#include "mapevent.h"
#include "mapeventeditor.h"
#include "mapspritepicker.h"

MapEventEditor::MapEventEditor(MapEvent *e, bool onLoadEvent, QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  mapEvent = e;
  if (onLoadEvent)
    wEventData->setVisible(false);
  else
  {
    uint8_t spriteID = Globals::map->getSpriteID(e->getSpriteID());
    leName->setText(mapEvent->getName());
    chkButton->setChecked(mapEvent->isFlagSet(MapEvent::FLAGS_ACTIVATE_BUTTON));
    chkDoor->setChecked(mapEvent->isFlagSet(MapEvent::FLAGS_DOOR));
    chkExit->setChecked(mapEvent->isFlagSet(MapEvent::FLAGS_EXIT));
    sprite = new Image(*Globals::sprites, QRect(spriteID % 16 * 16, spriteID / 16 * 24, 16, 24));
    imgSprite->setSize(QSize(64, 96));
    imgSprite->setTransparent(true);
    imgSprite->setImage(sprite);
    imgSprite->imageChanged();
    spritePicker = new MapSpritePicker(Globals::map, this);
    connect(spritePicker, SIGNAL(spriteClicked(uint8_t)), this, SLOT(spriteClicked(uint8_t)));
  }
  //TODO: set sprite graphic
  mapEvent->getTopLevelEvent()->setItem(treeEvents->invisibleRootItem());
  treeEvents->expandAll();
}

MapEventEditor::~MapEventEditor()
{
  treeEvents->invisibleRootItem()->takeChildren();
  mapEvent->getTopLevelEvent()->setItem(nullptr);
}

void MapEventEditor::on_leName_textEdited(QString text)
{
  mapEvent->setName(text);
}

void MapEventEditor::on_chkButton_toggled(bool on)
{
  mapEvent->setFlag(MapEvent::FLAGS_ACTIVATE_BUTTON, on);
}

void MapEventEditor::on_chkDoor_toggled(bool on)
{
  mapEvent->setFlag(MapEvent::FLAGS_DOOR, on);
}

void MapEventEditor::on_chkExit_toggled(bool on)
{
  mapEvent->setFlag(MapEvent::FLAGS_EXIT, on);
}

void MapEventEditor::on_imgSprite_mouseDoubleClicked()
{
  spritePicker->move(imgSprite->mapToGlobal(QPoint(0, 0)));
  spritePicker->show();
}

void MapEventEditor::on_treeEvents_insertEvent(QTreeWidgetItem *item)
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
  newEvent = MapEvent::Event::newEvent(mapEvent, parent, static_cast<MapEvent::Event::Type>(type));
  parent->addChildEvent(newEvent, parent->indexOfChildEvent(event));
  if (!newEvent->configureEvent(this))
    parent->deleteChildEvent(parent->indexOfChildEvent(newEvent));
  else
    newEvent->getItem();
}

void MapEventEditor::on_treeEvents_editEvent(QTreeWidgetItem *item)
{
  MapEvent::Event *event = static_cast<MapEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  event->configureEvent(this);
  event->getItem();
}

void MapEventEditor::on_treeEvents_deleteEvent(QTreeWidgetItem *item)
{
  MapEvent::Event *event = static_cast<MapEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  MapEvent::Event *parent = event->getParentEvent();
  if (event->getType() != MapEvent::Event::Type::End)
    parent->deleteChildEvent(parent->indexOfChildEvent(event));
}

void MapEventEditor::spriteClicked(uint8_t spriteID)
{
  uint8_t globalSpriteID = Globals::map->getSpriteID(spriteID);
  mapEvent->setSpriteID(spriteID);
  sprite->fillRect(QRect(0, 0, 16, 24), 0);
  sprite->blitImage(*Globals::sprites, QRect(globalSpriteID % 16 * 16, globalSpriteID / 16 * 24, 16, 24), QPoint(0, 0), true);
  imgSprite->imageChanged();
  spritePicker->hide();
}
