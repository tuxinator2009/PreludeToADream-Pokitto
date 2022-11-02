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

#include "battleevent.h"
#include "choosebattleevent.h"
#include "globals.h"
#include "image.h"
#include "monster.h"
#include "monsterseditor.h"

MonstersEditor::MonstersEditor(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  currentMonsterID = -1;
  monster = nullptr;
  sprite = new Image(QSize(48, 48));
  imgSprite->setImage(sprite);
  imgSprite->setTransparent(true);
  currentMonsterID = -1;
  ignoreEvents = true;
  for (int i = 0; i < 256; ++i)
  {
    sprite->blitImage(*Globals::battlers, QRect(i % 16 * 48, i / 16 * 48, 48, 48), QPoint(0, 0), false);
    if (sprite->isEmpty())
      break;
    if (!QFile::exists(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(i, 3, 10, QChar('0'))))
      QFile::copy(QString("%1/battlers/template.xml").arg(Globals::datadir), QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(i, 3, 10, QChar('0')));
    lstMonsters->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(sprite->toQImage())), ""));
  }
  lstMonsters->setCurrentItem(lstMonsters->item(0));
  lstMonsters->item(0)->setSelected(true);
  ignoreEvents = false;
  on_lstMonsters_itemSelectionChanged();
  resize(sizeHint());
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());
}

MonstersEditor::~MonstersEditor()
{
  if (monster != nullptr)
  {
    monster->toXMLNode().writeToFile(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(currentMonsterID, 3, 10, QChar('0')).toLocal8Bit().data());
    delete monster;
  }
  delete sprite;
}

void MonstersEditor::on_lstMonsters_itemSelectionChanged()
{
  if (ignoreEvents)
    return;
  if (lstMonsters->currentRow() < 0)
    return;
  if (monster != nullptr)
  {
    monster->toXMLNode().writeToFile(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(currentMonsterID, 3, 10, QChar('0')).toLocal8Bit().data());
    delete monster;
  }
  ignoreEvents = true;
  currentMonsterID = lstMonsters->currentRow();
  monster = new Monster(XMLNode::parseFile(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(currentMonsterID, 3, 10, QChar('0')).toLocal8Bit().data(), "monster"));
  sprite->blitImage(*Globals::battlers, QRect(currentMonsterID % 16 * 48, currentMonsterID / 16 * 48, 48, 48), QPoint(0, 0), false);
  imgSprite->imageChanged();
  leName->setText(monster->getName());
  numExperience->setValue(monster->getExperience());
  numHP->setValue(monster->getHP());
  numAttack->setValue(monster->getAttack());
  numDefense->setValue(monster->getDefense());
  numAgility->setValue(monster->getAgility());
  numMagic->setValue(monster->getMagic());
  for (int i = 0; i < 7; ++i)
    lstSpells->item(i)->setIcon(QIcon(Globals::spellIcons[(monster->getSpellResistance() >> (i * 2)) & 3]));
  numPoison->setValue(monster->getPoisonResistance());
  numSlow->setValue(monster->getSlowResistance());
  numMP->setValue(monster->getMP());
  numGold->setValue(monster->getGold());
  monster->getIntroEvent()->getTopLevelEvent()->setItem(treeIntroEvents->invisibleRootItem());
  treeIntroEvents->expandAll();
  monster->getOutroEvent()->getTopLevelEvent()->setItem(treeOutroEvents->invisibleRootItem());
  treeOutroEvents->expandAll();
  monster->getTurnEvent()->getTopLevelEvent()->setItem(treeTurnEvents->invisibleRootItem());
  treeTurnEvents->expandAll();
  wMonsterData->setEnabled(true);
  ignoreEvents = false;
}

void MonstersEditor::on_leName_textChanged(QString value)
{
  if (ignoreEvents)
    return;
  monster->setName(value);
}

void MonstersEditor::on_numGold_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setGold(value);
}

void MonstersEditor::on_numExperience_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setExperience(value);
}

void MonstersEditor::on_numHP_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setHP(value);
}

void MonstersEditor::on_numMP_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setMP(value);
}

void MonstersEditor::on_numAttack_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setAttack(value);
}

void MonstersEditor::on_numDefense_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setDefense(value);
}

void MonstersEditor::on_numAgility_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setAgility(value);
}

void MonstersEditor::on_numMagic_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setMagic(value);
}

void MonstersEditor::on_lstSpells_itemClicked(QListWidgetItem *item)
{
  int spellID = lstSpells->row(item);
  uint16_t spellResistance = monster->getSpellResistance();
  uint16_t resistance = (spellResistance >> (spellID * 2)) & 3;
  spellResistance &= ~(3 << (spellID * 2));
  resistance = (resistance + 1) % 4;
  item->setIcon(QIcon(Globals::spellIcons[resistance]));
  resistance <<= spellID * 2;
  spellResistance |= resistance;
  monster->setSpellResistance(spellResistance);
}

void MonstersEditor::on_numPoison_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setPoisonResistance(value);
}

void MonstersEditor::on_numSlow_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  monster->setSlowResistance(value);
}

void MonstersEditor::on_treeIntroEvents_insertEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  BattleEvent::Event *parent = event->getParentEvent();
  BattleEvent::Event *previousEvent = parent->previousChildEvent(event);
  BattleEvent::Event *newEvent;
  ChooseBattleEvent *eventChooser = new ChooseBattleEvent((previousEvent != nullptr) && previousEvent->isIf(), this);
  int type = eventChooser->exec();
  eventChooser->deleteLater();
  if (type == -1)
    return;
  newEvent = BattleEvent::Event::newEvent(monster->getIntroEvent(), parent, static_cast<BattleEvent::Event::Type>(type));
  parent->addChildEvent(newEvent, parent->indexOfChildEvent(event));
  if (!newEvent->configureEvent(this))
    parent->deleteChildEvent(parent->indexOfChildEvent(newEvent));
  else
    newEvent->getItem();
}

void MonstersEditor::on_treeIntroEvents_editEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  event->configureEvent(this);
  event->getItem();
}

void MonstersEditor::on_treeIntroEvents_deleteEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  BattleEvent::Event *parent = event->getParentEvent();
  if (event->getType() != BattleEvent::Event::Type::End)
    parent->deleteChildEvent(parent->indexOfChildEvent(event));
}

void MonstersEditor::on_treeOutroEvents_insertEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  BattleEvent::Event *parent = event->getParentEvent();
  BattleEvent::Event *previousEvent = parent->previousChildEvent(event);
  BattleEvent::Event *newEvent;
  ChooseBattleEvent *eventChooser = new ChooseBattleEvent((previousEvent != nullptr) && previousEvent->isIf(), this);
  int type = eventChooser->exec();
  eventChooser->deleteLater();
  if (type == -1)
    return;
  newEvent = BattleEvent::Event::newEvent(monster->getOutroEvent(), parent, static_cast<BattleEvent::Event::Type>(type));
  parent->addChildEvent(newEvent, parent->indexOfChildEvent(event));
  if (!newEvent->configureEvent(this))
    parent->deleteChildEvent(parent->indexOfChildEvent(newEvent));
  else
    newEvent->getItem();
}

void MonstersEditor::on_treeOutroEvents_editEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  event->configureEvent(this);
  event->getItem();
}

void MonstersEditor::on_treeOutroEvents_deleteEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  BattleEvent::Event *parent = event->getParentEvent();
  if (event->getType() != BattleEvent::Event::Type::End)
    parent->deleteChildEvent(parent->indexOfChildEvent(event));
}

void MonstersEditor::on_treeTurnEvents_insertEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  BattleEvent::Event *parent = event->getParentEvent();
  BattleEvent::Event *previousEvent = parent->previousChildEvent(event);
  BattleEvent::Event *newEvent;
  ChooseBattleEvent *eventChooser = new ChooseBattleEvent((previousEvent != nullptr) && previousEvent->isIf(), this);
  int type = eventChooser->exec();
  eventChooser->deleteLater();
  if (type == -1)
    return;
  newEvent = BattleEvent::Event::newEvent(monster->getTurnEvent(), parent, static_cast<BattleEvent::Event::Type>(type));
  parent->addChildEvent(newEvent, parent->indexOfChildEvent(event));
  if (!newEvent->configureEvent(this))
    parent->deleteChildEvent(parent->indexOfChildEvent(newEvent));
  else
    newEvent->getItem();
}

void MonstersEditor::on_treeTurnEvents_editEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  event->configureEvent(this);
  event->getItem();
}

void MonstersEditor::on_treeTurnEvents_deleteEvent(QTreeWidgetItem *item)
{
  BattleEvent::Event *event = static_cast<BattleEvent::Event*>(item->data(1, Qt::UserRole).value<void*>());
  BattleEvent::Event *parent = event->getParentEvent();
  if (event->getType() != BattleEvent::Event::Type::End)
    parent->deleteChildEvent(parent->indexOfChildEvent(event));
}
