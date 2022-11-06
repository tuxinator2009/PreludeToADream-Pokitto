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

#include <QInputDialog>
#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include "globals.h"
#include "configureevent_battle.h"
#include "configureevent_buffercharacter.h"
#include "configureevent_buffervalue.h"
#include "configureevent_changeeventflags.h"
#include "configureevent_changeeventlocation.h"
#include "configureevent_changepassability.h"
#include "configureevent_changesprite.h"
#include "configureevent_changevariable.h"
#include "configureevent_giveplayeritem.h"
#include "configureevent_ifexited.h"
#include "configureevent_ifhasitem.h"
#include "configureevent_ifswitch.h"
#include "configureevent_ifvalue.h"
#include "configureevent_ifvariable.h"
#include "configureevent_ifyesno.h"
#include "configureevent_moveplayer.h"
#include "configureevent_playsoundeffect.h"
#include "configureevent_playmusic.h"
#include "configureevent_shakescreen.h"
#include "configureevent_showhidescreen.h"
#include "configureevent_showimage.h"
#include "configureevent_showmessage.h"
#include "configureevent_showshopmenu.h"
#include "configureevent_switchrange.h"
#include "configureevent_teleport.h"
#include "map.h"
#include "mapevent.h"

//BEGIN: MapEvent
MapEvent::MapEvent(Map *m)
{
  name = "";
  x = 0;
  y = 0;
  flags = 0;
  spriteID = 0;
  map = m;
  topLevelEvent = new TopLevelEvent(this, nullptr);
}

MapEvent::MapEvent(Map *m, XMLNode topNode)
{
  if (topNode.nAttribute() == 5)
  {
    name = topNode.getAttribute("name");
    x = atoi(topNode.getAttribute("x"));
    y = atoi(topNode.getAttribute("y"));
    flags = atoi(topNode.getAttribute("flags"));
    spriteID = atoi(topNode.getAttribute("sprite"));
  }
  map = m;
  topLevelEvent = new TopLevelEvent(this, nullptr, topNode);
}

MapEvent::~MapEvent()
{
  delete topLevelEvent;
}

XMLNode MapEvent::toXMLNode(bool onLoadEvent)
{
  XMLNode topNode = topLevelEvent->toXMLNode();
  if (!onLoadEvent)
  {
    topNode.addAttribute("name", name.toLocal8Bit().data());
    topNode.addAttribute("x", QString::number(x).toLocal8Bit().data());
    topNode.addAttribute("y", QString::number(y).toLocal8Bit().data());
    topNode.addAttribute("flags", QString::number(flags).toLocal8Bit().data());
    topNode.addAttribute("sprite", QString::number(spriteID).toLocal8Bit().data());
  }
  else
    topNode.updateName("onLoadEvent");
  return topNode;
}

void MapEvent::compileEvent(Globals::MapData *pMapData, int index, QByteArray *bytes)
{
  if (index >= 0)
  {
    pMapData->events[index].x = x;
    pMapData->events[index].y = y;
    pMapData->events[index].flags = flags;
    pMapData->events[index].spriteID = spriteID;
    pMapData->events[index].offset = bytes->count();
  }
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
//END: MapEvent

//BEGIN: MapEvent::Event
const char *MapEvent::Event::typeString[] =
{
  "hideScreen",
  "showScreen",
  "teleport",
  "showDialog",
  "hideDialog",
  "bufferMessage",
  "bufferValue",
  "bufferCharacter",
  "showMessage",
  "showOneLiner",
  "showShopMenu",
  "showGold",
  "hideGold",
  "waitButtons",
  "waitFrames",
  "battle",
  "shakeScreen",
  "cutscene",
  "jump",
  "ifSwitch",
  "ifVariable",
  "ifHasItem",
  "ifExited",
  "ifYesNo",
  "ifValue",
  "ifFacing",
  "playSoundEffect",
  "playMusic",
  "pauseMusic",
  "resumeMusic",
  "waitMusic",
  "waitSoundEffect",
  "turnSwitchOn",
  "turnSwitchOff",
  "toggleSwitch",
  "turnSwitchRangeOn",
  "turnSwitchRangeOff",
  "toggleSwitchRange",
  "changeVariable",
  "changeSprite",
  "changeEventLocation",
  "changeEventFlags",
  "changePassability",
  "givePlayerItem",
  "givePlayerGold",
  "heal",
  "showImage",
  "hideImage",
  "movePlayer",
  "endEventProcessing",
  "label",
  "comment",
  "else",
  ""
};

const char *MapEvent::Event::hideTransitions[] =
{
  "instant",
  "fadeOut",
  "wipeDown",
  "wipeUp",
  "wipeIn",
  "maskOut",
  "whiteOut"
};

const char *MapEvent::Event::showTransitions[] =
{
  "instant",
  "fadeIn",
  "unwipeDown",
  "unwipeUp",
  "unwipeOut",
  "whiteIn"
};

const char *MapEvent::Event::facings[] =
{
  "up",
  "right",
  "down",
  "left",
  "retain"
};

const char *MapEvent::Event::bufferValues[] =
{
  "name",
  "hp",
  "maxHP",
  "mp",
  "maxMP",
  "xp",
  "nextLevel",
  "xpLeft",
  "gold",
  "level",
  "attack",
  "defense",
  "agility",
  "magic",
  "variable",
  "supplied"
};

const char *MapEvent::Event::operations[] =
{
  "SET",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "MOD"
};

const char *MapEvent::Event::movements[] =
{
  "up",
  "right",
  "down",
  "left",
  "forward",
  "strafeRight",
  "backward",
  "strafeLeft",
  "faceUp",
  "faceRight",
  "faceDown",
  "faceLeft",
  "turnLeft",
  "turnRight",
  "turnAround"
};

const char *MapEvent::Event::conditions[] = {"=","!=",">",">=","<","<="};

MapEvent::Event::Event(MapEvent *pBase, Event *p, Type t)
{
  if (t != Type::TopLevelEvent)
  {
    item = new QTreeWidgetItem();
    item->setData(1, Qt::UserRole, QVariant::fromValue(static_cast<void*>(this)));
    item->setExpanded(true);
  }
  pMapEvent = pBase;
  parent = p;
  type = t;
  pMapEvent->allEvents += this;
}

MapEvent::Event::~Event()
{
  if (type != Type::TopLevelEvent)
    delete item;
  pMapEvent->allEvents.removeAll(this);
}

MapEvent::Event *MapEvent::Event::newEvent(MapEvent *pBase, Event *pEvent, Type t)
{
  if (t == Type::HideScreen)
    return new HideScreen(pBase, pEvent);
  else if (t == Type::ShowScreen)
    return new ShowScreen(pBase, pEvent);
  else if (t == Type::Teleport)
    return new Teleport(pBase, pEvent);
  else if (t == Type::ShowDialog)
    return new ShowDialog(pBase, pEvent);
  else if (t == Type::HideDialog)
    return new Event(pBase, pEvent, t);
  else if (t == Type::BufferMessage)
    return new BufferMessage(pBase, pEvent);
  else if (t == Type::BufferValue)
    return new BufferValue(pBase, pEvent);
  else if (t == Type::BufferCharacter)
    return new BufferCharacter(pBase, pEvent);
  else if (t == Type::ShowMessage)
    return new ShowMessage(pBase, pEvent);
  else if (t == Type::ShowOneLiner)
    return new ShowOneLiner(pBase, pEvent);
  else if (t == Type::ShowShopMenu)
    return new ShowShopMenu(pBase, pEvent);
  else if (t == Type::ShowGold)
    return new Event(pBase, pEvent, t);
  else if (t == Type::HideGold)
    return new Event(pBase, pEvent, t);
  else if (t == Type::WaitButtons)
    return new Event(pBase, pEvent, t);
  else if (t == Type::WaitFrames)
    return new WaitFrames(pBase, pEvent);
  else if (t == Type::Battle)
    return new Battle(pBase, pEvent);
  else if (t == Type::ShakeScreen)
    return new ShakeScreen(pBase, pEvent);
  else if (t == Type::Cutscene)
    return new Cutscene(pBase, pEvent);
  else if (t == Type::Jump)
    return new Jump(pBase, pEvent);
  else if (t == Type::IfSwitch)
    return new IfSwitch(pBase, pEvent);
  else if (t == Type::IfVariable)
    return new IfVariable(pBase, pEvent);
  else if (t == Type::IfHasItem)
    return new IfHasItem(pBase, pEvent);
  else if (t == Type::IfExited)
    return new IfExited(pBase, pEvent);
  else if (t == Type::IfYesNo)
    return new IfYesNo(pBase, pEvent);
  else if (t == Type::IfValue)
    return new IfValue(pBase, pEvent);
  else if (t == Type::IfFacing)
    return new IfFacing(pBase, pEvent);
  else if (t == Type::PlaySoundEffect)
    return new PlaySoundEffect(pBase, pEvent);
  else if (t == Type::PlayMusic)
    return new PlayMusic(pBase, pEvent);
  else if (t == Type::PauseMusic)
    return new Event(pBase, pEvent, t);
  else if (t == Type::ResumeMusic)
    return new Event(pBase, pEvent, t);
  else if (t == Type::WaitMusic)
    return new Event(pBase, pEvent, t);
  else if (t == Type::WaitSoundEffect)
    return new Event(pBase, pEvent, t);
  else if (t == Type::TurnSwitchOn)
    return new TurnSwitchOn(pBase, pEvent);
  else if (t == Type::TurnSwitchOff)
    return new TurnSwitchOff(pBase, pEvent);
  else if (t == Type::ToggleSwitch)
    return new ToggleSwitch(pBase, pEvent);
  else if (t == Type::TurnSwitchRangeOn)
    return new TurnSwitchRangeOn(pBase, pEvent);
  else if (t == Type::TurnSwitchRangeOff)
    return new TurnSwitchRangeOff(pBase, pEvent);
  else if (t == Type::ToggleSwitchRange)
    return new ToggleSwitchRange(pBase, pEvent);
  else if (t == Type::ChangeVariable)
    return new ChangeVariable(pBase, pEvent);
  else if (t == Type::ChangeSprite)
    return new ChangeSprite(pBase, pEvent);
  else if (t == Type::ChangeEventLocation)
    return new ChangeEventLocation(pBase, pEvent);
  else if (t == Type::ChangeEventFlags)
    return new ChangeEventFlags(pBase, pEvent);
  else if (t == Type::ChangePassability)
    return new ChangePassability(pBase, pEvent);
  else if (t == Type::GivePlayerItem)
    return new GivePlayerItem(pBase, pEvent);
  else if (t == Type::GivePlayerGold)
    return new GivePlayerGold(pBase, pEvent);
  else if (t == Type::Heal)
    return new Event(pBase, pEvent, t);
  else if (t == Type::ShowImage)
    return new ShowImage(pBase, pEvent);
  else if (t == Type::HideImage)
    return new Event(pBase, pEvent, t);
  else if (t == Type::MovePlayer)
    return new MovePlayer(pBase, pEvent);
  else if (t == Type::EndEventProcessing)
    return new Event(pBase, pEvent, t);
  else if (t == Type::Label)
    return new Label(pBase, pEvent);
  else if (t == Type::Comment)
    return new Comment(pBase, pEvent);
  else if (t == Type::Else)
    return new Else(pBase, pEvent);
  else if (t == Type::End)
    return new End(pBase, pEvent);
  return nullptr;
}

MapEvent::Event *MapEvent::Event::newEvent(MapEvent *pBase, Event *pEvent, XMLNode eventNode)
{
  std::string eventType = eventNode.getName();
  if (eventType.compare("hideScreen") == 0)
    return new HideScreen(pBase, pEvent, eventNode);
  else if (eventType.compare("showScreen") == 0)
    return new ShowScreen(pBase, pEvent, eventNode);
  else if (eventType.compare("teleport") == 0)
    return new Teleport(pBase, pEvent, eventNode);
  else if (eventType.compare("showDialog") == 0)
    return new ShowDialog(pBase, pEvent, eventNode);
  else if (eventType.compare("hideDialog") == 0)
    return new Event(pBase, pEvent, Type::HideDialog);
  else if (eventType.compare("bufferMessage") == 0)
    return new BufferMessage(pBase, pEvent, eventNode);
  else if (eventType.compare("bufferValue") == 0)
    return new BufferValue(pBase, pEvent, eventNode);
  else if (eventType.compare("bufferCharacter") == 0)
    return new BufferCharacter(pBase, pEvent, eventNode);
  else if (eventType.compare("showMessage") == 0)
    return new ShowMessage(pBase, pEvent, eventNode);
  else if (eventType.compare("showOneLiner") == 0)
    return new ShowOneLiner(pBase, pEvent, eventNode);
  else if (eventType.compare("showShopMenu") == 0)
    return new ShowShopMenu(pBase, pEvent, eventNode);
  else if (eventType.compare("showGold") == 0)
    return new Event(pBase, pEvent, Type::ShowGold);
  else if (eventType.compare("hideGold") == 0)
    return new Event(pBase, pEvent, Type::HideGold);
  else if (eventType.compare("waitButtons") == 0)
    return new Event(pBase, pEvent, Type::WaitButtons);
  else if (eventType.compare("waitFrames") == 0)
    return new WaitFrames(pBase, pEvent, eventNode);
  else if (eventType.compare("battle") == 0)
    return new Battle(pBase, pEvent, eventNode);
  else if (eventType.compare("shakeScreen") == 0)
    return new ShakeScreen(pBase, pEvent, eventNode);
  else if (eventType.compare("cutscene") == 0)
    return new Cutscene(pBase, pEvent, eventNode);
  else if (eventType.compare("jump") == 0)
    return new Jump(pBase, pEvent, eventNode);
  else if (eventType.compare("ifSwitch") == 0 || eventType.compare("elseIfSwitch") == 0)
    return new IfSwitch(pBase, pEvent, eventNode);
  else if (eventType.compare("ifVariable") == 0 || eventType.compare("elseIfVariable") == 0)
    return new IfVariable(pBase, pEvent, eventNode);
  else if (eventType.compare("ifHasItem") == 0 || eventType.compare("elseHasItem") == 0)
    return new IfHasItem(pBase, pEvent, eventNode);
  else if (eventType.compare("ifExited") == 0 || eventType.compare("elseIfExited") == 0)
    return new IfExited(pBase, pEvent, eventNode);
  else if (eventType.compare("ifYes") == 0 || eventType.compare("elseIfYes") == 0)
    return new IfYesNo(pBase, pEvent, eventNode);
  else if (eventType.compare("ifNo") == 0 || eventType.compare("elseIfNo") == 0)
    return new IfYesNo(pBase, pEvent, eventNode);
  else if (eventType.compare("ifValue") == 0 || eventType.compare("elseIfValue") == 0)
    return new IfValue(pBase, pEvent, eventNode);
  else if (eventType.compare("ifFacing") == 0 || eventType.compare("elseIfFacing") == 0)
    return new IfFacing(pBase, pEvent, eventNode);
  else if (eventType.compare("playSoundEffect") == 0)
    return new PlaySoundEffect(pBase, pEvent, eventNode);
  else if (eventType.compare("playMusic") == 0)
    return new PlayMusic(pBase, pEvent, eventNode);
  else if (eventType.compare("pauseMusic") == 0)
    return new Event(pBase, pEvent, Type::PauseMusic);
  else if (eventType.compare("resumeMusic") == 0)
    return new Event(pBase, pEvent, Type::ResumeMusic);
  else if (eventType.compare("waitMusic") == 0)
    return new Event(pBase, pEvent, Type::WaitMusic);
  else if (eventType.compare("waitSoundEffect") == 0)
    return new Event(pBase, pEvent, Type::WaitSoundEffect);
  else if (eventType.compare("turnSwitchOn") == 0)
    return new TurnSwitchOn(pBase, pEvent, eventNode);
  else if (eventType.compare("turnSwitchOff") == 0)
    return new TurnSwitchOff(pBase, pEvent, eventNode);
  else if (eventType.compare("toggleSwitch") == 0)
    return new ToggleSwitch(pBase, pEvent, eventNode);
  else if (eventType.compare("turnSwitchRangeOn") == 0)
    return new TurnSwitchRangeOn(pBase, pEvent, eventNode);
  else if (eventType.compare("turnSwitchRangeOff") == 0)
    return new TurnSwitchRangeOff(pBase, pEvent, eventNode);
  else if (eventType.compare("toggleSwitchRange") == 0)
    return new ToggleSwitchRange(pBase, pEvent, eventNode);
  else if (eventType.compare("changeVariable") == 0)
    return new ChangeVariable(pBase, pEvent, eventNode);
  else if (eventType.compare("changeSprite") == 0)
    return new ChangeSprite(pBase, pEvent, eventNode);
  else if (eventType.compare("changeEventLocation") == 0)
    return new ChangeEventLocation(pBase, pEvent, eventNode);
  else if (eventType.compare("changeEventFlags") == 0)
    return new ChangeEventFlags(pBase, pEvent, eventNode);
  else if (eventType.compare("changePassability") == 0)
    return new ChangePassability(pBase, pEvent, eventNode);
  else if (eventType.compare("givePlayerItem") == 0)
    return new GivePlayerItem(pBase, pEvent, eventNode);
  else if (eventType.compare("givePlayerGold") == 0)
    return new GivePlayerGold(pBase, pEvent, eventNode);
  else if (eventType.compare("heal") == 0)
    return new Event(pBase, pEvent, Type::Heal);
  else if (eventType.compare("showImage") == 0)
    return new ShowImage(pBase, pEvent, eventNode);
  else if (eventType.compare("hideImage") == 0)
    return new Event(pBase, pEvent, Type::HideImage);
  else if (eventType.compare("movePlayer") == 0)
    return new MovePlayer(pBase, pEvent, eventNode);
  else if (eventType.compare("endEventProcessing") == 0)
    return new Event(pBase, pEvent, Type::EndEventProcessing);
  else if (eventType.compare("label") == 0)
    return new Label(pBase, pEvent, eventNode);
  else if (eventType.compare("comment") == 0)
    return new Comment(pBase, pEvent, eventNode);
  else if (eventType.compare("else") == 0)
    return new Else(pBase, pEvent, eventNode);
  return nullptr;
}
//END: MapEvent::Event

//BEGIN: TopLevelEvent (COMPLETE)
MapEvent::TopLevelEvent::TopLevelEvent(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::TopLevelEvent)
{
  for (int i = 0; i < eventNode.nChildNode(); ++i)
    events += Event::newEvent(pBase, this, eventNode.getChildNode(i));
  events += Event::newEvent(pBase, this, Type::End);
}

MapEvent::TopLevelEvent::~TopLevelEvent()
{
  for (auto event : events)
    delete event;
  events.clear();
}

XMLNode MapEvent::TopLevelEvent::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("event");
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::TopLevelEvent::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  for (auto event : events)
    event->compileEvent(bytes);
}

MapEvent::Event *MapEvent::TopLevelEvent::previousChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == 0)
    return nullptr;
  return events[index - 1];
}

MapEvent::Event *MapEvent::TopLevelEvent::nextChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == events.count() - 1)
    return nullptr;
  return events[index + 1];
}

void MapEvent::TopLevelEvent::setItem(QTreeWidgetItem *value)
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
MapEvent::IfEvent::IfEvent(MapEvent *pBase, Event *pEvent, Type t, XMLNode eventNode) : Event(pBase, pEvent, t)
{
  elseIf = strncmp(eventNode.getName(), "else", 4) == 0;
  for (int i = 0; i < eventNode.nChildNode(); ++i)
    events += Event::newEvent(pBase, this, eventNode.getChildNode(i));
  events += Event::newEvent(pBase, this, Type::End);
}

MapEvent::IfEvent::~IfEvent()
{
  for (auto event : events)
    delete event;
  events.clear();
}

MapEvent::Event *MapEvent::IfEvent::previousChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == 0)
    return nullptr;
  return events[index - 1];
}

MapEvent::Event *MapEvent::IfEvent::nextChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == events.count() - 1)
    return nullptr;
  return events[index + 1];
}

void MapEvent::IfEvent::addItems()
{
  for (auto event : events)
    item->addChild(event->getItem());
}
//END: IfEvent

//BEGIN: HideScreen (COMPLETE)
MapEvent::HideScreen::HideScreen(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::HideScreen)
{
  transition = HIDE_INSTANT;
  speed = 0;
}

MapEvent::HideScreen::HideScreen(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::HideScreen)
{
  transition = atoi(eventNode.getAttribute("transition"));
  speed = atoi(eventNode.getAttribute("speed"));
}

MapEvent::HideScreen::~HideScreen()
{
}

XMLNode MapEvent::HideScreen::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("hideScreen");
  eventNode.addAttribute("transition", QString::number(transition).toLocal8Bit().data());
  eventNode.addAttribute("speed", QString::number(speed).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::HideScreen::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(transition);
  bytes->append(speed);
}

bool MapEvent::HideScreen::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ShowHideScreen *configure = new ConfigureEvent_ShowHideScreen(parentWidget);
  configure->setSpeed(speed);
  configure->setTransitions(QStringList() << "instant" << "fadeOut" << "wipeDown" << "wipeUp" << "wipeIn" << "maskOut" << "whiteOut");
  configure->setTransition(transition);
  if (configure->exec())
  {
    speed = configure->getSpeed();
    transition = configure->getTransition();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::HideScreen::getItem()
{
  item->setText(0, QString("<> hideScreen: transition=%1 speed=%2").arg(hideTransitions[transition]).arg(speed));
  return item;
}
//END: HideScreen

//BEGIN: ShowScreen (COMPLETE)
MapEvent::ShowScreen::ShowScreen(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ShowScreen)
{
  transition = SHOW_INSTANT;
  speed = 0;
}

MapEvent::ShowScreen::ShowScreen(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ShowScreen)
{
  transition = atoi(eventNode.getAttribute("transition"));
  speed = atoi(eventNode.getAttribute("speed"));
}

MapEvent::ShowScreen::~ShowScreen()
{
}

XMLNode MapEvent::ShowScreen::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("showScreen");
  eventNode.addAttribute("transition", QString::number(transition).toLocal8Bit().data());
  eventNode.addAttribute("speed", QString::number(speed).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ShowScreen::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(transition);
  bytes->append(speed);
}

bool MapEvent::ShowScreen::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ShowHideScreen *configure = new ConfigureEvent_ShowHideScreen(parentWidget);
  configure->setSpeed(speed);
  configure->setTransitions(QStringList() << "instant" << "fadeIn" << "unwipeDown" << "unwipeUp" << "unwipeOut" << "whiteIn");
  configure->setTransition(transition);
  if (configure->exec())
  {
    speed = configure->getSpeed();
    transition = configure->getTransition();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ShowScreen::getItem()
{
  item->setText(0, QString("<> showScreen: transition=%1 speed=%2").arg(showTransitions[transition]).arg(speed));
  return item;
}
//END: ShowScreen

//BEGIN: Teleport (COMPLETE)
MapEvent::Teleport::Teleport(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::Teleport)
{
  mapID = 0;
  x = 0;
  y = 0;
  facing = DIR_RETAIN;
}

MapEvent::Teleport::Teleport(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::Teleport)
{
  mapID = atoi(eventNode.getAttribute("mapID"));
  x = atoi(eventNode.getAttribute("x"));
  y = atoi(eventNode.getAttribute("y"));
  facing = atoi(eventNode.getAttribute("facing"));
}

MapEvent::Teleport::~Teleport()
{
}

XMLNode MapEvent::Teleport::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("teleport");
  eventNode.addAttribute("mapID", QString::number(mapID).toLocal8Bit().data());
  eventNode.addAttribute("x", QString::number(x).toLocal8Bit().data());
  eventNode.addAttribute("y", QString::number(y).toLocal8Bit().data());
  eventNode.addAttribute("facing", QString::number(facing).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::Teleport::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  QString mapLocation = QString("/maps/map%1.xml").arg(mapID, 4, 10, QChar('0'));
  Globals::Value32 value32 = {.uValue = Globals::hash(mapLocation.toLocal8Bit().data(), mapLocation.length() + 1)};
  bytes->append(static_cast<char>(type));
  bytes->append(value32.bytes[0]);
  bytes->append(value32.bytes[1]);
  bytes->append(value32.bytes[2]);
  bytes->append(value32.bytes[3]);
  bytes->append(x);
  bytes->append(y);
  bytes->append(facing);
}

bool MapEvent::Teleport::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_Teleport *configure = new ConfigureEvent_Teleport(parentWidget);
  configure->setMapID(mapID);
  configure->setLocation(x, y);
  configure->setFacing(facing);
  if (configure->exec())
  {
    mapID = configure->getMapID();
    x = configure->getX();
    y = configure->getY();
    facing = configure->getFacing();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::Teleport::getItem()
{
  item->setText(0, QString("<> teleport: \"%1\" (%2, %3) %4").arg(Globals::mapNames[mapID]).arg(x).arg(y).arg(facings[facing]));
  return item;
}
//END: Teleport

//BEGIN: ShowDialog (COMPLETE)
MapEvent::ShowDialog::ShowDialog(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ShowDialog)
{
  name = "";
}

MapEvent::ShowDialog::ShowDialog(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ShowDialog)
{
  name = eventNode.getAttribute("name");
}

MapEvent::ShowDialog::~ShowDialog()
{
}

XMLNode MapEvent::ShowDialog::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("showDialog");
  eventNode.addAttribute("name", name.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ShowDialog::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  for (int i = 0; i < 8; ++i)
  {
    if (name == "__PLAYER__")
      bytes->append(0xFF);
    else if (i < name.length())
      bytes->append(Globals::charToCode(name[i].toLatin1()));
    else
      bytes->append(Globals::charToCode('='));
  }
}

bool MapEvent::ShowDialog::configureEvent(QWidget *parentWidget)
{
  QString value;
  bool ok;
  value = QInputDialog::getText(parentWidget, "Show Dialog", "Name:", QLineEdit::Normal, name, &ok);
  if (ok)
  {
    if (value != "__PLAYER__" && value.length() > 8)
    {
      QMessageBox::critical(parentWidget, "Name Too Long", "The name you entered is too long.\nMax length is 8 characters or \"__PLAYER__\" for the player's name.");
      return false;
    }
    name = value;
    return true;
  }
  return false;
}

QTreeWidgetItem *MapEvent::ShowDialog::getItem()
{
  item->setText(0, QString("<> showDialog: \"%1\"").arg(name));
  return item;
}
//END: ShowDialog

//BEGIN: BufferMessage (COMPLETE)
MapEvent::BufferMessage::BufferMessage(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::BufferMessage)
{
  message = "";
}

MapEvent::BufferMessage::BufferMessage(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::BufferMessage)
{
  message = eventNode.getAttribute("message");
}

MapEvent::BufferMessage::~BufferMessage()
{
}

XMLNode MapEvent::BufferMessage::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("bufferMessage");
  eventNode.addAttribute("message", message.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::BufferMessage::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append((uint8_t)message.length());
  for (int i = 0; i < message.length(); ++i)
    bytes->append(Globals::charToCode(message[i].toLatin1()));
}

bool MapEvent::BufferMessage::configureEvent(QWidget *parentWidget)
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

QTreeWidgetItem *MapEvent::BufferMessage::getItem()
{
  item->setText(0, QString("<> bufferMessage: \"%1\"").arg(message));
  return item;
}
//END: BufferMessage

//BEGIN: BufferValue (COMPLETE)
MapEvent::BufferValue::BufferValue(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::BufferValue)
{
  valueType = VALUE_PLAYER_NAME;
  value = 0;
}

MapEvent::BufferValue::BufferValue(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::BufferValue)
{
  valueType = atoi(eventNode.getAttribute("value"));
  if (valueType == VALUE_VARIABLE)
    value = atoi(eventNode.getAttribute("variable"));
  else if (valueType == VALUE_SUPPLIED)
    value = atoi(eventNode.getAttribute("supplied"));
}

MapEvent::BufferValue::~BufferValue()
{
}

XMLNode MapEvent::BufferValue::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("bufferValue");
  eventNode.addAttribute("value", QString::number(valueType).toLocal8Bit().data());
  if (valueType == VALUE_VARIABLE)
    eventNode.addAttribute("variable", QString::number(value).toLocal8Bit().data());
  else if (valueType == VALUE_SUPPLIED)
    eventNode.addAttribute("supplied", QString::number(value).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::BufferValue::compileEvent(QByteArray *bytes)
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

bool MapEvent::BufferValue::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_BufferValue *configure = new ConfigureEvent_BufferValue(parentWidget);
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

QTreeWidgetItem *MapEvent::BufferValue::getItem()
{
  QString text = QString("<> bufferValue: %1").arg(bufferValues[valueType]);
  if (valueType == VALUE_VARIABLE || valueType == VALUE_SUPPLIED)
    text += QString(" (%1)").arg(value);
  item->setText(0, text);
  return item;
}
//END: BufferValue

//BEGIN: BufferCharacter (COMPLETE)
MapEvent::BufferCharacter::BufferCharacter(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::BufferCharacter)
{
  character = ' ';
}

MapEvent::BufferCharacter::BufferCharacter(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::BufferCharacter)
{
  character = eventNode.getAttribute("char")[0];
}

MapEvent::BufferCharacter::~BufferCharacter()
{
}

XMLNode MapEvent::BufferCharacter::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("bufferCharacter");
  char text[2] = {character, '\0'};
  eventNode.addAttribute("char", text);
  return eventNode;
}

void MapEvent::BufferCharacter::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(Globals::charToCode(character));
}

bool MapEvent::BufferCharacter::configureEvent(QWidget *parentWidget)
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

QTreeWidgetItem *MapEvent::BufferCharacter::getItem()
{
  item->setText(0, QString("<> bufferCharacter: \'%1\'").arg(character));
  return item;
}
//END: BufferCharacter

//BEGIN: ShowMessage (COMPLETE)
MapEvent::ShowMessage::ShowMessage(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ShowMessage)
{
}

MapEvent::ShowMessage::ShowMessage(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ShowMessage)
{
  for (int i = 0; i < eventNode.nChildNode("line"); ++i)
    message += eventNode.getChildNode("line", i).getAttribute("message");
}

MapEvent::ShowMessage::~ShowMessage()
{
}

XMLNode MapEvent::ShowMessage::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("showMessage");
  for (int i = 0; i < message.count(); ++i)
    eventNode.addChild("line").addAttribute("message", message[i].toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ShowMessage::compileEvent(QByteArray *bytes)
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
        }
        else
          line += message[i][j];
      }
      bufferMessage(bytes, line);
      bytes->append(static_cast<char>(Type::ShowMessage));
      if (i % 4 == 3)
        bytes->append(static_cast<char>(Type::WaitButtons));
    }
    if (message.count() % 4 != 0)
      bytes->append(static_cast<char>(Type::WaitButtons));
  }
  else
    bytes->append(static_cast<char>(type));
}

bool MapEvent::ShowMessage::configureEvent(QWidget *parentWidget)
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

QTreeWidgetItem *MapEvent::ShowMessage::getItem()
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

void MapEvent::ShowMessage::bufferMessage(QByteArray *bytes, QString line)
{
  if (line.length() == 0)
    return;
  bytes->append(static_cast<char>(Type::BufferMessage));
  bytes->append((uint8_t)line.length());
  for (int i = 0; i < line.length(); ++i)
    bytes->append(Globals::charToCode(line[i].toLatin1()));
}
//END: ShowMessage

//BEGIN: ShowOneLiner (COMPLETE)
MapEvent::ShowOneLiner::ShowOneLiner(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ShowOneLiner)
{
  message = "";
}

MapEvent::ShowOneLiner::ShowOneLiner(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ShowOneLiner)
{
  message = eventNode.getAttribute("message");
}

MapEvent::ShowOneLiner::~ShowOneLiner()
{
}

XMLNode MapEvent::ShowOneLiner::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("showOneLiner");
  eventNode.addAttribute("message", message.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ShowOneLiner::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append((uint8_t)message.length());
  for (int i = 0; i < message.length(); ++i)
    bytes->append(Globals::charToCode(message[i].toLatin1()));
}

bool MapEvent::ShowOneLiner::configureEvent(QWidget *parentWidget)
{
  QString value;
  bool ok;
  value = QInputDialog::getText(parentWidget, "Show One Liner", "Message:", QLineEdit::Normal, message, &ok);
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

QTreeWidgetItem *MapEvent::ShowOneLiner::getItem()
{
  item->setText(0, QString("<> showOneLiner: \"%1\"").arg(message));
  return item;
}
//END: ShowOneLiner

//BEGIN: ShowShopMenu (COMPLETE)
MapEvent::ShowShopMenu::ShowShopMenu(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ShowShopMenu)
{
  greeting = "";
  consumables = 0;
  weapons = 0;
  helmets = 0;
  armor = 0;
  boots = 0;
  rings = 0;
  amulets = 0;
}

MapEvent::ShowShopMenu::ShowShopMenu(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ShowShopMenu)
{
  greeting = eventNode.getAttribute("greetings");
  consumables = atoi(eventNode.getAttribute("consumables"));
  weapons = atoi(eventNode.getAttribute("weapons"));
  helmets = atoi(eventNode.getAttribute("helmets"));
  armor = atoi(eventNode.getAttribute("armor"));
  boots = atoi(eventNode.getAttribute("boots"));
  rings = atoi(eventNode.getAttribute("rings"));
  amulets = atoi(eventNode.getAttribute("amulets"));
}

MapEvent::ShowShopMenu::~ShowShopMenu()
{
}

XMLNode MapEvent::ShowShopMenu::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("showShopMenu");
  eventNode.addAttribute("greetings", greeting.toLocal8Bit().data());
  eventNode.addAttribute("consumables", QString::number(consumables).toLocal8Bit().data());
  eventNode.addAttribute("weapons", QString::number(weapons).toLocal8Bit().data());
  eventNode.addAttribute("helmets", QString::number(helmets).toLocal8Bit().data());
  eventNode.addAttribute("armor", QString::number(armor).toLocal8Bit().data());
  eventNode.addAttribute("boots", QString::number(boots).toLocal8Bit().data());
  eventNode.addAttribute("rings", QString::number(rings).toLocal8Bit().data());
  eventNode.addAttribute("amulets", QString::number(amulets).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ShowShopMenu::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  Globals::Value16 v;
  bytes->append(static_cast<char>(type));
  for (int i = 0; i < 24; ++i)
  {
    if (i < greeting.length())
      bytes->append(Globals::charToCode(greeting[i].toLatin1()));
    else
      bytes->append(Globals::charToCode(' '));
  }
  v.uValue = consumables;
  bytes->append(v.bytes[0]);
  bytes->append(v.bytes[1]);
  v.uValue = weapons;
  bytes->append(v.bytes[0]);
  bytes->append(v.bytes[1]);
  v.uValue = helmets;
  bytes->append(v.bytes[0]);
  bytes->append(v.bytes[1]);
  v.uValue = armor;
  bytes->append(v.bytes[0]);
  bytes->append(v.bytes[1]);
  v.uValue = boots;
  bytes->append(v.bytes[0]);
  bytes->append(v.bytes[1]);
  v.uValue = rings;
  bytes->append(v.bytes[0]);
  bytes->append(v.bytes[1]);
  v.uValue = amulets;
  bytes->append(v.bytes[0]);
  bytes->append(v.bytes[1]);
}

bool MapEvent::ShowShopMenu::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ShowShopMenu *configure = new ConfigureEvent_ShowShopMenu(parentWidget);
  configure->setGreeting(greeting);
  configure->setConsumables(consumables);
  configure->setWeapons(weapons);
  configure->setHelmets(helmets);
  configure->setArmor(armor);
  configure->setBoots(boots);
  configure->setRings(rings);
  configure->setAmulets(amulets);
  if (configure->exec())
  {
    greeting = configure->getGreeting();
    consumables = configure->getConsumables();
    weapons = configure->getWeapons();
    helmets = configure->getHelmets();
    armor = configure->getArmor();
    boots = configure->getBoots();
    rings = configure->getRings();
    amulets = configure->getAmulets();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ShowShopMenu::getItem()
{
  QString text = QString("<> showShopMenu: \"%1\"\n").arg(greeting);
  QStringList consumablesText;
  QStringList weaponsText;
  QStringList helmetsText;
  QStringList armorText;
  QStringList bootsText;
  QStringList ringsText;
  QStringList amuletsText;
  for (int i = 0; i < 16; ++i)
  {
    if ((consumables & (1 << i)) != 0)
      consumablesText += Globals::items[0][i].name;
    if ((weapons & (1 << i)) != 0)
      weaponsText += Globals::items[2][i].name;
    if ((helmets & (1 << i)) != 0)
      helmetsText += Globals::items[3][i].name;
    if ((armor & (1 << i)) != 0)
      armorText += Globals::items[4][i].name;
    if ((boots & (1 << i)) != 0)
      bootsText += Globals::items[5][i].name;
    if ((rings & (1 << i)) != 0)
      ringsText += Globals::items[6][i].name;
    if ((amulets & (1 << i)) != 0)
      amuletsText += Globals::items[7][i].name;
  }
  if (consumablesText.count() == 0)
    text += "  Consumables: NO CONSUMABLES FOR SALE\n";
  else
    text += QString("  Consumables: %1\n").arg(consumablesText.join(", "));
  if (weaponsText.count() == 0)
    text += "  Weapons:     NO WEAPONS FOR SALE\n";
  else
    text += QString("  Weapons:     %1\n").arg(weaponsText.join(", "));
  if (helmetsText.count() == 0)
    text += "  Helmets:     NO HELMETS FOR SALE\n";
  else
    text += QString("  Helmets:     %1\n").arg(helmetsText.join(", "));
  if (armorText.count() == 0)
    text += "  Armor:       NO ARMOR FOR SALE\n";
  else
    text += QString("  Armor:       %1\n").arg(armorText.join(", "));
  if (bootsText.count() == 0)
    text += "  Boots:       NO BOOTS FOR SALE\n";
  else
    text += QString("  Boots:       %1\n").arg(bootsText.join(", "));
  if (ringsText.count() == 0)
    text += "  Rings:       NO RINGS FOR SALE\n";
  else
    text += QString("  Rings:       %1\n").arg(ringsText.join(", "));
  if (amuletsText.count() == 0)
    text += "  Amulets:     NO AMULETS FOR SALE";
  else
    text += QString("  Amulets:     %1").arg(amuletsText.join(", "));
  item->setText(0, text);
  return item;
}

QString MapEvent::ShowShopMenu::toString(uint16_t items, const char **names)
{
  QStringList text;
  for (int i = 0; i < 16; ++i)
  {
    if ((items & (1 << i)) != 0)
      text += names[i];
  }
  if (text.count())
    return QString();
  return QString("  %1\n").arg(text.join(", "));
}
//END: ShowShopMenu

//BEGIN: WaitFrames (COMPLETE)
MapEvent::WaitFrames::WaitFrames(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::WaitFrames)
{
  numFrames = 0;
}

MapEvent::WaitFrames::WaitFrames(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::WaitFrames)
{
  numFrames = atoi(eventNode.getAttribute("frames"));
}

MapEvent::WaitFrames::~WaitFrames()
{
}

XMLNode MapEvent::WaitFrames::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("waitFrames");
  eventNode.addAttribute("frames", QString::number(numFrames).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::WaitFrames::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(numFrames);
}

bool MapEvent::WaitFrames::configureEvent(QWidget *parentWidget)
{
  uint8_t newValue;
  bool ok;
  newValue = QInputDialog::getInt(parentWidget, "Number of Frames", "Frames:", numFrames, 0, 255, 1, &ok);
  if (ok)
    numFrames = newValue;
  return ok;
}

QTreeWidgetItem *MapEvent::WaitFrames::getItem()
{
  item->setText(0, QString("<> waitFrames: %1").arg(numFrames));
  return item;
}
//END: WaitFrames

//BEGIN: Battle (COMPLETE)
MapEvent::Battle::Battle(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::Battle)
{
  background = Globals::backdrops[0];
  music = Globals::bgms[0];
  monster = 0;
}

MapEvent::Battle::Battle(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::Battle)
{
  monster = atoi(eventNode.getAttribute("monster"));
  background = eventNode.getAttribute("background");
  music = eventNode.getAttribute("music");
}

MapEvent::Battle::~Battle()
{
}

XMLNode MapEvent::Battle::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("battle");
  eventNode.addAttribute("monster", QString::number(monster).toLocal8Bit().data());
  eventNode.addAttribute("background", background.toLocal8Bit().data());
  eventNode.addAttribute("music", music.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::Battle::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(monster);
  bytes->append((uint8_t)Globals::backdrops.indexOf(background));
  bytes->append((uint8_t)Globals::bgms.indexOf(music));
}

bool MapEvent::Battle::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_Battle *configure = new ConfigureEvent_Battle(parentWidget);
  configure->setBackground(background);
  configure->setMusic(music);
  configure->setMonster(monster);
  if (configure->exec())
  {
    background = configure->getBackground();
    music = configure->getMusic();
    monster = configure->getMonster();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::Battle::getItem()
{
  //TODO: get monster name
  item->setText(0, QString("<> battle: \"%1\" in \"%2\" playing \"%3\"").arg(monster).arg(background).arg(music));
  return item;
}
//END: Battle

//BEGIN: ShakeScreen (COMPLETE)
MapEvent::ShakeScreen::ShakeScreen(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ShakeScreen)
{
  duration = 0;
  magnitude = 0;
}

MapEvent::ShakeScreen::ShakeScreen(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ShakeScreen)
{
  duration = atoi(eventNode.getAttribute("duration"));
  magnitude = atoi(eventNode.getAttribute("magnitude"));
}

MapEvent::ShakeScreen::~ShakeScreen()
{
}

XMLNode MapEvent::ShakeScreen::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("shakeScreen");
  eventNode.addAttribute("duration", QString::number(duration).toLocal8Bit().data());
  eventNode.addAttribute("magnitude", QString::number(magnitude).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ShakeScreen::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(duration);
  bytes->append(magnitude);
}

bool MapEvent::ShakeScreen::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ShakeScreen *configure = new ConfigureEvent_ShakeScreen(parentWidget);
  configure->setDuration(duration);
  configure->setMagnitude(magnitude);
  if (configure->exec())
  {
    duration = configure->getDuration();
    magnitude = configure->getMagnitude();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ShakeScreen::getItem()
{
  item->setText(0, QString("<> shakeScreen: duration=%1 magnitude=%2").arg(duration).arg(magnitude));
  return item;
}
//END: ShakeScreen

//BEGIN: Cutscene (NOT IMPLEMENTED)
MapEvent::Cutscene::Cutscene(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::Cutscene)
{
  location = "";
}

MapEvent::Cutscene::Cutscene(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::Cutscene)
{
  location = eventNode.getAttribute("location");
}

MapEvent::Cutscene::~Cutscene()
{
}

XMLNode MapEvent::Cutscene::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("cutscene");
  eventNode.addAttribute("location", location.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::Cutscene::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  Globals::Value32 hash;
  bytes->append(static_cast<char>(type));
  hash.uValue = Globals::hash(location.toLocal8Bit().data(), location.length() + 1);
  bytes->append(hash.bytes[0]);
  bytes->append(hash.bytes[1]);
  bytes->append(hash.bytes[2]);
  bytes->append(hash.bytes[3]);
}

bool MapEvent::Cutscene::configureEvent(QWidget *parentWidget)
{
  Q_UNUSED(parentWidget);
  //TODO
  return true;
}

QTreeWidgetItem *MapEvent::Cutscene::getItem()
{
  if (item == nullptr)
    item = new QTreeWidgetItem(QStringList() << QString("<> cutscene: \"%1\"").arg(location));
  return item;
}
//END: Cutscene

//BEGIN: Jump (COMPLETE)
MapEvent::Jump::Jump(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::Jump)
{
  label = "";
}

MapEvent::Jump::Jump(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::Jump)
{
  label = eventNode.getAttribute("label");
}

MapEvent::Jump::~Jump()
{
}

XMLNode MapEvent::Jump::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("jump");
  eventNode.addAttribute("label", label.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::Jump::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  for (auto event : pMapEvent->allEvents)
  {
    if (event->getType() == Type::Label && ((Label*)event)->getName() == label)
    {
      JumpLocation jump;
      jump.event = event;
      jump.pos = bytes->count();
      pMapEvent->jumps += jump;
      break;
    }
  }
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
}

bool MapEvent::Jump::configureEvent(QWidget *parentWidget)
{
  QStringList labels;
  QString newValue;
  bool ok;
  for (auto event : pMapEvent->allEvents)
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

QTreeWidgetItem *MapEvent::Jump::getItem()
{
  if (item == nullptr)
    item = new QTreeWidgetItem;
  item->setText(0, QString("<> jump: \"%1\"").arg(label));
  item->setBackground(0, Qt::red);
  item->setForeground(0, Qt::white);
  for (auto event : pMapEvent->allEvents)
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

//BEGIN: IfSwitch (COMPLETE)
MapEvent::IfSwitch::IfSwitch(MapEvent *pBase, Event *pEvent) : IfEvent(pBase, pEvent, Type::IfSwitch)
{
  switchID = 0;
  isOn = true;
}

MapEvent::IfSwitch::IfSwitch(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : IfEvent(pBase, pEvent, Type::IfSwitch, eventNode)
{
  switchID = atoi(eventNode.getAttribute("switchID"));
  isOn = strcmp(eventNode.getAttribute("is"), "ON") == 0;
}

MapEvent::IfSwitch::~IfSwitch()
{
  for (auto event : events)
    delete event;
  events.clear();
}

XMLNode MapEvent::IfSwitch::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfSwitch":"ifSwitch");
  eventNode.addAttribute("switchID", QString::number(switchID).toLocal8Bit().data());
  eventNode.addAttribute("is", (isOn) ? "ON":"OFF");
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::IfSwitch::compileEvent(QByteArray *bytes)
{
  JumpLocation offJump, onJump;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  offJump.pos = bytes->count();
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  onJump.pos = bytes->count();
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  bytes->append(switchID);
  if (isOn)
  {
    onJump.event = events[0];
    offJump.event = parent->nextChildEvent(this);
  }
  else
  {
    onJump.event = parent->nextChildEvent(this);
    offJump.event = events[0];
  }
  pMapEvent->jumps += onJump;
  pMapEvent->jumps += offJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool MapEvent::IfSwitch::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_IfSwitch *configure = new ConfigureEvent_IfSwitch(parentWidget);
  Event *previous = parent->previousChildEvent(this);
  if (previous != nullptr && previous->isIf())
    configure->enableElseIf();
  configure->setElseIf(elseIf);
  configure->setSwitchID(switchID);
  configure->setIsOn(isOn);
  if (configure->exec())
  {
    elseIf = configure->isElseIf();
    switchID = configure->getSwitchID();
    isOn = configure->getIsOn();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::IfSwitch::getItem()
{
  if (item->childCount() == 0)
    addItems();
  item->setText(0, QString("<> %1: %2 is %3").arg(elseIf ? "elseIfSwitch":"ifSwitch").arg(switchID).arg(isOn ? "ON":"OFF"));
  return item;
}
//END: IfSwitch

//BEGIN: IfVariable (COMPLETE)
MapEvent::IfVariable::IfVariable(MapEvent *pBase, Event *pEvent) : IfEvent(pBase, pEvent, Type::IfVariable)
{
  value = 0;
  variableID = 0;
  condition = 0;
}

MapEvent::IfVariable::IfVariable(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : IfEvent(pBase, pEvent, Type::IfVariable, eventNode)
{
  QString conditionString = eventNode.getAttribute("condition");
  variableID = atoi(eventNode.getAttribute("variableID"));
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
  value = strtol(eventNode.getAttribute("value"), NULL, 10);
}

MapEvent::IfVariable::~IfVariable()
{
}

XMLNode MapEvent::IfVariable::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfVariable":"ifVariable");
  eventNode.addAttribute("variableID", QString::number(variableID).toLocal8Bit().data());
  eventNode.addAttribute("condition", conditions[condition]);
  eventNode.addAttribute("value", QString::number(value).toLocal8Bit().data());
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::IfVariable::compileEvent(QByteArray *bytes)
{
  JumpLocation trueJump, falseJump;
  Globals::Value32 value32;
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
  value32.sValue = value;
  bytes->append(value32.bytes[0]);
  bytes->append(value32.bytes[1]);
  bytes->append(value32.bytes[2]);
  bytes->append(value32.bytes[3]);
  bytes->append(variableID);
  bytes->append(condition);
  pMapEvent->jumps += trueJump;
  pMapEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool MapEvent::IfVariable::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_IfVariable *configure = new ConfigureEvent_IfVariable(parentWidget);
  Event *previous = parent->previousChildEvent(this);
  if (previous != nullptr && previous->isIf())
    configure->enableElseIf();
  configure->setElseIf(elseIf);
  configure->setVariableID(variableID);
  configure->setCondition(condition);
  configure->setValue(value);
  if (configure->exec())
  {
    elseIf = configure->isElseIf();
    variableID = configure->getVariableID();
    condition = configure->getCondition();
    value = configure->getValue();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::IfVariable::getItem()
{
  if (item->childCount() == 0)
    addItems();
  item->setText(0, QString("<> %1: %2 %3 %4").arg((elseIf) ? "elseIfVariable":"ifVariable").arg(variableID).arg(conditions[condition]).arg(value));
  return item;
}
//END: IfVariable

//BEGIN: IfHasItem (COMPLETE)
MapEvent::IfHasItem::IfHasItem(MapEvent *pBase, Event *pEvent) : IfEvent(pBase, pEvent, Type::IfHasItem)
{
  itemType = 0;
  itemID = 0;
}

MapEvent::IfHasItem::IfHasItem(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : IfEvent(pBase, pEvent, Type::IfHasItem, eventNode)
{
  itemType = atoi(eventNode.getAttribute("type"));
  itemID = atoi(eventNode.getAttribute("item"));
}

MapEvent::IfHasItem::~IfHasItem()
{
}

XMLNode MapEvent::IfHasItem::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfHasItem":"ifHasItem");
  eventNode.addAttribute("type", QString::number(itemType).toLocal8Bit().data());
  eventNode.addAttribute("item", QString::number(itemID).toLocal8Bit().data());
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::IfHasItem::compileEvent(QByteArray *bytes)
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
  bytes->append((itemType << 4) + itemID);
  pMapEvent->jumps += trueJump;
  pMapEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool MapEvent::IfHasItem::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_IfHasItem *configure = new ConfigureEvent_IfHasItem(parentWidget);
  Event *previous = parent->previousChildEvent(this);
  if (previous != nullptr && previous->isIf())
    configure->enableElseIf();
  configure->setElseIf(elseIf);
  configure->setItem(itemType, itemID);
  if (configure->exec())
  {
    elseIf = configure->isElseIf();
    itemType = configure->getType();
    itemID = configure->getItem();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::IfHasItem::getItem()
{
  if (item->childCount() == 0)
    addItems();
  item->setText(0, QString("<> %1: %2").arg(elseIf ? "elseIfHasItem":"ifHasItem").arg(Globals::items[itemType][itemID].name));
  return item;
}
//END: IfHasItem

//BEGIN: IfExited (COMPLETE)
MapEvent::IfExited::IfExited(MapEvent *pBase, Event *pEvent) : IfEvent(pBase, pEvent, Type::IfExited)
{
  facing = 0;
}

MapEvent::IfExited::IfExited(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : IfEvent(pBase, pEvent, Type::IfExited, eventNode)
{
  QString facingString = eventNode.getAttribute("facing");
  if (facingString == "up")
    facing = DIR_UP;
  else if (facingString == "right")
    facing = DIR_RIGHT;
  else if (facingString == "down")
    facing = DIR_DOWN;
  else if (facingString == "left")
    facing = DIR_DOWN;
}

MapEvent::IfExited::~IfExited()
{
}

XMLNode MapEvent::IfExited::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfExited":"ifExited");
  eventNode.addAttribute("facing", facings[facing]);
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::IfExited::compileEvent(QByteArray *bytes)
{
  JumpLocation trueJump, falseJump;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(facing);
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
  pMapEvent->jumps += trueJump;
  pMapEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool MapEvent::IfExited::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_IfExited *configure = new ConfigureEvent_IfExited(parentWidget);
  Event *previous = parent->previousChildEvent(this);
  if (previous != nullptr && previous->isIf())
    configure->enableElseIf();
  configure->setElseIf(elseIf);
  configure->setFacing(facing);
  if (configure->exec())
  {
    elseIf = configure->isElseIf();
    facing = configure->getFacing();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::IfExited::getItem()
{
  if (item->childCount() == 0)
    addItems();
  item->setText(0, QString("<> %1: %2").arg(elseIf ? "elseIfExited":"ifExited").arg(facings[facing]));
  return item;
}
//END: IfExited

//BEGIN: IfYesNo (COMPLETE)
MapEvent::IfYesNo::IfYesNo(MapEvent *pBase, Event *pEvent) : IfEvent(pBase, pEvent, Type::IfYesNo)
{
  yes = true;
}

MapEvent::IfYesNo::IfYesNo(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : IfEvent(pBase, pEvent, Type::IfYesNo, eventNode)
{
  yes = QString(eventNode.getName()).endsWith("Yes");
}

MapEvent::IfYesNo::~IfYesNo()
{
}

XMLNode MapEvent::IfYesNo::toXMLNode()
{
  XMLNode eventNode;
  if (yes)
    eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfYes":"ifYes");
  else
    eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfNo":"ifNo");
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::IfYesNo::compileEvent(QByteArray *bytes)
{
  JumpLocation yesJump, noJump;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  yesJump.pos = bytes->count();
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  noJump.pos = bytes->count();
  bytes->append(0xDE);
  bytes->append(0xAD);
  bytes->append(0xC0);
  bytes->append(0xDE);
  if (yes)
  {
    yesJump.event = events[0];
    noJump.event = parent->nextChildEvent(this);
  }
  else
  {
    yesJump.event = parent->nextChildEvent(this);
    noJump.event = events[0];
  }
  pMapEvent->jumps += yesJump;
  pMapEvent->jumps += noJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool MapEvent::IfYesNo::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_IfYesNo *configure = new ConfigureEvent_IfYesNo(parentWidget);
  Event *previous = parent->previousChildEvent(this);
  if (previous != nullptr && previous->isIf())
    configure->enableElseIf();
  configure->setElseIf(elseIf);
  configure->setYes(yes);
  if (configure->exec())
  {
    elseIf = configure->isElseIf();
    yes = configure->isYes();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::IfYesNo::getItem()
{
  if (item->childCount() == 0)
    addItems();
  item->setText(0, QString("<> %1%2").arg(elseIf ? "elseIf":"if").arg((yes) ? "Yes":"No"));
  return item;
}
//END: IfYesNo

//BEGIN: IfValue (COMPLETE)
MapEvent::IfValue::IfValue(MapEvent *pBase, Event *pEvent) : IfEvent(pBase, pEvent, Type::IfValue)
{
  desiredValue = 0;
  valueToCheck = 0;
  condition = 0;
}

MapEvent::IfValue::IfValue(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : IfEvent(pBase, pEvent, Type::IfValue, eventNode)
{
  QString conditionString = eventNode.getAttribute("condition");
  valueToCheck = atoi(eventNode.getAttribute("valueToCheck"));
  desiredValue = strtol(eventNode.getAttribute("desiredValue"), NULL, 10);
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

MapEvent::IfValue::~IfValue()
{
}

XMLNode MapEvent::IfValue::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode((elseIf) ? "elseIfValue":"ifValue");
  eventNode.addAttribute("valueToCheck", QString::number(valueToCheck).toLocal8Bit().data());
  eventNode.addAttribute("condition", conditions[condition]);
  eventNode.addAttribute("desiredValue", QString::number(desiredValue).toLocal8Bit().data());
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::IfValue::compileEvent(QByteArray *bytes)
{
  JumpLocation trueJump, falseJump;
  Globals::Value32 value32;
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
  bytes->append(valueToCheck);
  bytes->append(condition);
  value32.sValue = desiredValue;
  bytes->append(value32.bytes[0]);
  bytes->append(value32.bytes[1]);
  bytes->append(value32.bytes[2]);
  bytes->append(value32.bytes[3]);
  pMapEvent->jumps += trueJump;
  pMapEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool MapEvent::IfValue::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_IfValue *configure = new ConfigureEvent_IfValue(parentWidget);
  Event *previous = parent->previousChildEvent(this);
  if (previous != nullptr && previous->isIf())
    configure->enableElseIf();
  configure->setElseIf(elseIf);
  configure->setValueToCheck(valueToCheck);
  configure->setCondition(condition);
  configure->setValue(desiredValue);
  if (configure->exec())
  {
    elseIf = configure->isElseIf();
    valueToCheck = configure->getValueToCheck();
    condition = configure->getCondition();
    desiredValue = configure->getValue();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::IfValue::getItem()
{
  if (item->childCount() == 0)
    addItems();
  item->setText(0, QString("<> %1: %2 %3 %4").arg(elseIf ? "elseIfValue":"ifValue").arg(bufferValues[valueToCheck]).arg(conditions[condition]).arg(desiredValue));
  return item;
}
//END: IfValue

//BEGIN: IfFacing (COMPLETE)
MapEvent::IfFacing::IfFacing(MapEvent *pBase, Event *pEvent) : IfEvent(pBase, pEvent, Type::IfFacing)
{
  direction = 0;
}

MapEvent::IfFacing::IfFacing(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : IfEvent(pBase, pEvent, Type::IfFacing, eventNode)
{
  QString directionString = eventNode.getAttribute("direction");
  if (directionString == "up")
    direction = DIR_UP;
  else if (directionString == "right")
    direction = DIR_RIGHT;
  else if (directionString == "down")
    direction = DIR_DOWN;
  else if (directionString == "left")
    direction = DIR_LEFT;
}

MapEvent::IfFacing::~IfFacing()
{
}

XMLNode MapEvent::IfFacing::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("ifFacing");
  eventNode.addAttribute("direction", facings[direction]);
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::IfFacing::compileEvent(QByteArray *bytes)
{
  JumpLocation trueJump, falseJump;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(direction);
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
  pMapEvent->jumps += trueJump;
  pMapEvent->jumps += falseJump;
  for (auto event : events)
    event->compileEvent(bytes);
}

bool MapEvent::IfFacing::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_IfExited *configure = new ConfigureEvent_IfExited(parentWidget);
  Event *previous = parent->previousChildEvent(this);
  configure->setWindowTitle("If Facing");
  if (previous != nullptr && previous->isIf())
    configure->enableElseIf();
  configure->setElseIf(elseIf);
  configure->setFacing(direction);
  if (configure->exec())
  {
    elseIf = configure->isElseIf();
    direction = configure->getFacing();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::IfFacing::getItem()
{
  if (item->childCount() == 0)
    addItems();
  item->setText(0, QString("<> %1: %2").arg(elseIf ? "elseIfFacing":"ifFacing").arg(facings[direction]));
  return item;
}
//END: IfFacing

//BEGIN: PlaySoundEffect (COMPLETE)
MapEvent::PlaySoundEffect::PlaySoundEffect(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::PlaySoundEffect)
{
  sfx = Globals::sfx[0];
  loop = false;
}

MapEvent::PlaySoundEffect::PlaySoundEffect(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::PlaySoundEffect)
{
  sfx = eventNode.getAttribute("sound");
  loop = strcmp(eventNode.getAttribute("loop"), "true") == 0;
}

MapEvent::PlaySoundEffect::~PlaySoundEffect()
{
}

XMLNode MapEvent::PlaySoundEffect::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("playSoundEffect");
  eventNode.addAttribute("sound", sfx.toLocal8Bit().data());
  eventNode.addAttribute("loop", (loop) ? "true":"false");
  return eventNode;
}

void MapEvent::PlaySoundEffect::compileEvent(QByteArray *bytes)
{
  uint8_t sfxID = Globals::sfx.indexOf(sfx);
  offset = bytes->count();
  if (loop)
    sfxID |= 0x80;
  bytes->append(static_cast<char>(type));
  bytes->append(sfxID);
}

bool MapEvent::PlaySoundEffect::configureEvent(QWidget *parentWidget)
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

QTreeWidgetItem *MapEvent::PlaySoundEffect::getItem()
{
  item->setText(0, QString("<> playSoundEffect: %1%2").arg(sfx).arg((loop) ? " (loop)":""));
  return item;
}
//END: PlaySoundEffect

//BEGIN: PlayMusic (COMPLETE)
MapEvent::PlayMusic::PlayMusic(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::PlayMusic)
{
  bgm = Globals::bgms[0];
  loop = true;
}

MapEvent::PlayMusic::PlayMusic(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::PlayMusic)
{
  bgm = eventNode.getAttribute("music");
  loop = strcmp(eventNode.getAttribute("loop"), "true") == 0;
}

MapEvent::PlayMusic::~PlayMusic()
{
}

XMLNode MapEvent::PlayMusic::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("playMusic");
  eventNode.addAttribute("music", bgm.toLocal8Bit().data());
  eventNode.addAttribute("loop", (loop) ? "true":"false");
  return eventNode;
}

void MapEvent::PlayMusic::compileEvent(QByteArray *bytes)
{
  uint8_t bgmID = Globals::bgms.indexOf(bgm);
  offset = bytes->count();
  if (loop)
    bgmID |= 0x80;
  bytes->append(static_cast<char>(type));
  bytes->append(bgmID);
}

bool MapEvent::PlayMusic::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_PlayMusic *configure = new ConfigureEvent_PlayMusic(parentWidget);
  configure->setMusic(bgm);
  configure->setLoop(loop);
  if (configure->exec())
  {
    bgm = configure->getMusic();
    loop = configure->getLoop();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::PlayMusic::getItem()
{
  //TODO: get music name
  item->setText(0, QString("<> playMusic: %1").arg(bgm));
  return item;
}
//END: PlayMusic

//BEGIN: TurnSwitchOn (COMPLETE)
MapEvent::TurnSwitchOn::TurnSwitchOn(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::TurnSwitchOn)
{
  switchID = 0;
}

MapEvent::TurnSwitchOn::TurnSwitchOn(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::TurnSwitchOn)
{
  switchID = atoi(eventNode.getAttribute("switchID"));
}

MapEvent::TurnSwitchOn::~TurnSwitchOn()
{
}

XMLNode MapEvent::TurnSwitchOn::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("turnSwitchOn");
  eventNode.addAttribute("switchID", QString::number(switchID).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::TurnSwitchOn::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(switchID);
}

bool MapEvent::TurnSwitchOn::configureEvent(QWidget *parentWidget)
{
  int newValue;
  bool ok;
  newValue = QInputDialog::getInt(parentWidget, "Turn Switch On", "Switch:", switchID, 0, 63, 1, &ok);
  if (ok)
    switchID = newValue;
  return ok;
}

QTreeWidgetItem *MapEvent::TurnSwitchOn::getItem()
{
  //TODO: possibly named switches
  item->setText(0, QString("<> turnSwitchOn: %1").arg(switchID));
  return item;
}
//END: TurnSwitchOn

//BEGIN: TurnSwitchOff (COMPLETE)
MapEvent::TurnSwitchOff::TurnSwitchOff(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::TurnSwitchOff)
{
  switchID = 0;
}

MapEvent::TurnSwitchOff::TurnSwitchOff(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::TurnSwitchOff)
{
  switchID = atoi(eventNode.getAttribute("switchID"));
}

MapEvent::TurnSwitchOff::~TurnSwitchOff()
{
}

XMLNode MapEvent::TurnSwitchOff::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("turnSwitchOff");
  eventNode.addAttribute("switchID", QString::number(switchID).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::TurnSwitchOff::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(switchID);
}

bool MapEvent::TurnSwitchOff::configureEvent(QWidget *parentWidget)
{
  int newValue;
  bool ok;
  newValue = QInputDialog::getInt(parentWidget, "Turn Switch Off", "Switch:", switchID, 0, 63, 1, &ok);
  if (ok)
    switchID = newValue;
  return ok;
}

QTreeWidgetItem *MapEvent::TurnSwitchOff::getItem()
{
  //TODO: possibly named switches
  item->setText(0, QString("<> turnSwitchOff: %1").arg(switchID));
  return item;
}
//END: TurnSwitchOff

//BEGIN: ToggleSwitch (COMPLETE)
MapEvent::ToggleSwitch::ToggleSwitch(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ToggleSwitch)
{
  switchID = 0;
}

MapEvent::ToggleSwitch::ToggleSwitch(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ToggleSwitch)
{
  switchID = atoi(eventNode.getAttribute("switchID"));
}

MapEvent::ToggleSwitch::~ToggleSwitch()
{
}

XMLNode MapEvent::ToggleSwitch::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("toggleSwitch");
  eventNode.addAttribute("switchID", QString::number(switchID).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ToggleSwitch::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(switchID);
}

bool MapEvent::ToggleSwitch::configureEvent(QWidget *parentWidget)
{
  int newValue;
  bool ok;
  newValue = QInputDialog::getInt(parentWidget, "Toggle Switch", "Switch:", switchID, 0, 63, 1, &ok);
  if (ok)
    switchID = newValue;
  return ok;
}

QTreeWidgetItem *MapEvent::ToggleSwitch::getItem()
{
  //TODO: possibly named switches
  item->setText(0, QString("<> toggleSwitch: %1").arg(switchID));
  return item;
}
//END: ToggleSwitch

//BEGIN: TurnSwitchRangeOn (COMPLETE)
MapEvent::TurnSwitchRangeOn::TurnSwitchRangeOn(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::TurnSwitchRangeOn)
{
  startSwitchID = 0;
  endSwitchID = 0;
}

MapEvent::TurnSwitchRangeOn::TurnSwitchRangeOn(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::TurnSwitchRangeOn)
{
  startSwitchID = atoi(eventNode.getAttribute("startID"));
  endSwitchID = atoi(eventNode.getAttribute("endID"));
}

MapEvent::TurnSwitchRangeOn::~TurnSwitchRangeOn()
{
}

XMLNode MapEvent::TurnSwitchRangeOn::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("turnSwitchRangeOn");
  eventNode.addAttribute("startID", QString::number(startSwitchID).toLocal8Bit().data());
  eventNode.addAttribute("endID", QString::number(endSwitchID).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::TurnSwitchRangeOn::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(startSwitchID);
  bytes->append(endSwitchID);
}

bool MapEvent::TurnSwitchRangeOn::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_SwitchRange *configure = new ConfigureEvent_SwitchRange(parentWidget);
  configure->setWindowTitle("Turn Switch Range On");
  configure->setStartSwitchID(startSwitchID);
  configure->setEndSwitchID(endSwitchID);
  if (configure->exec())
  {
    startSwitchID = configure->getStartSwitchID();
    endSwitchID = configure->getEndSwitchID();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::TurnSwitchRangeOn::getItem()
{
  //TODO: possibly named switches
  item->setText(0, QString("<> turnSwitchRangeOn: %1 - %2").arg(startSwitchID).arg(endSwitchID));
  return item;
}
//END: TurnSwitchRangeOn

//BEGIN: TurnSwitchRangeOff (COMPLETE)
MapEvent::TurnSwitchRangeOff::TurnSwitchRangeOff(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::TurnSwitchRangeOff)
{
  startSwitchID = 0;
  endSwitchID = 0;
}

MapEvent::TurnSwitchRangeOff::TurnSwitchRangeOff(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::TurnSwitchRangeOff)
{
  startSwitchID = atoi(eventNode.getAttribute("startID"));
  endSwitchID = atoi(eventNode.getAttribute("endID"));
}

MapEvent::TurnSwitchRangeOff::~TurnSwitchRangeOff()
{
}

XMLNode MapEvent::TurnSwitchRangeOff::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("turnSwitchRangeOff");
  eventNode.addAttribute("startID", QString::number(startSwitchID).toLocal8Bit().data());
  eventNode.addAttribute("endID", QString::number(endSwitchID).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::TurnSwitchRangeOff::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(startSwitchID);
  bytes->append(endSwitchID);
}

bool MapEvent::TurnSwitchRangeOff::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_SwitchRange *configure = new ConfigureEvent_SwitchRange(parentWidget);
  configure->setWindowTitle("Turn Switch Range Off");
  configure->setStartSwitchID(startSwitchID);
  configure->setEndSwitchID(endSwitchID);
  if (configure->exec())
  {
    startSwitchID = configure->getStartSwitchID();
    endSwitchID = configure->getEndSwitchID();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::TurnSwitchRangeOff::getItem()
{
  //TODO: possibly named switches
  item->setText(0, QString("<> turnSwitchRangeOff: %1 - %2").arg(startSwitchID).arg(endSwitchID));
  return item;
}
//END: TurnSwitchRangeOff

//BEGIN: ToggleSwitchRange (COMPLETE)
MapEvent::ToggleSwitchRange::ToggleSwitchRange(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ToggleSwitchRange)
{
  startSwitchID = 0;
  endSwitchID = 0;
}

MapEvent::ToggleSwitchRange::ToggleSwitchRange(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ToggleSwitchRange)
{
  startSwitchID = atoi(eventNode.getAttribute("startID"));
  endSwitchID = atoi(eventNode.getAttribute("endID"));
}

MapEvent::ToggleSwitchRange::~ToggleSwitchRange()
{
}

XMLNode MapEvent::ToggleSwitchRange::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("toggleSwitchRange");
  eventNode.addAttribute("startID", QString::number(startSwitchID).toLocal8Bit().data());
  eventNode.addAttribute("endID", QString::number(endSwitchID).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ToggleSwitchRange::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(startSwitchID);
  bytes->append(endSwitchID);
}

bool MapEvent::ToggleSwitchRange::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_SwitchRange *configure = new ConfigureEvent_SwitchRange(parentWidget);
  configure->setWindowTitle("Toggle Switch Range");
  configure->setStartSwitchID(startSwitchID);
  configure->setEndSwitchID(endSwitchID);
  if (configure->exec())
  {
    startSwitchID = configure->getStartSwitchID();
    endSwitchID = configure->getEndSwitchID();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ToggleSwitchRange::getItem()
{
  //TODO: possibly named switches
  item->setText(0, QString("<> toggleSwitchRange: %1 - %2").arg(startSwitchID).arg(endSwitchID));
  return item;
}
//END: ToggleSwitchRange

//BEGIN: ChangeVariable (COMPLETE)
MapEvent::ChangeVariable::ChangeVariable(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ChangeVariable)
{
  value = 0;
  variableID = 0;
  operation = 0;
}

MapEvent::ChangeVariable::ChangeVariable(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ChangeVariable)
{
  variableID = atoi(eventNode.getAttribute("variableID"));
  operation = atoi(eventNode.getAttribute("operation"));
  value = strtol(eventNode.getAttribute("value"), NULL, 10);
}

MapEvent::ChangeVariable::~ChangeVariable()
{
}

XMLNode MapEvent::ChangeVariable::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("changeVariable");
  eventNode.addAttribute("variableID", QString::number(variableID).toLocal8Bit().data());
  eventNode.addAttribute("operation", QString::number(operation).toLocal8Bit().data());
  eventNode.addAttribute("value", QString::number(value).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ChangeVariable::compileEvent(QByteArray *bytes)
{
  Globals::Value32 value32;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  value32.sValue = value;
  bytes->append(value32.bytes[0]);
  bytes->append(value32.bytes[1]);
  bytes->append(value32.bytes[2]);
  bytes->append(value32.bytes[3]);
  bytes->append(variableID);
  bytes->append(operation);
}

bool MapEvent::ChangeVariable::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ChangeVariable *configure = new ConfigureEvent_ChangeVariable(parentWidget);
  configure->setVariable(variableID);
  configure->setOperation(operation);
  configure->setValue(value);
  if (configure->exec())
  {
    variableID = configure->getVariable();
    operation = configure->getOperation();
    value = configure->getValue();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ChangeVariable::getItem()
{
  item->setText(0, QString("<> changeVariable: %1 %2 %3").arg(variableID).arg(operations[operation]).arg(value));
  return item;
}
//END: ChangeVariable

//BEGIN: ChangeSprite (COMPLETE)
MapEvent::ChangeSprite::ChangeSprite(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ChangeSprite)
{
  event = "this";
  spriteID = 0;
}

MapEvent::ChangeSprite::ChangeSprite(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ChangeSprite)
{
  event = eventNode.getAttribute("event");
  spriteID = atoi(eventNode.getAttribute("spriteID"));
}

MapEvent::ChangeSprite::~ChangeSprite()
{
}

XMLNode MapEvent::ChangeSprite::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("changeSprite");
  eventNode.addAttribute("event", event.toLocal8Bit().data());
  eventNode.addAttribute("spriteID", QString::number(spriteID).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ChangeSprite::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  for (uint8_t i = 0; i < 29; ++i)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(i);
    if (mapEvent == nullptr || mapEvent->getName() == event || (event == "this" && mapEvent == pMapEvent))
    {
      bytes->append(i);
      break;
    }
  }
  bytes->append(spriteID);
}

bool MapEvent::ChangeSprite::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ChangeSprite *configure = new ConfigureEvent_ChangeSprite(pMapEvent->map, parentWidget);
  if (pMapEvent != pMapEvent->map->getOnLoadEvent())
    configure->addEvent("this");
  for (int i = 0; i < 29; ++i)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(i);
    if (mapEvent != nullptr)
      configure->addEvent(mapEvent->getName());
    else
      break;
  }
  configure->setEvent(event);
  configure->setSprite(spriteID);
  if (configure->exec())
  {
    event = configure->getEvent();
    spriteID = configure->getSprite();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ChangeSprite::getItem()
{
  item->setText(0, QString("<> changeSprite: \"%1\" %2").arg(event).arg(spriteID));
  item->setBackground(0, Qt::red);
  item->setForeground(0, Qt::white);
  for (int i = 0; i < 29; ++i)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(i);
    if (mapEvent == nullptr)
      break;
    else if (mapEvent->getName() == event || (event == "this" && mapEvent == pMapEvent))
    {
      item->setBackground(0, Qt::NoBrush);
      item->setForeground(0, Qt::NoBrush);
    }
  }
  return item;
}
//END: ChangeSprite

//BEGIN: ChangeEventLocation (COMPLETE)
MapEvent::ChangeEventLocation::ChangeEventLocation(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ChangeEventLocation)
{
  MapEvent *initial;
  if (pBase->map->getOnLoadEvent() == pBase)
    initial = pBase->map->getMapEvent(0);
  else
    initial = pBase;
  event = initial->getName();
  x = initial->getX();
  y = initial->getY();
  offsetFlag = initial->isFlagSet(FLAGS_OFFSET);
}

MapEvent::ChangeEventLocation::ChangeEventLocation(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ChangeEventLocation)
{
  event = eventNode.getAttribute("event");
  x = atoi(eventNode.getAttribute("x"));
  y = atoi(eventNode.getAttribute("y"));
  offsetFlag = strcmp(eventNode.getAttribute("offset"), "true") == 0;
}

MapEvent::ChangeEventLocation::~ChangeEventLocation()
{
}

XMLNode MapEvent::ChangeEventLocation::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("changeEventLocation");
  eventNode.addAttribute("event", event.toLocal8Bit().data());
  eventNode.addAttribute("x", QString::number(x).toLocal8Bit().data());
  eventNode.addAttribute("y", QString::number(y).toLocal8Bit().data());
  eventNode.addAttribute("offset", (offsetFlag) ? "true":"false");
  return eventNode;
}

void MapEvent::ChangeEventLocation::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  for (uint8_t eventID = 0; eventID < 29; ++eventID)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(eventID);
    if (mapEvent == nullptr || mapEvent->getName() == event || (event == "this" && mapEvent == pMapEvent))
    {
      if (offsetFlag)
        eventID |= 128;
      bytes->append(eventID);
      break;
    }
  }
  bytes->append(x);
  bytes->append(y);
}

bool MapEvent::ChangeEventLocation::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ChangeEventLocation *configure = new ConfigureEvent_ChangeEventLocation(pMapEvent->map, pMapEvent->map->getMapEventID(pMapEvent), parentWidget);
  if (pMapEvent != pMapEvent->map->getOnLoadEvent())
    configure->addEvent("this");
  for (int i = 0; i < 29; ++i)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(i);
    if (mapEvent != nullptr)
      configure->addEvent(mapEvent->getName());
    else
      break;
  }
  configure->setEvent(event);
  configure->setLocation(x, y, offsetFlag);
  if (configure->exec())
  {
    event = configure->getEvent();
    x = configure->getX();
    y = configure->getY();
    offsetFlag = configure->getOffset();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ChangeEventLocation::getItem()
{
  item->setText(0, QString("<> changeEventLocation: \"%1\" %2 %3%4").arg(event).arg(x).arg(y).arg((offsetFlag) ? " (offset)":""));
  item->setBackground(0, Qt::red);
  item->setForeground(0, Qt::white);
  for (int i = 0; i < 29; ++i)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(i);
    if (mapEvent == nullptr)
      break;
    else if (mapEvent->getName() == event || (event == "this" && mapEvent == pMapEvent))
    {
      item->setBackground(0, Qt::NoBrush);
      item->setForeground(0, Qt::NoBrush);
    }
  }
  return item;
}
//END: ChangeEventLocation

//BEGIN: ChangeEventFlags (COMPLETE)
MapEvent::ChangeEventFlags::ChangeEventFlags(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ChangeEventFlags)
{
  event = "this";
  flags = 0;
  operation = 0;
}

MapEvent::ChangeEventFlags::ChangeEventFlags(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ChangeEventFlags)
{
  event = eventNode.getAttribute("event");
  flags = atoi(eventNode.getAttribute("flags"));
  operation = atoi(eventNode.getAttribute("operation"));
}

MapEvent::ChangeEventFlags::~ChangeEventFlags()
{
}

XMLNode MapEvent::ChangeEventFlags::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("changeEventFlags");
  eventNode.addAttribute("event", event.toLocal8Bit().data());
  eventNode.addAttribute("flags", QString::number(flags).toLocal8Bit().data());
  eventNode.addAttribute("operation", QString::number(operation).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ChangeEventFlags::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  for (uint8_t eventID = 0; eventID < 29; ++eventID)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(eventID);
    if (mapEvent == nullptr || mapEvent->getName() == event || (event == "this" && mapEvent == pMapEvent))
    {
      if (operation == FLAG_CLEAR)
        eventID |= 64;
      else if (operation == FLAG_TOGGLE)
        eventID |= 128;
      bytes->append(eventID);
      break;
    }
  }
  bytes->append(flags);
}

bool MapEvent::ChangeEventFlags::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ChangeEventFlags *configure = new ConfigureEvent_ChangeEventFlags(parentWidget);
  if (pMapEvent != pMapEvent->map->getOnLoadEvent())
    configure->addEvent("this");
  for (int i = 0; i < 29; ++i)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(i);
    if (mapEvent != nullptr)
      configure->addEvent(mapEvent->getName());
    else
      break;
  }
  configure->setEvent(event);
  configure->setOperation(operation);
  configure->setFlags(flags);
  if (configure->exec())
  {
    event = configure->getEvent();
    operation = configure->getOperation();
    flags = configure->getFlags();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ChangeEventFlags::getItem()
{
  QStringList flagsList;
  QString op;
  if ((flags & FLAGS_ACTIVATE_BUTTON) != 0)
    flagsList += "button";
  if ((flags & FLAGS_DOOR) != 0)
    flagsList += "door";
  if ((flags & FLAGS_EXIT) != 0)
    flagsList += "exit";
  if ((flags & FLAGS_OFFSET) != 0)
    flagsList += "offset";
  if (operation == FLAG_SET)
    op = "SET";
  else if (operation == FLAG_CLEAR)
    op = "CLEAR";
  else
    op = "TOGGLE";
  item->setText(0, QString("<> changeEventFlags: \"%1\" %2 %3").arg(event).arg(op).arg(flagsList.join(",")));
  item->setBackground(0, Qt::red);
  item->setForeground(0, Qt::white);
  for (int i = 0; i < 29; ++i)
  {
    MapEvent *mapEvent = pMapEvent->map->getMapEvent(i);
    if (mapEvent == nullptr)
      break;
    else if (mapEvent->getName() == event || (event == "this" && mapEvent == pMapEvent))
    {
      item->setBackground(0, Qt::NoBrush);
      item->setForeground(0, Qt::NoBrush);
    }
  }
  return item;
}
//END: ChangeEventFlags

//BEGIN: ChangePassability (COMPLETE)
MapEvent::ChangePassability::ChangePassability(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ChangePassability)
{
  x = 0;
  y = 0;
  operation = 0;
}

MapEvent::ChangePassability::ChangePassability(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ChangePassability)
{
  x = atoi(eventNode.getAttribute("x"));
  y = atoi(eventNode.getAttribute("y"));
  operation = atoi(eventNode.getAttribute("operation"));
}

MapEvent::ChangePassability::~ChangePassability()
{
}

XMLNode MapEvent::ChangePassability::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("changePassability");
  eventNode.addAttribute("x", QString::number(x).toLocal8Bit().data());
  eventNode.addAttribute("y", QString::number(y).toLocal8Bit().data());
  eventNode.addAttribute("operation", QString::number(operation).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ChangePassability::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append(x);
  bytes->append(y);
  bytes->append(operation);
}

bool MapEvent::ChangePassability::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ChangePassability *configure = new ConfigureEvent_ChangePassability(pMapEvent->map, parentWidget);
  configure->setLocation(x, y);
  configure->setOperation(operation);
  if (configure->exec())
  {
    x = configure->getX();
    y = configure->getY();
    operation = configure->getOperation();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ChangePassability::getItem()
{
  QString op;
  if (operation == FLAG_SET)
    op = "SET";
  else if (operation == FLAG_CLEAR)
    op = "CLEAR";
  else
    op = "TOGGLE";
  item->setText(0, QString("<> changePassability: %1 (%2, %3)").arg(op).arg(x).arg(y));
  return item;
}
//END: ChangePassability

//BEGIN: GivePlayerItem (COMPLETE)
MapEvent::GivePlayerItem::GivePlayerItem(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::GivePlayerItem)
{
  itemType = 0;
  itemID = 0;
  amount = 0;
}

MapEvent::GivePlayerItem::GivePlayerItem(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::GivePlayerItem)
{
  itemType = atoi(eventNode.getAttribute("type"));
  itemID = atoi(eventNode.getAttribute("item"));
  if (itemType == 0 || itemType == 1)
    amount = atoi(eventNode.getAttribute("amount"));
  else
    amount = 0;
}

MapEvent::GivePlayerItem::~GivePlayerItem()
{
}

XMLNode MapEvent::GivePlayerItem::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("givePlayerItem");
  eventNode.addAttribute("type", QString::number(itemType).toLocal8Bit().data());
  eventNode.addAttribute("item", QString::number(itemID).toLocal8Bit().data());
  if (itemType == 0 || itemType == 1)
    eventNode.addAttribute("amount", QString::number(amount).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::GivePlayerItem::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  bytes->append((itemType << 4) + itemID);
  if (itemType == 0 || itemType == 1)
    bytes->append(amount);
}

bool MapEvent::GivePlayerItem::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_GivePlayerItem *configure = new ConfigureEvent_GivePlayerItem(parentWidget);
  configure->setAmount(amount);
  configure->setItem(itemType, itemID);
  if (configure->exec())
  {
    amount = configure->getAmount();
    itemType = configure->getType();
    itemID = configure->getItem();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::GivePlayerItem::getItem()
{
  item->setText(0, QString("<> givePlayerItem: %1 x%2").arg(Globals::items[itemType][itemID].name).arg((itemType < 2) ? amount:1));
  return item;
}
//END: GivePlayerItem

//BEGIN: GivePlayerGold (COMPLETE)
MapEvent::GivePlayerGold::GivePlayerGold(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::GivePlayerGold)
{
  amount = 0;
}

MapEvent::GivePlayerGold::GivePlayerGold(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::GivePlayerGold)
{
  amount = atoi(eventNode.getAttribute("amount"));
}

MapEvent::GivePlayerGold::~GivePlayerGold()
{
}

XMLNode MapEvent::GivePlayerGold::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("givePlayerGold");
  eventNode.addAttribute("amount", QString::number(amount).toLocal8Bit().data());
  return eventNode;
}

void MapEvent::GivePlayerGold::compileEvent(QByteArray *bytes)
{
  Globals::Value16 value16;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  value16.sValue = amount;
  bytes->append(value16.bytes[0]);
  bytes->append(value16.bytes[1]);
}

bool MapEvent::GivePlayerGold::configureEvent(QWidget *parentWidget)
{
  int16_t newValue;
  bool ok;
  newValue = QInputDialog::getInt(parentWidget, "Give Player Gold", "Amount:", amount, -99999, 99999, 1, &ok);
  if (ok)
    amount = newValue;
  return ok;
}

QTreeWidgetItem *MapEvent::GivePlayerGold::getItem()
{
  item->setText(0, QString("<> givePlayerGold: %1").arg(amount));
  return item;
}
//END: GivePlayerGold

//BEGIN: ShowImage (COMPLETE)
MapEvent::ShowImage::ShowImage(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::ShowImage)
{
  location = "";
}

MapEvent::ShowImage::ShowImage(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::ShowImage)
{
  location = eventNode.getAttribute("location");
}

MapEvent::ShowImage::~ShowImage()
{
}

XMLNode MapEvent::ShowImage::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("showImage");
  eventNode.addAttribute("location", location.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::ShowImage::compileEvent(QByteArray *bytes)
{
  QString path = QString("/screens/%2.png").arg(location);
  Globals::Value32 hash;
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  hash.uValue = Globals::hash(path.toLocal8Bit().data(), path.length() + 1);
  bytes->append(hash.bytes[0]);
  bytes->append(hash.bytes[1]);
  bytes->append(hash.bytes[2]);
  bytes->append(hash.bytes[3]);
}

bool MapEvent::ShowImage::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_ShowImage *configure = new ConfigureEvent_ShowImage(parentWidget);
  configure->setLocation(location);
  if (configure->exec())
  {
    location = configure->getLocation();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::ShowImage::getItem()
{
  item->setText(0, QString("<> showImage: \"%1\"").arg(location));
  return item;
}
//END: ShowImage

//BEGIN: MovePlayer (COMPLETE)
MapEvent::MovePlayer::MovePlayer(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::MovePlayer)
{
  movement = 0;
  retainFacing = false;
}

MapEvent::MovePlayer::MovePlayer(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::MovePlayer)
{
  movement = atoi(eventNode.getAttribute("movement"));
  retainFacing = strcmp(eventNode.getAttribute("retainFacing"), "true") == 0;
}

MapEvent::MovePlayer::~MovePlayer()
{
}

XMLNode MapEvent::MovePlayer::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("movePlayer");
  eventNode.addAttribute("movement", QString::number(movement).toLocal8Bit().data());
  eventNode.addAttribute("retainFacing", (retainFacing) ? "true":"false");
  return eventNode;
}

void MapEvent::MovePlayer::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  bytes->append(static_cast<char>(type));
  if (retainFacing)
    bytes->append(movement | 128);
  else
    bytes->append(movement);
}

bool MapEvent::MovePlayer::configureEvent(QWidget *parentWidget)
{
  ConfigureEvent_MovePlayer *configure = new ConfigureEvent_MovePlayer(parentWidget);
  configure->setMovement(movement);
  configure->setRetainFacing(retainFacing);
  if (configure->exec())
  {
    movement = configure->getMovement();
    retainFacing = configure->getRetainFacing();
    configure->deleteLater();
    return true;
  }
  configure->deleteLater();
  return false;
}

QTreeWidgetItem *MapEvent::MovePlayer::getItem()
{
  item->setText(0, QString("<> movePlayer: %1%2").arg(movements[movement]).arg((retainFacing) ? " (retainFacing)":""));
  return item;
}
//END: MovePlayer

//BEGIN: Label (COMPLETE)
MapEvent::Label::Label(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::Label)
{
  name = "";
}

MapEvent::Label::Label(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::Label)
{
  name = eventNode.getAttribute("name");
}

MapEvent::Label::~Label()
{
}

XMLNode MapEvent::Label::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("label");
  eventNode.addAttribute("name", name.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::Label::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
}

bool MapEvent::Label::configureEvent(QWidget *parentWidget)
{
  QString input;
  bool ok;
  input = QInputDialog::getText(parentWidget, "Label", "Name:", QLineEdit::Normal, name, &ok);
  if (ok && !input.isEmpty())
  {
    for (auto event : pMapEvent->allEvents)
    {
      if (event->getType() == Type::Jump)
        event->getItem();
    }
    name = input;
  }
  getItem();
  return ok;
}

QTreeWidgetItem *MapEvent::Label::getItem()
{
  item->setText(0, QString("<> label: \"%2\"").arg(name));
  return item;
}
//END: Label

//BEGIN: Comment (COMPLETE)
MapEvent::Comment::Comment(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::Comment)
{
  comment = "";
}

MapEvent::Comment::Comment(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::Comment)
{
  comment = QString(eventNode.getAttribute("value")).replace("\\n", "\n").replace("\\\\", "\\");
}

MapEvent::Comment::~Comment()
{
}

XMLNode MapEvent::Comment::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("comment");
  QString adjustedComment = comment;
  adjustedComment.replace("\\", "\\\\");
  adjustedComment.replace("\n", "\\n");
  eventNode.addAttribute("value", adjustedComment.toLocal8Bit().data());
  return eventNode;
}

void MapEvent::Comment::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
}

bool MapEvent::Comment::configureEvent(QWidget *parentWidget)
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

QTreeWidgetItem *MapEvent::Comment::getItem()
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
MapEvent::Else::Else(MapEvent *pBase, Event *pEvent) : Event(pBase, pEvent, Type::Else)
{
  events += Event::newEvent(pBase, this, Type::End);
}

MapEvent::Else::Else(MapEvent *pBase, Event *pEvent, XMLNode eventNode) : Event(pBase, pEvent, Type::Else)
{
  for (int i = 0; i < eventNode.nChildNode(); ++i)
    events += Event::newEvent(pBase, this, eventNode.getChildNode(i));
  events += Event::newEvent(pBase, this, Type::End);
}

MapEvent::Else::~Else()
{
  for (auto event : events)
    delete event;
  events.clear();
}

XMLNode MapEvent::Else::toXMLNode()
{
  XMLNode eventNode = XMLNode::createXMLTopNode("else");
  for (auto event : events)
  {
    if (event->getType() != Type::End)
      eventNode.addChild(event->toXMLNode());
  }
  return eventNode;
}

void MapEvent::Else::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
  for (auto event : events)
    event->compileEvent(bytes);
}

MapEvent::Event *MapEvent::Else::previousChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == 0)
    return nullptr;
  return events[index - 1];
}

MapEvent::Event *MapEvent::Else::nextChildEvent(Event *event)
{
  int index = events.indexOf(event);
  if (index == events.count() - 1)
    return nullptr;
  return events[index + 1];
}

QTreeWidgetItem *MapEvent::Else::getItem()
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
void MapEvent::End::compileEvent(QByteArray *bytes)
{
  offset = bytes->count();
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
    pMapEvent->jumps += jump;
    bytes->append(0xDE);
    bytes->append(0xAD);
    bytes->append(0xC0);
    bytes->append(0xDE);
  }
}
//END: End
