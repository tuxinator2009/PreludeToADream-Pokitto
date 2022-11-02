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

#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include "battleevent.h"
#include "configureevent_buffercharacter.h"
#include "configureevent_buffervalue.h"
#include "configureevent_showmessage.h"
#include "configureevent_playsoundeffect.h"
#include "configureevent_shakescreen.h"
#include "globals.h"

//BEGIN: BattleEvent (COMPLETE)
XMLNode BattleEvent::toXMLNode(const char *name)
{
  XMLNode eventNode = topLevelEvent->toXMLNode();
  eventNode.updateName(name);
  return eventNode;
}

void BattleEvent::compileEvent(QByteArray *bytes)
{
  topLevelEvent->compileEvent(bytes);
  for (auto jump : jumps)
  {
    Globals::Value32 value;
    value.uValue = jump.event->getOffset();
    for (int i = 0; i < 4; ++i)
      (*bytes)[jump.pos + i] = value.bytes[i];
  }
  jumps.clear();
}
//END: BattleEvent

//BEGIN: Event (COMPLETE)
const char *BattleEvent::Event::typeString[] =
{
  "shakeScreen",
  "flashBattler",
  "flashUi",
  "basicAttack",
  "useSkill",
  "castSpell",
  "playSoundEffect",
  "bufferMessage",
  "bufferValue",
  "bufferCharacter",
  "showMessage",
  "jump",
  "ifCounter",
  "ifStatus",
  "ifStat",
  "changeBattlerSprite",
  "changeBackgroundImage",
  "playBattleAnimation",
  "waitFrames",
  "waitButtons",
  "inflictStatus",
  "consumeMP",
  "random",
  "endEventProcessing",
  "label",
  "comment",
  "else",
  ""
};

const char *BattleEvent::Event::bufferValues[] =
{
  "playerName",
  "playerHp",
  "playerMaxHP",
  "playerMp",
  "playerMaxMP",
  "playerXp",
  "playerNextLevel",
  "playerXpLeft",
  "playerGold",
  "playerLevel",
  "playerAttack",
  "playerDefense",
  "playerAgility",
  "playerMagic",
  "monsterName",
  "monsterHP",
  "monsterMaxHP",
  "monsterMp",
  "monsterMaxMP",
  "monsterExperience",
  "monsterGold",
  "monsterAttack",
  "monsterDefense",
  "monsterAgility",
  "monsterMagic",
  "variable",
  "supplied"
};

const char *BattleEvent::Event::conditions[] = {"=","!=",">",">=","<","<="};
const char *BattleEvent::Event::statuses[] = {"poison", "speed", "focus", "berserk"};

BattleEvent::Event::Event(BattleEvent *pBase, Event *p, Type t)
{
  if (t != Type::TopLevelEvent)
  {
    item = new QTreeWidgetItem();
    item->setData(1, Qt::UserRole, QVariant::fromValue(static_cast<void*>(this)));
    item->setExpanded(true);
  }
  pBattleEvent = pBase;
  parent = p;
  type = t;
  pBattleEvent->allEvents += this;
}

BattleEvent::Event::~Event()
{
  if (type != Type::TopLevelEvent)
    delete item;
  pBattleEvent->allEvents.removeAll(this);
}

BattleEvent::Event *BattleEvent::Event::newEvent(BattleEvent *pBase, Event *parent, Type t)
{
  if (t == Type::ShakeScreen)
    return new ShakeScreen(pBase, parent);
  if (t == Type::FlashBattler)
    return new FlashBattler(pBase, parent);
  if (t == Type::FlashUi)
    return new FlashUi(pBase, parent);
  if (t == Type::BasicAttack)
    return new BasicAttack(pBase, parent);
  if (t == Type::UseSkill)
    return new UseSkill(pBase, parent);
  if (t == Type::CastSpell)
    return new CastSpell(pBase, parent);
  if (t == Type::PlaySoundEffect)
    return new PlaySoundEffect(pBase, parent);
  if (t == Type::BufferMessage)
    return new BufferMessage(pBase, parent);
  if (t == Type::BufferValue)
    return new BufferValue(pBase, parent);
  if (t == Type::BufferCharacter)
    return new BufferCharacter(pBase, parent);
  if (t == Type::ShowMessage)
    return new ShowMessage(pBase, parent);
  if (t == Type::Jump)
    return new Jump(pBase, parent);
  if (t == Type::IfCounter)
    return new IfCounter(pBase, parent);
  if (t == Type::IfStatus)
    return new IfStatus(pBase, parent);
  if (t == Type::IfStat)
    return new IfStat(pBase, parent);
  if (t == Type::ChangeBattlerSprite)
    return new ChangeBattlerSprite(pBase, parent);
  if (t == Type::ChangeBackgroundImage)
    return new ChangeBackgroundImage(pBase, parent);
  if (t == Type::PlayBattleAnimation)
    return new PlayBattleAnimation(pBase, parent);
  if (t == Type::WaitFrames)
    return new WaitFrames(pBase, parent);
  if (t == Type::WaitButtons)
    return new Event(pBase, parent, t);
  if (t == Type::InflictStatus)
    return new InflictStatus(pBase, parent);
  if (t == Type::ConsumeMP)
    return new ConsumeMP(pBase, parent);
  if (t == Type::Random)
    return new Random(pBase, parent);
  if (t == Type::EndEventProcessing)
    return new Event(pBase, parent, t);
  if (t == Type::Label)
    return new Label(pBase, parent);
  if (t == Type::Comment)
    return new Comment(pBase, parent);
  if (t == Type::Else)
    return new Else(pBase, parent);
  if (t == Type::End)
    return new End(pBase, parent);
  return nullptr;
}

BattleEvent::Event *BattleEvent::Event::newEvent(BattleEvent *pBase, Event *parent, XMLNode eventNode)
{
  std::string eventType = eventNode.getName();
  if (eventType.compare("shakeScreen") == 0)
    return new ShakeScreen(pBase, parent, eventNode);
  if (eventType.compare("flashBattler") == 0)
    return new FlashBattler(pBase, parent, eventNode);
  if (eventType.compare("flashUi") == 0)
    return new FlashUi(pBase, parent, eventNode);
  if (eventType.compare("basicAttack") == 0)
    return new BasicAttack(pBase, parent, eventNode);
  if (eventType.compare("useSkill") == 0)
    return new UseSkill(pBase, parent, eventNode);
  if (eventType.compare("castSpell") == 0)
    return new CastSpell(pBase, parent, eventNode);
  if (eventType.compare("playSoundEffect") == 0)
    return new PlaySoundEffect(pBase, parent, eventNode);
  if (eventType.compare("bufferMessage") == 0)
    return new BufferMessage(pBase, parent, eventNode);
  if (eventType.compare("bufferValue") == 0)
    return new BufferValue(pBase, parent, eventNode);
  if (eventType.compare("bufferCharacter") == 0)
    return new BufferCharacter(pBase, parent, eventNode);
  if (eventType.compare("showMessage") == 0)
    return new ShowMessage(pBase, parent, eventNode);
  if (eventType.compare("jump") == 0)
    return new Jump(pBase, parent, eventNode);
  if (eventType.compare("ifCounter") == 0)
    return new IfCounter(pBase, parent, eventNode);
  if (eventType.compare("ifStatus") == 0)
    return new IfStatus(pBase, parent, eventNode);
  if (eventType.compare("ifStat") == 0)
    return new IfStat(pBase, parent, eventNode);
  if (eventType.compare("changeBattlerSprite") == 0)
    return new ChangeBattlerSprite(pBase, parent, eventNode);
  if (eventType.compare("changeBackgroundImage") == 0)
    return new ChangeBackgroundImage(pBase, parent, eventNode);
  if (eventType.compare("playBattleAnimation") == 0)
    return new PlayBattleAnimation(pBase, parent, eventNode);
  if (eventType.compare("waitFrames") == 0)
    return new WaitFrames(pBase, parent, eventNode);
  if (eventType.compare("waitButtons") == 0)
    return new Event(pBase, parent, Type::WaitButtons);
  if (eventType.compare("inflictStatus") == 0)
    return new InflictStatus(pBase, parent, eventNode);
  if (eventType.compare("consumeMP") == 0)
    return new ConsumeMP(pBase, parent, eventNode);
  if (eventType.compare("random") == 0)
    return new Random(pBase, parent, eventNode);
  if (eventType.compare("endEventProcessing") == 0)
    return new Event(pBase, parent, Type::EndEventProcessing);
  if (eventType.compare("label") == 0)
    return new Label(pBase, parent, eventNode);
  if (eventType.compare("comment") == 0)
    return new Comment(pBase, parent, eventNode);
  if (eventType.compare("else") == 0)
    return new Else(pBase, parent, eventNode);
  return nullptr;
}
//END: Event

//BEGIN: TopLevelEvent (COMPLETE)
BattleEvent::TopLevelEvent::TopLevelEvent(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::TopLevelEvent)
{
  for (int i = 0; i < eventNode.nChildNode(); ++i)
    events += Event::newEvent(pBase, this, eventNode.getChildNode(i));
  events += Event::newEvent(pBase, this, Type::End);
}

BattleEvent::TopLevelEvent::~TopLevelEvent()
{
  for (auto event : events)
    delete event;
  events.clear();
}

XMLNode BattleEvent::TopLevelEvent::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("event");
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void BattleEvent::TopLevelEvent::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  for (auto event : events)
    event->compileEvent(bytes);
  delete events.takeLast();
}

BattleEvent::Event *BattleEvent::TopLevelEvent::previousChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == 0)
    return nullptr;
  return events[index - 1];
}

BattleEvent::Event *BattleEvent::TopLevelEvent::nextChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == events.count() - 1)
    return nullptr;
  return events[index + 1];
}

void BattleEvent::TopLevelEvent::setItem(QTreeWidgetItem *value)
{
  if (value == nullptr && item != nullptr)
    item->takeChildren();
  item = value;
  if (item != nullptr)
  {
    for (auto event : events)
      item->addChild(event->getItem());
  }
}
//END: TopLevelEvent

//BEGIN: IfEvent (COMPLETE)
BattleEvent::IfEvent::IfEvent(BattleEvent *pBase, Event *parent, Type t, XMLNode eventNode) : Event(pBase, parent, t)
{
  elseIf = strncmp(eventNode.getName(), "else", 4) == 0;
  for (int i = 0; i < eventNode.nChildNode(); ++i)
    events += Event::newEvent(pBase, this, eventNode.getChildNode(i));
  events += Event::newEvent(pBase, this, Type::End);
}

BattleEvent::IfEvent::~IfEvent()
{
  for (auto event : events)
    delete event;
  events.clear();
}

BattleEvent::Event *BattleEvent::IfEvent::previousChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == 0)
    return nullptr;
  return events[index - 1];
}

BattleEvent::Event *BattleEvent::IfEvent::nextChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == events.count() - 1)
    return nullptr;
  return events[index + 1];
}

void BattleEvent::IfEvent::addItems(QTreeWidgetItem *item)
{
  for (auto event : events)
    item->addChild(event->getItem());
}
//END: IfEvent

//BEGIN: ShakeScreen (COMPLETE)
BattleEvent::ShakeScreen::ShakeScreen(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::ShakeScreen)
{
  duration = 0;
  rate = 0;
}

BattleEvent::ShakeScreen::ShakeScreen(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::ShakeScreen)
{
  duration = atoi(eventNode.getAttribute("duration"));
  rate = atoi(eventNode.getAttribute("rate"));
}

BattleEvent::ShakeScreen::~ShakeScreen()
{
}

XMLNode BattleEvent::ShakeScreen::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("shakeScreen");
  eventNode.addAttribute("duration", QString::number(duration).toLocal8Bit().data());
  eventNode.addAttribute("rate", QString::number(rate).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::ShakeScreen::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(duration);
  bytes->append(rate);
}

bool BattleEvent::ShakeScreen::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ShakeScreen *configure = new ConfigureEvent_ShakeScreen(false, parentWidget);
  configure->setDuration(duration);
  configure->setMagnitude(rate);
  if (configure->exec())
  {
    duration = configure->getDuration();
    rate = configure->getMagnitude();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *BattleEvent::ShakeScreen::getItem()
{
  item->setText(0, QString("<> shakeScreen: duration=%1 rate=%2").arg(duration).arg(rate));
  return item;
}
//END: ShakeScreen

//BEGIN: FlashBattler (NEEDS EDITOR)
BattleEvent::FlashBattler::FlashBattler(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::FlashBattler)
{
  duration = 0;
  color1 = 0;
  color2 = 0;
  color3 = 0;
}

BattleEvent::FlashBattler::FlashBattler(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::FlashBattler)
{
  duration = atoi(eventNode.getAttribute("duration"));
  color1 = atoi(eventNode.getAttribute("color1"));
  color2 = atoi(eventNode.getAttribute("color2"));
  color3 = atoi(eventNode.getAttribute("color3"));
}

BattleEvent::FlashBattler::~FlashBattler()
{
}

XMLNode BattleEvent::FlashBattler::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("flashBattler");
  eventNode.addAttribute("duration", QString::number(duration).toLocal8Bit().data());
  eventNode.addAttribute("color1", QString::number(color1).toLocal8Bit().data());
  eventNode.addAttribute("color2", QString::number(color2).toLocal8Bit().data());
  eventNode.addAttribute("color3", QString::number(color3).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::FlashBattler::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(duration);
  bytes->append(color1);
  bytes->append(color2);
  bytes->append(color3);
}

bool BattleEvent::FlashBattler::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::FlashBattler::getItem()
{
  item->setText(0, QString("<> flashBattler: duration=%1 color=(%2, %3, %4)").arg(duration).arg(color1).arg(color2).arg(color3));
  return item;
}
//END: FlashBattler

//BEGIN: FlashUi (NEEDS EDITOR)
BattleEvent::FlashUi::FlashUi(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::FlashUi)
{
  duration = 0;
  color = 0;
}

BattleEvent::FlashUi::FlashUi(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::FlashUi)
{
  duration = atoi(eventNode.getAttribute("duration"));
  color = atoi(eventNode.getAttribute("color"));
}

BattleEvent::FlashUi::~FlashUi()
{
}

XMLNode BattleEvent::FlashUi::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("flashBattler");
  eventNode.addAttribute("duration", QString::number(duration).toLocal8Bit().data());
  eventNode.addAttribute("color", QString::number(color).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::FlashUi::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(duration);
  bytes->append(color);
}

bool BattleEvent::FlashUi::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::FlashUi::getItem()
{
  item->setText(0, QString("<> flashBattler: duration=%1 color=%2").arg(duration).arg(color));
  return item;
}
//END: FlashUi

//BEGIN: BasicAttack (COMPLETE)
BattleEvent::BasicAttack::BasicAttack(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::BasicAttack)
{
  QDir folder(QString("%1/animations").arg(Globals::datadir));
  QStringList animations = folder.entryList(QStringList() << "*.anim", QDir::Files, QDir::Name);
  animations.replaceInStrings(".anim", "", Qt::CaseInsensitive);
  animation = animations[0];
}

BattleEvent::BasicAttack::BasicAttack(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::BasicAttack)
{
  animation = eventNode.getAttribute("animation");
}

BattleEvent::BasicAttack::~BasicAttack()
{
}

XMLNode BattleEvent::BasicAttack::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("flashBattler");
  eventNode.addAttribute("animation", animation.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::BasicAttack::compileEvent(QByteArray *bytes)
{
  QString location = QString("animations/%1.anim").arg(animation);
  Globals::Value32 value32;
  offset = bytes->count();
  value32.uValue = Globals::hash(location.toLocal8Bit().data(), location.size());
  bytes->append(static_cast<char>(type));
  bytes->append(value32.bytes[0]);
  bytes->append(value32.bytes[1]);
  bytes->append(value32.bytes[2]);
  bytes->append(value32.bytes[3]);
}

bool BattleEvent::BasicAttack::configureEvent(QWidget *parentWidget)
{
  QDir folder(QString("%1/animations").arg(Globals::datadir));
  QStringList animations = folder.entryList(QStringList() << "*.anim", QDir::Files, QDir::Name);
  QString choice;
  bool ok;
  animations.replaceInStrings(".anim", "", Qt::CaseInsensitive);
  choice = QInputDialog::getItem(parentWidget, "Basic Attack", "Animation", animations, animations.indexOf(animation), false, &ok);
  if (ok)
    animation = choice;
  return ok;
}

QTreeWidgetItem *BattleEvent::BasicAttack::getItem()
{
  item->setText(0, QString("<> basicAttack: %1").arg(animation));
  return item;
}
//END: BasicAttack

//BEGIN: UseSkill (NOT IMPLEMENTED IN ENGINE)
BattleEvent::UseSkill::UseSkill(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::UseSkill)
{
}

BattleEvent::UseSkill::UseSkill(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::UseSkill)
{
}

BattleEvent::UseSkill::~UseSkill()
{
}

XMLNode BattleEvent::UseSkill::toXMLNode()
{
  return XMLNode::createXMLTopNode("useSkill");
}

void BattleEvent::UseSkill::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
}

bool BattleEvent::UseSkill::configureEvent(QWidget *parentWidget)
{
  return false;
}

QTreeWidgetItem *BattleEvent::UseSkill::getItem()
{
  item->setText(0, QString("<> useSkill"));
  return item;
}
//END: UseSkill

//BEGIN: CastSpell (NEEDS EDITOR)
BattleEvent::CastSpell::CastSpell(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::CastSpell)
{
  QDir folder(QString("%1/animations").arg(Globals::datadir));
  QStringList animations = folder.entryList(QStringList() << "*.anim", QDir::Files, QDir::Name);
  animations.replaceInStrings(".anim", "", Qt::CaseInsensitive);
  animation = animations[0];
  spellType = 0;
  level = 1;
  mp = 1;
}

BattleEvent::CastSpell::CastSpell(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::CastSpell)
{
  animation = eventNode.getAttribute("animation");
  spellType = QString(eventNode.getAttribute("type")).toInt();
  level = QString(eventNode.getAttribute("level")).toInt();
  mp = QString(eventNode.getAttribute("mp")).toInt();
}

BattleEvent::CastSpell::~CastSpell()
{
}

XMLNode BattleEvent::CastSpell::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("castSpell");
  eventNode.addAttribute("animation", animation.toLocal8Bit().data());
  eventNode.addAttribute("type", QString::number(spellType).toLocal8Bit().data());
  eventNode.addAttribute("level", QString::number(level).toLocal8Bit().data());
  eventNode.addAttribute("mp", QString::number(mp).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::CastSpell::compileEvent(QByteArray *bytes)
{
  QString location = QString("animations/%1.anim").arg(animation);
  Globals::Value32 value32;
  offset = bytes->count();
  value32.uValue = Globals::hash(location.toLocal8Bit().data(), location.size());
  bytes->append(static_cast<char>(type));
  bytes->append(value32.bytes[0]);
  bytes->append(value32.bytes[1]);
  bytes->append(value32.bytes[2]);
  bytes->append(value32.bytes[3]);
  bytes->append(spellType);
  bytes->append(level);
  bytes->append(mp);
}

bool BattleEvent::CastSpell::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::CastSpell::getItem()
{
  item->setText(0, QString("<> castSpell: animation=%1 type=%2 level=%3 mp=%4").arg(animation).arg(spellType).arg(level).arg(mp));
  return item;
}
//END: CastSpell

//BEGIN: PlaySoundEffect (COMPLETE)
BattleEvent::PlaySoundEffect::PlaySoundEffect(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::PlaySoundEffect)
{
  sfx = Globals::sfx[0];
  loop = false;
}

BattleEvent::PlaySoundEffect::PlaySoundEffect(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::PlaySoundEffect)
{
  sfx = eventNode.getAttribute("sound");
  loop = strcmp(eventNode.getAttribute("loop"), "true") == 0;
}

BattleEvent::PlaySoundEffect::~PlaySoundEffect()
{
}

XMLNode BattleEvent::PlaySoundEffect::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("playSoundEffect");
  eventNode.addAttribute("sound", sfx.toLocal8Bit().data());
  eventNode.addAttribute("loop", (loop) ? "true":"false");
  return eventNode;
}

void BattleEvent::PlaySoundEffect::compileEvent(QByteArray *bytes)
{
  uint8_t sfxID = Globals::sfx.indexOf(sfx);
  offset = bytes->count();
  if (loop)
    sfxID |= 0x80;
  bytes->append(static_cast<char>(type));
  bytes->append(sfxID);
}

bool BattleEvent::PlaySoundEffect::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_PlaySoundEffect *configure = new ConfigureEvent_PlaySoundEffect(parentWidget);
  configure->setSoundEffect(sfx);
  configure->setLoop(loop);
  if (configure->exec())
  {
    sfx = configure->getSoundEffect();
    loop = configure->getLoop();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *BattleEvent::PlaySoundEffect::getItem()
{
  item->setText(0, QString("<> playSoundEffect: %1%2").arg(sfx).arg((loop) ? " (loop)":""));
  return item;
}
//END: PlaySoundEffect

//BEGIN: BufferMessage (COMPLETE)
BattleEvent::BufferMessage::BufferMessage(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::BufferMessage)
{
  message = "";
}

BattleEvent::BufferMessage::BufferMessage(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::BufferMessage)
{
  message = eventNode.getAttribute("message");
}

BattleEvent::BufferMessage::~BufferMessage()
{
}

XMLNode BattleEvent::BufferMessage::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("bufferMessage");
  eventNode.addAttribute("message", message.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::BufferMessage::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append((uint8_t)message.length());
  for (int i = 0; i < message.length(); ++i)
    bytes->append(Globals::charToCode(message[i].toLatin1()));
}

bool BattleEvent::BufferMessage::configureEvent(QWidget *parentWidget)
{
  QString value;
  bool ok;
  value = QInputDialog::getText(parentWidget, "Buffer Message", "Message:", QLineEdit::Normal, message, &ok);
  if (ok)
  {
    if (value.length() > 24)
    {
      QMessageBox::critical(parentWidget, "Message Too Long", "The message you entered is too long.\nMax length is 24 characters.");
      return false;
    }
    message = value;
    return true;
  }
  return false;
}

QTreeWidgetItem *BattleEvent::BufferMessage::getItem()
{
  item->setText(0, QString("<> bufferMessage: \"%1\"").arg(message));
  return item;
}
//END: BufferMessage

//BEGIN: BufferValue (COMPLETE)
BattleEvent::BufferValue::BufferValue(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::BufferValue)
{
  valueType = VALUE_PLAYER_NAME;
  value = 0;
}

BattleEvent::BufferValue::BufferValue(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::BufferValue)
{
  valueType = atoi(eventNode.getAttribute("value"));
  if (valueType == VALUE_VARIABLE)
    value = atoi(eventNode.getAttribute("variable"));
  else if (valueType == VALUE_SUPPLIED)
    value = atoi(eventNode.getAttribute("supplied"));
}

BattleEvent::BufferValue::~BufferValue()
{
}

XMLNode BattleEvent::BufferValue::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("bufferValue");
  eventNode.addAttribute("value", QString::number(valueType).toLocal8Bit().data());
  if (valueType == VALUE_VARIABLE)
    eventNode.addAttribute("variable", QString::number(value).toLocal8Bit().data());
  else if (valueType == VALUE_SUPPLIED)
    eventNode.addAttribute("supplied", QString::number(value).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::BufferValue::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(valueType);
  if (valueType == VALUE_VARIABLE)
    bytes->append((uint8_t)value);
  else if (valueType == VALUE_SUPPLIED)
  {
    Globals::Value32 v = {.sValue = value};
    bytes->append(v.bytes[0]);
    bytes->append(v.bytes[1]);
    bytes->append(v.bytes[2]);
    bytes->append(v.bytes[3]);
  }
}

bool BattleEvent::BufferValue::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_BufferValue *configure = new ConfigureEvent_BufferValue(false, parentWidget);
  configure->setValue(valueType);
  if (valueType == VALUE_VARIABLE)
    configure->setVariableID(value);
  else if (valueType == VALUE_SUPPLIED)
    configure->setSuppliedValue(value);
  if (configure->exec())
  {
    valueType = configure->getValue();
    if (valueType == VALUE_VARIABLE)
      value = configure->getVariableID();
    else if (valueType == VALUE_SUPPLIED)
      value = configure->getSuppliedValue();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *BattleEvent::BufferValue::getItem()
{
  QString text = QString("<> bufferValue: %1").arg(bufferValues[valueType]);
  if (valueType == VALUE_VARIABLE || valueType == VALUE_SUPPLIED)
    text += QString(" (%1)").arg(value);
  item->setText(0, text);
  return item;
}
//END: BufferValue

//BEGIN: BufferCharacter (COMPLETE)
BattleEvent::BufferCharacter::BufferCharacter(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::BufferCharacter)
{
  character = ' ';
}

BattleEvent::BufferCharacter::BufferCharacter(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::BufferCharacter)
{
  character = eventNode.getAttribute("char")[0];
}

BattleEvent::BufferCharacter::~BufferCharacter()
{
}

XMLNode BattleEvent::BufferCharacter::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("bufferCharacter");
  char text[2] = {character, '\0'};
  eventNode.addAttribute("char", text);
  return eventNode;
}

void BattleEvent::BufferCharacter::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(Globals::charToCode(character));
}

bool BattleEvent::BufferCharacter::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_BufferCharacter *configure = new ConfigureEvent_BufferCharacter(parentWidget);
  if (configure->exec())
  {
    character = configure->getCharacter();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *BattleEvent::BufferCharacter::getItem()
{
  item->setText(0, QString("<> bufferCharacter: \'%1\'").arg(character));
  return item;
}
//END: BufferCharacter

//BEGIN: ShowMessage (COMPLETE)
BattleEvent::ShowMessage::ShowMessage(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::ShowMessage)
{
}

BattleEvent::ShowMessage::ShowMessage(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::ShowMessage)
{
  for (int i = 0; i < eventNode.nChildNode("line"); ++i)
    message += eventNode.getChildNode("line", i).getAttribute("message");
}

BattleEvent::ShowMessage::~ShowMessage()
{
}

XMLNode BattleEvent::ShowMessage::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("showMessage");
  for (int i = 0; i < message.count(); ++i)
    eventNode.addChild("line").addAttribute("message", message[i].toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::ShowMessage::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  if (message.count() > 0)
  {
    for (int i = 0; i < message.count(); ++i)
    {
      QString line = "";
      for (int j = 0; j < message[i].length(); ++j)
      {
        if (message[i][j] == '_')
        {
          bufferMessage(bytes, line);
          line = "";
          bytes->append(static_cast<char>(Type::BufferValue));
          if (message[i].mid(j).startsWith("__NAME__"))
          {
            bytes->append(VALUE_PLAYER_NAME);
            j += 7;
          }
          else if (message[i].mid(j).startsWith("_hp"))
          {
            bytes->append(VALUE_PLAYER_HP);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_HP"))
          {
            bytes->append(VALUE_PLAYER_MAXHP);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_m"))
          {
            bytes->append(VALUE_PLAYER_MP);
            j += 1;
          }
          else if (message[i].mid(j).startsWith("_M"))
          {
            bytes->append(VALUE_PLAYER_MAXMP);
            j += 1;
          }
          else if (message[i].mid(j).startsWith("__XP__"))
          {
            bytes->append(VALUE_PLAYER_EXPERIENCE);
            j += 5;
          }
          else if (message[i].mid(j).startsWith("_NLVL_"))
          {
            bytes->append(VALUE_PLAYER_NEXTLEVEL);
            j += 5;
          }
          else if (message[i].mid(j).startsWith("_XPLFT"))
          {
            bytes->append(VALUE_PLAYER_EXPERIENCELEFT);
            j += 5;
          }
          else if (message[i].mid(j).startsWith("_GLD_"))
          {
            bytes->append(VALUE_PLAYER_GOLD);
            j += 4;
          }
          else if (message[i].mid(j).startsWith("_L"))
          {
            bytes->append(VALUE_PLAYER_LEVEL);
            j += 1;
          }
          else if (message[i].mid(j).startsWith("_AT"))
          {
            bytes->append(VALUE_PLAYER_ATTACK);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_DF"))
          {
            bytes->append(VALUE_PLAYER_DEFENSE);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_AG"))
          {
            bytes->append(VALUE_PLAYER_AGILITY);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_PW"))
          {
            bytes->append(VALUE_PLAYER_MAGIC);
            j += 2;
          }
          static constexpr uint8_t VALUE_MONSTER_NAME = 14;
          static constexpr uint8_t VALUE_MONSTER_HP = 15;
          static constexpr uint8_t VALUE_MONSTER_MAXHP = 16;
          static constexpr uint8_t VALUE_MONSTER_MP = 17;
          static constexpr uint8_t VALUE_MONSTER_MAXMP = 18;
          static constexpr uint8_t VALUE_MONSTER_EXPERIENCE = 19;
          static constexpr uint8_t VALUE_MONSTER_GOLD = 20;
          static constexpr uint8_t VALUE_MONSTER_ATTACK = 21;
          static constexpr uint8_t VALUE_MONSTER_DEFENSE = 22;
          static constexpr uint8_t VALUE_MONSTER_AGILITY = 23;
          static constexpr uint8_t VALUE_MONSTER_MAGIC = 24;
          if (message[i].mid(j).startsWith("__name__"))
          {
            bytes->append(VALUE_MONSTER_NAME);
            j += 7;
          }
          else if (message[i].mid(j).startsWith("_mh"))
          {
            bytes->append(VALUE_MONSTER_HP);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_MH"))
          {
            bytes->append(VALUE_MONSTER_MAXHP);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_p"))
          {
            bytes->append(VALUE_MONSTER_MP);
            j += 1;
          }
          else if (message[i].mid(j).startsWith("_P"))
          {
            bytes->append(VALUE_MONSTER_MAXMP);
            j += 1;
          }
          else if (message[i].mid(j).startsWith("__xp__"))
          {
            bytes->append(VALUE_MONSTER_EXPERIENCE);
            j += 5;
          }
          else if (message[i].mid(j).startsWith("_g_"))
          {
            bytes->append(VALUE_MONSTER_GOLD);
            j += 4;
          }
          else if (message[i].mid(j).startsWith("_at"))
          {
            bytes->append(VALUE_MONSTER_ATTACK);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_df"))
          {
            bytes->append(VALUE_MONSTER_DEFENSE);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_ag"))
          {
            bytes->append(VALUE_MONSTER_AGILITY);
            j += 2;
          }
          else if (message[i].mid(j).startsWith("_pw"))
          {
            bytes->append(VALUE_MONSTER_MAGIC);
            j += 2;
          }
        }
        else
          line += message[i][j];
      }
      bufferMessage(bytes, line);
      bytes->append(static_cast<char>(Type::ShowMessage));
      if (i % 4 == 3)
        bytes->append(static_cast<char>(Type::WaitButtons));
    }
    if (message.count() % 4 != 3)
      bytes->append(static_cast<char>(Type::WaitButtons));
  }
  else
    bytes->append(static_cast<char>(type));
}

bool BattleEvent::ShowMessage::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ShowMessage *configure = new ConfigureEvent_ShowMessage(parentWidget);
  configure->setMessage(message.join('\n'));
  if (configure->exec())
  {
    QString text = configure->getMessage();
    if (text.length() == 0)
      message = QStringList();
    else
      message = text.split('\n');
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *BattleEvent::ShowMessage::getItem()
{
  QString text = "<> showMessage";
  if (message.count() > 0)
  {
    text += ":\n  /------------------------\\\n";
    for (int i = 0; i < message.count(); ++i)
    {
      text += QString("  |%1|\n").arg(message[i], -24);
      if (i % 4 == 3 && i < message.count() - 1)
        text += "  |------------------------|\n";
    }
    text += "  \\------------------------/";
  }
  item->setText(0, text);
  return item;
}

void BattleEvent::ShowMessage::bufferMessage(QByteArray *bytes, QString line)
{
  if (line.length() == 0)
    return;
  bytes->append(static_cast<char>(Type::BufferMessage));
  bytes->append((uint8_t)line.length());
  for (int i = 0; i < line.length(); ++i)
    bytes->append(Globals::charToCode(line[i].toLatin1()));
}
//END: ShowMessage

//BEGIN: Jump (COMPLETE)
BattleEvent::Jump::Jump(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::Jump)
{
  label = "";
}

BattleEvent::Jump::Jump(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::Jump)
{
  label = eventNode.getAttribute("label");
}

BattleEvent::Jump::~Jump()
{
}

XMLNode BattleEvent::Jump::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("jump");
  eventNode.addAttribute("label", label.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::Jump::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  for (auto event : pBattleEvent->allEvents)
  {
    if (event->getType() == Type::Label && ((Label*)event)->getName() == label)
    {
      JumpLocation jump;
      jump.event = event;
      jump.pos = bytes->count();
      pBattleEvent->jumps += jump;
      break;
    }
  }
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
}

bool BattleEvent::Jump::configureEvent(QWidget *parentWidget)
{
  QStringList labels;
  QString newValue;
  bool ok;
  for (auto event : pBattleEvent->allEvents)
  {
    if (event->getType() == Type::Label)
      labels += ((Label*)event)->getName();
  }
  newValue = QInputDialog::getItem(parentWidget, "Jump to Label", "Label:", labels, labels.indexOf(label), true, &ok);
  if (ok)
    label = newValue;
  getItem();
  return ok;
}

QTreeWidgetItem *BattleEvent::Jump::getItem()
{
  if (item == nullptr)
    item = new QTreeWidgetItem;
  item->setText(0, QString("<> jump: \"%1\"").arg(label));
  item->setBackground(0, Qt::red);
  item->setForeground(0, Qt::white);
  for (auto event : pBattleEvent->allEvents)
  {
    if (event->getType() == Type::Label && ((Label*)event)->getName() == label)
    {
      item->setBackground(0, Qt::NoBrush);
      item->setForeground(0, Qt::NoBrush);
      break;
    }
  }
  return item;
}
//END: Jump

//BEGIN: IfCounter (NEEDS EDITOR)
BattleEvent::IfCounter::IfCounter(BattleEvent *pBase, Event *parent) : IfEvent(pBase, parent, Type::IfCounter)
{
  counter = 0;
  value = 0;
  condition = CONDITION_EQUAL_TO;
}

BattleEvent::IfCounter::IfCounter(BattleEvent *pBase, Event *parent, XMLNode eventNode) : IfEvent(pBase, parent, Type::IfCounter, eventNode)
{
  QString conditionString = eventNode.getAttribute("condition");
  counter = QString(eventNode.getAttribute("counter")).toInt();
  value = QString(eventNode.getAttribute("value")).toInt();
  if (conditionString == "=")
    condition = CONDITION_EQUAL_TO;
  else if (conditionString == "!=")
    condition = CONDITION_NOT_EQUAL_TO;
  else if (conditionString == ">")
    condition = CONDITION_GREATER_THAN;
  else if (conditionString == ">=")
    condition = CONDITION_GREATER_THAN_OR_EQUAL_TO;
  else if (conditionString == "<")
    condition = CONDITION_LESS_THAN;
  else if (conditionString == "<=")
    condition = CONDITION_LESS_THAN_OR_EQUAL_TO;
}

BattleEvent::IfCounter::~IfCounter()
{
}

XMLNode BattleEvent::IfCounter::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfCounter":"ifCounter");
  eventNode.addAttribute("counter", QString::number(counter).toLocal8Bit().data());
  eventNode.addAttribute("condition", conditions[condition]);
  eventNode.addAttribute("value", QString::number(value).toLocal8Bit().data());
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void BattleEvent::IfCounter::compileEvent(QByteArray *bytes)
{
  JumpLocation trueJump, falseJump;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  trueJump.pos = bytes->count();
  trueJump.event = events[0];
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  falseJump.pos = bytes->count();
  falseJump.event = parent->nextChildEvent(this);
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  bytes->append(counter);
  bytes->append(value);
  bytes->append(condition);
  pBattleEvent->jumps += trueJump;
  pBattleEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool BattleEvent::IfCounter::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::IfCounter::getItem()
{
  if (item->childCount() == 0)
    addItems(item);
  item->setText(0, QString("<> %1: counters[%2] %3 %4").arg((elseIf) ? "elseIfCounter":"ifCounter").arg(counter).arg(conditions[condition]).arg(value));
  return item;
}
//END: IfCounter

//BEGIN: IfStatus (NEEDS EDITOR)
BattleEvent::IfStatus::IfStatus(BattleEvent *pBase, Event *parent) : IfEvent(pBase, parent, Type::IfStatus)
{
  condition = CONDITION_EQUAL_TO;
  status = 0;
  level = 1;
  self = false;
}

BattleEvent::IfStatus::IfStatus(BattleEvent *pBase, Event *parent, XMLNode eventNode) : IfEvent(pBase, parent, Type::IfStatus, eventNode)
{
  QString statusString = eventNode.getAttribute("status");
  QString conditionString = eventNode.getAttribute("condition");
  if (statusString == "poison")
    status = 0;
  else if (statusString == "speed")
    status = 1;
  else if (statusString == "focus")
    status = 2;
  else if (statusString == "berserk")
    status = 3;
  if (conditionString == "=")
    condition = CONDITION_EQUAL_TO;
  else if (conditionString == "!=")
    condition = CONDITION_NOT_EQUAL_TO;
  else if (conditionString == ">")
    condition = CONDITION_GREATER_THAN;
  else if (conditionString == ">=")
    condition = CONDITION_GREATER_THAN_OR_EQUAL_TO;
  else if (conditionString == "<")
    condition = CONDITION_LESS_THAN;
  else if (conditionString == "<=")
    condition = CONDITION_LESS_THAN_OR_EQUAL_TO;
  level = QString(eventNode.getAttribute("level")).toInt();
  self = QString(eventNode.getAttribute("self")) == "true";
}

BattleEvent::IfStatus::~IfStatus()
{
}

XMLNode BattleEvent::IfStatus::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfStatus":"ifStatus");
  eventNode.addAttribute("self", (self) ? "true":"false");
  eventNode.addAttribute("status", statuses[status]);
  eventNode.addAttribute("condition", conditions[condition]);
  eventNode.addAttribute("level", QString::number(level).toLocal8Bit().data());
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void BattleEvent::IfStatus::compileEvent(QByteArray *bytes)
{
  JumpLocation trueJump, falseJump;
  uint8_t value = (status << 2) | level | ((self) ? 128:0);
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(value);
  bytes->append(condition);
  trueJump.pos = bytes->count();
  trueJump.event = events[0];
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  falseJump.pos = bytes->count();
  falseJump.event = parent->nextChildEvent(this);
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  pBattleEvent->jumps += trueJump;
  pBattleEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool BattleEvent::IfStatus::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::IfStatus::getItem()
{
  if (item->childCount() == 0)
    addItems(item);
  item->setText(0, QString("<> %1: %2 %3 %4 %5").arg((elseIf) ? "elseIfStatus":"ifStatus").arg((self) ? "self":"player").arg(statuses[status]).arg(conditions[condition]).arg(level));
  return item;
}
//END: IfStatus

//BEGIN: IfStat (NEEDS EDITOR)
BattleEvent::IfStat::IfStat(BattleEvent *pBase, Event *parent) : IfEvent(pBase, parent, Type::IfStat)
{
  condition = CONDITION_EQUAL_TO;
  value = 0;
  hp = true;
}

BattleEvent::IfStat::IfStat(BattleEvent *pBase, Event *parent, XMLNode eventNode) : IfEvent(pBase, parent, Type::IfStat, eventNode)
{
  QString conditionString = eventNode.getAttribute("condition");
  value = QString(eventNode.getAttribute("value")).toInt();
  hp = QString(eventNode.getAttribute("stat")) == "HP";
  if (conditionString == "=")
    condition = CONDITION_EQUAL_TO;
  else if (conditionString == "!=")
    condition = CONDITION_NOT_EQUAL_TO;
  else if (conditionString == ">")
    condition = CONDITION_GREATER_THAN;
  else if (conditionString == ">=")
    condition = CONDITION_GREATER_THAN_OR_EQUAL_TO;
  else if (conditionString == "<")
    condition = CONDITION_LESS_THAN;
  else if (conditionString == "<=")
    condition = CONDITION_LESS_THAN_OR_EQUAL_TO;
}

BattleEvent::IfStat::~IfStat()
{
}

XMLNode BattleEvent::IfStat::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfStat":"ifStat");
  eventNode.addAttribute("stat", (hp) ? "HP":"MP");
  eventNode.addAttribute("condition", conditions[condition]);
  eventNode.addAttribute("value", QString::number(value).toLocal8Bit().data());
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void BattleEvent::IfStat::compileEvent(QByteArray *bytes)
{
  JumpLocation trueJump, falseJump;
  Globals::Value16 value16;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  condition &= 0x7F;
  if (hp)
    condition |= 128;
  bytes->append(condition);
  condition &= 0x7F;
  value16.uValue = value;
  bytes->append(value16.bytes[0]);
  bytes->append(value16.bytes[1]);
  trueJump.pos = bytes->count();
  trueJump.event = events[0];
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  falseJump.pos = bytes->count();
  falseJump.event = parent->nextChildEvent(this);
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  pBattleEvent->jumps += trueJump;
  pBattleEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool BattleEvent::IfStat::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::IfStat::getItem()
{
  if (item->childCount() == 0)
    addItems(item);
  item->setText(0, QString("<> %1: %2 %3 %4").arg((elseIf) ? "elseIfStat":"ifStat").arg((hp) ? "HP":"MP").arg(conditions[condition]).arg(value));
  return item;
}
//END: IfStat

//BEGIN: ChangeBattlerSprite (NEEDS EDITOR)
BattleEvent::ChangeBattlerSprite::ChangeBattlerSprite(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::ChangeBattlerSprite)
{
  spriteID = 0;
}

BattleEvent::ChangeBattlerSprite::ChangeBattlerSprite(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::ChangeBattlerSprite)
{
  spriteID = QString(eventNode.getAttribute("spriteID")).toInt();
}

BattleEvent::ChangeBattlerSprite::~ChangeBattlerSprite()
{
}

XMLNode BattleEvent::ChangeBattlerSprite::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("changeBattlerSprite");
  eventNode.addAttribute("spriteID", QString::number(spriteID).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::ChangeBattlerSprite::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(spriteID);
}

bool BattleEvent::ChangeBattlerSprite::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::ChangeBattlerSprite::getItem()
{
  item->setText(0, QString("<> changeBattlerSprite: %1").arg(spriteID));
  return item;
}
//END: ChangeBattlerSprite

//BEGIN: ChangeBackgroundImage (NEEDS EDITOR)
BattleEvent::ChangeBackgroundImage::ChangeBackgroundImage(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::ChangeBackgroundImage)
{
  image = Globals::backdrops[0];
}

BattleEvent::ChangeBackgroundImage::ChangeBackgroundImage(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::ChangeBackgroundImage)
{
  image = eventNode.getAttribute("image");
}

BattleEvent::ChangeBackgroundImage::~ChangeBackgroundImage()
{
}

XMLNode BattleEvent::ChangeBackgroundImage::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("changeBackgroundImage");
  eventNode.addAttribute("image", image.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::ChangeBackgroundImage::compileEvent(QByteArray *bytes)
{
  uint8_t id = Globals::backdrops.indexOf(image);
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(id);
}

bool BattleEvent::ChangeBackgroundImage::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::ChangeBackgroundImage::getItem()
{
  item->setText(0, QString("<> changeBackgroundImage: %1").arg(image));
  return item;
}
//END: ChangeBackgroundImage

//BEGIN: PlayBattleAnimation (COMPLETE)
BattleEvent::PlayBattleAnimation::PlayBattleAnimation(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::PlayBattleAnimation)
{
  QDir folder(QString("%1/animations").arg(Globals::datadir));
  QStringList animations = folder.entryList(QStringList() << "*.anim", QDir::Files, QDir::Name);
  animations.replaceInStrings(".anim", "", Qt::CaseInsensitive);
  animation = animations[0];
}

BattleEvent::PlayBattleAnimation::PlayBattleAnimation(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::PlayBattleAnimation)
{
  animation = eventNode.getAttribute("animation");
}

BattleEvent::PlayBattleAnimation::~PlayBattleAnimation()
{
}

XMLNode BattleEvent::PlayBattleAnimation::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("playBattleAnimation");
  eventNode.addAttribute("animation", animation.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::PlayBattleAnimation::compileEvent(QByteArray *bytes)
{
  QString location = QString("animations/%1.anim").arg(animation);
  Globals::Value32 value32;
  offset = bytes->count();
  value32.uValue = Globals::hash(location.toLocal8Bit().data(), location.size());
  bytes->append(static_cast<char>(type));
  bytes->append(value32.bytes[0]);
  bytes->append(value32.bytes[1]);
  bytes->append(value32.bytes[2]);
  bytes->append(value32.bytes[3]);
}

bool BattleEvent::PlayBattleAnimation::configureEvent(QWidget *parentWidget)
{
  QDir folder(QString("%1/animations").arg(Globals::datadir));
  QStringList animations = folder.entryList(QStringList() << "*.anim", QDir::Files, QDir::Name);
  QString choice;
  bool ok;
  animations.replaceInStrings(".anim", "", Qt::CaseInsensitive);
  choice = QInputDialog::getItem(parentWidget, "Basic Attack", "Animation", animations, animations.indexOf(animation), false, &ok);
  if (ok)
    animation = choice;
  return ok;
}

QTreeWidgetItem *BattleEvent::PlayBattleAnimation::getItem()
{
  item->setText(0, QString("<> playBattleAnimation: %1").arg(animation));
  return item;
}
//END: PlayBattleAnimation

//BEGIN: WaitFrames (COMPLETE)
BattleEvent::WaitFrames::WaitFrames(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::WaitFrames)
{
  numFrames = 0;
}

BattleEvent::WaitFrames::WaitFrames(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::WaitFrames)
{
  numFrames = atoi(eventNode.getAttribute("frames"));
}

BattleEvent::WaitFrames::~WaitFrames()
{
}

XMLNode BattleEvent::WaitFrames::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("waitFrames");
  eventNode.addAttribute("frames", QString::number(numFrames).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::WaitFrames::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(numFrames);
}

bool BattleEvent::WaitFrames::configureEvent(QWidget *parentWidget)
{
  uint8_t newValue;
  bool ok;
  newValue = QInputDialog::getInt(parentWidget, "Number of Frames", "Frames:", numFrames, 0, 255, 1, &ok);
  if (ok)
    numFrames = newValue;
  return ok;
}

QTreeWidgetItem *BattleEvent::WaitFrames::getItem()
{
  item->setText(0, QString("<> waitFrames: %1").arg(numFrames));
  return item;
}
//END: WaitFrames

//BEGIN: InflictStatus (NEEDS EDITOR)
BattleEvent::InflictStatus::InflictStatus(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::InflictStatus)
{
  successMessage = "Success";
  failMessage = "Failed";
  status = 0;
  level = 1;
  chance = 255;
  self = true;
}

BattleEvent::InflictStatus::InflictStatus(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::InflictStatus)
{
  QString statusString = eventNode.getAttribute("status");
  successMessage = eventNode.getAttribute("success");
  failMessage = eventNode.getAttribute("failed");
  self = QString(eventNode.getAttribute("self")) == "true";
  if (statusString == "poison")
    status = 0;
  else if (statusString == "speed")
    status = 1;
  else if (statusString == "focus")
    status = 2;
  else if (statusString == "berserk")
    status = 3;
  level = QString(eventNode.getAttribute("level")).toInt();
  if (!self)
    chance = QString(eventNode.getAttribute("chance")).toInt();
  else
    chance = 255;
}

BattleEvent::InflictStatus::~InflictStatus()
{
}

XMLNode BattleEvent::InflictStatus::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("inflictStatus");
  eventNode.addAttribute("self", (self) ? "true":"false");
  eventNode.addAttribute("status", statuses[status]);
  eventNode.addAttribute("level", QString::number(level).toLocal8Bit().data());
  if (!self)
    eventNode.addAttribute("chance", QString::number(chance).toLocal8Bit().data());
  eventNode.addAttribute("success", successMessage.toLocal8Bit().data());
  eventNode.addAttribute("failed", failMessage.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::InflictStatus::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append((status << 2) | level | ((self) ? 128:0));
  bytes->append(successMessage.size());
  bytes->append(failMessage.size());
  if (!self)
    bytes->append(chance);
  for (int i = 0; i < successMessage.size(); ++i)
    bytes->append(Globals::charToCode(successMessage[i].toLatin1()));
  for (int i = 0; i < failMessage.size(); ++i)
    bytes->append(Globals::charToCode(failMessage[i].toLatin1()));
}

bool BattleEvent::InflictStatus::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::InflictStatus::getItem()
{
  QString text = "<> inflictStatus: %1 %2%3 success=\"%4\" fail=\"%5\"";
  if (self)
    text = text.arg("self");
  else
    text = text.arg(QString("player (%1)").arg(chance));
  text = text.arg(statuses[status]).arg(level).arg(successMessage).arg(failMessage);
  item->setText(0, text);
  return item;
}
//END: InflictStatus

//BEGIN: ConsumeMP (COMPLETE)
BattleEvent::ConsumeMP::ConsumeMP(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::ConsumeMP)
{
  amount = 1;
}

BattleEvent::ConsumeMP::ConsumeMP(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::ConsumeMP)
{
  amount = QString(eventNode.getAttribute("amount")).toInt();
}

BattleEvent::ConsumeMP::~ConsumeMP()
{
}

XMLNode BattleEvent::ConsumeMP::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("consumeMP");
  eventNode.addAttribute("amount", QString::number(amount).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::ConsumeMP::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(amount);
}

bool BattleEvent::ConsumeMP::configureEvent(QWidget *parentWidget)
{
  uint8_t value;
  bool ok;
  value = QInputDialog::getInt(parentWidget, "Consume MP", "Amount:", amount, 0, 255, 1, &ok);
  if (ok)
    amount = value;
  return ok;
}

QTreeWidgetItem *BattleEvent::ConsumeMP::getItem()
{
  item->setText(0, QString("<> consumeMP: %1").arg(amount));
  return item;
}
//END: ConsumeMP

//BEGIN: Random (NEEDS EDITOR)
BattleEvent::Random::Random(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::Random)
{
  counter = 0;
  max = 255;
}

BattleEvent::Random::Random(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::Random)
{
  counter = QString(eventNode.getAttribute("counter")).toInt();
  max = QString(eventNode.getAttribute("max")).toInt();
}

BattleEvent::Random::~Random()
{
}

XMLNode BattleEvent::Random::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("random");
  eventNode.addAttribute("counter", QString::number(counter).toLocal8Bit().data());
  eventNode.addAttribute("max", QString::number(max).toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::Random::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(counter);
  bytes->append(max);
}

bool BattleEvent::Random::configureEvent(QWidget *parentWidget)
{
  //TODO
  return false;
}

QTreeWidgetItem *BattleEvent::Random::getItem()
{
  item->setText(0, QString("<> random: counters[%1] = rand(0, %2)").arg(counter).arg(max));
  return item;
}
//END: Random

//BEGIN: Label (COMPLETE)
BattleEvent::Label::Label(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::Label)
{
  name = "";
}

BattleEvent::Label::Label(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::Label)
{
  name = eventNode.getAttribute("name");
}

BattleEvent::Label::~Label()
{
}

XMLNode BattleEvent::Label::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("label");
  eventNode.addAttribute("name", name.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::Label::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
}

bool BattleEvent::Label::configureEvent(QWidget *parentWidget)
{
  QString input;
  bool ok;
  input = QInputDialog::getText(parentWidget, "Label", "Name:", QLineEdit::Normal, name, &ok);
  if (ok && !input.isEmpty())
  {
    for (auto event : pBattleEvent->allEvents)
    {
      if (event->getType() == Type::Jump)
        event->getItem();
    }
    name = input;
  }
  getItem();
  return ok;
}

QTreeWidgetItem *BattleEvent::Label::getItem()
{
  item->setText(0, QString("<> label: \"%2\"").arg(name));
  return item;
}
//END: Label

//BEGIN: Comment (COMPLETE)
BattleEvent::Comment::Comment(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::Comment)
{
  comment = "";
}

BattleEvent::Comment::Comment(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::Comment)
{
  comment = QString(eventNode.getAttribute("value")).replace("\\n", "\n").replace("\\\\", "\\");
}

BattleEvent::Comment::~Comment()
{
}

XMLNode BattleEvent::Comment::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("comment");
  QString adjustedComment = comment;
  adjustedComment.replace("\\", "\\\\");
  adjustedComment.replace("\n", "\\n");
  eventNode.addAttribute("value", adjustedComment.toLocal8Bit().data());
  return eventNode;
}

void BattleEvent::Comment::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
}

bool BattleEvent::Comment::configureEvent(QWidget *parentWidget)
{
  QString input;
  bool ok;
  input = QInputDialog::getMultiLineText(parentWidget, "Comment", QString(), comment, &ok);
  if (ok)
  {
    comment = input;
    getItem();
  }
  return ok;
}

QTreeWidgetItem *BattleEvent::Comment::getItem()
{
  QStringList lines = comment.split("\n");
  QString text;
  int maxLength = lines[0].length();
  for (int i = 1; i < lines.count(); ++i)
  {
    if (lines[i].length() > maxLength)
      maxLength = lines[i].length();
  }
  text += "**";
  for (int i = 0; i < maxLength; ++i)
    text += "*";
  text += "**\n";
  for (int i = 0; i < lines.count(); ++i)
    text += QString("* %1 *\n").arg(lines[i], maxLength * -1);
  text += "**";
  for (int i = 0; i < maxLength; ++i)
    text += "*";
  text += "**";
  item->setText(0, text);
  item->setForeground(0, Qt::darkGreen);
  return item;
}
//END: Comment

//BEGIN: Else (COMPLETE)
BattleEvent::Else::Else(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::Else)
{
  events += Event::newEvent(pBase, this, Type::End);
}

BattleEvent::Else::Else(BattleEvent *pBase, Event *parent, XMLNode eventNode) : Event(pBase, parent, Type::Else)
{
  for (int i = 0; i < eventNode.nChildNode(); ++i)
    events += Event::newEvent(pBase, this, eventNode.getChildNode(i));
  events += Event::newEvent(pBase, this, Type::End);
}

BattleEvent::Else::~Else()
{
  for (auto event : events)
    delete event;
  events.clear();
}

XMLNode BattleEvent::Else::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("else");
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void BattleEvent::Else::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  for (auto event : events)
    event->compileEvent(bytes);
  delete events.takeLast();
}

BattleEvent::Event *BattleEvent::Else::previousChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == 0)
    return nullptr;
  return events[index - 1];
}

BattleEvent::Event *BattleEvent::Else::nextChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == events.count() - 1)
    return nullptr;
  return events[index + 1];
}

QTreeWidgetItem *BattleEvent::Else::getItem()
{
  if (item->childCount() == 0)
  {
    for (auto event : events)
      item->addChild(event->getItem());
  }
  item->setText(0, "<> else");
  return item;
}
//END: Else

//BEGIN: End (COMPLETE)
void BattleEvent::End::compileEvent(QByteArray *bytes)
{
  if (parent->getType() == Event::Type::TopLevelEvent)
    bytes->append(static_cast<char>(Event::Type::EndEventProcessing));
  else
  {
    JumpLocation jump;
    bytes->append(static_cast<char>(Event::Type::Jump));
    jump.event = parent->getParentEvent()->nextChildEvent(parent);
    while (jump.event->isElse())
      jump.event = parent->getParentEvent()->nextChildEvent(jump.event);
    jump.pos = bytes->count();
    pBattleEvent->jumps += jump;
    bytes->append(0xDE);
    bytes->append(0xAD);
    bytes->append(0xC0);
    bytes->append(0xDE);
  }
}
//END: End
