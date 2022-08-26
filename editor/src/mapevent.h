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

#ifndef MAPEVENT_H
#define MAPEVENT_H

#include <QList>
#include <QTreeWidgetItem>
#include "globals.h"
#include "xmlParser.h"

class Map;

class MapEvent
{
  public:
    static constexpr uint8_t FLAGS_ACTIVATE_BUTTON = 1;
    static constexpr uint8_t FLAGS_DOOR = 2;
    static constexpr uint8_t FLAGS_EXIT = 4;
    static constexpr uint8_t FLAGS_OFFSET = 8;
    static constexpr uint8_t FLAGS_NULL = 255;
    class Event
    {
      public:
        enum class Type
        {
          HideScreen,
          ShowScreen,
          Teleport,
          ShowDialog,
          HideDialog,
          BufferMessage,
          BufferValue,
          BufferCharacter,
          ShowMessage,
          ShowOneLiner,
          ShowShopMenu,
          ShowGold,
          HideGold,
          WaitButtons,
          WaitFrames,
          Battle,
          ShakeScreen,
          Cutscene,
          Jump,
          IfSwitch,
          IfVariable,
          IfHasItem,
          IfExited,
          IfYesNo,
          IfValue,
          IfFacing,
          PlaySoundEffect,
          PlayMusic,
          PauseMusic,
          ResumeMusic,
          WaitMusic,
          WaitSoundEffect,
          TurnSwitchOn,
          TurnSwitchOff,
          ToggleSwitch,
          TurnSwitchRangeOn,
          TurnSwitchRangeOff,
          ToggleSwitchRange,
          ChangeVariable,
          ChangeSprite,
          ChangeEventLocation,
          ChangeEventFlags,
          ChangePassability,
          GivePlayerItem,
          GivePlayerGold,
          Heal,
          ShowImage,
          HideImage,
          MovePlayer,
          EndEventProcessing,
          Label,
          Comment,
          Else,
          End,
          TopLevelEvent,
          IfEvent,
          NUM_EVENT_TYPES
        };
        Event(MapEvent *pBase, Event *p, Type t);
        virtual ~Event();
        Type getType() {return type;}
        const char *getTypeString() {return typeString[static_cast<int>(type)];}
        uint32_t getOffset() {return offset;}
        virtual XMLNode toXMLNode() {return XMLNode::createXMLTopNode(getTypeString());}
        virtual void compileEvent(QByteArray *bytes) {offset = bytes->count();bytes->append(static_cast<char>(type));}
        virtual bool configureEvent(QWidget *parentWidget) {Q_UNUSED(parentWidget);return true;}
        virtual void addChildEvent(Event *event, int location) {Q_UNUSED(event);Q_UNUSED(location);}
        virtual void deleteChildEvent(int location) {Q_UNUSED(location);}
        virtual Event *getChildEvent(int location) {Q_UNUSED(location);return nullptr;}
        virtual Event *previousChildEvent(Event *event) {Q_UNUSED(event);return nullptr;}
        virtual Event *nextChildEvent(Event *event) {Q_UNUSED(event);return nullptr;}
        virtual int indexOfChildEvent(Event *event) {Q_UNUSED(event);return 0;}
        virtual int numChildEvents() {return 0;}
        virtual QTreeWidgetItem *getItem() {item->setText(0, QString("<> %1").arg(getTypeString()));return item;}
        virtual bool isIf() {return false;}
        virtual bool isElse() {return false;}
        Event *getParentEvent() {return parent;}
        static Event *newEvent(MapEvent *pBase, Event *parent, Type t);
        static Event *newEvent(MapEvent *pBase, Event *parent, XMLNode eventNode);
        //BEGIN Constants
        static constexpr uint8_t DIR_UP = 0;
        static constexpr uint8_t DIR_RIGHT = 1;
        static constexpr uint8_t DIR_DOWN = 2;
        static constexpr uint8_t DIR_LEFT = 3;
        static constexpr uint8_t DIR_RETAIN = 4;
        static constexpr uint8_t VALUE_PLAYER_NAME = 0;
        static constexpr uint8_t VALUE_PLAYER_HP = 1;
        static constexpr uint8_t VALUE_PLAYER_MAXHP = 2;
        static constexpr uint8_t VALUE_PLAYER_MP = 3;
        static constexpr uint8_t VALUE_PLAYER_MAXMP = 4;
        static constexpr uint8_t VALUE_PLAYER_EXPERIENCE = 5;
        static constexpr uint8_t VALUE_PLAYER_NEXTLEVEL = 6;
        static constexpr uint8_t VALUE_PLAYER_EXPERIENCELEFT = 7;
        static constexpr uint8_t VALUE_PLAYER_GOLD = 8;
        static constexpr uint8_t VALUE_PLAYER_LEVEL = 9;
        static constexpr uint8_t VALUE_PLAYER_ATTACK = 10;
        static constexpr uint8_t VALUE_PLAYER_DEFENSE = 11;
        static constexpr uint8_t VALUE_PLAYER_AGILITY = 12;
        static constexpr uint8_t VALUE_PLAYER_MAGIC = 13;
        static constexpr uint8_t VALUE_VARIABLE = 14;
        static constexpr uint8_t VALUE_SUPPLIED = 15;
        static constexpr uint8_t CONDITION_EQUAL_TO = 0;
        static constexpr uint8_t CONDITION_NOT_EQUAL_TO = 1;
        static constexpr uint8_t CONDITION_GREATER_THAN = 2;
        static constexpr uint8_t CONDITION_GREATER_THAN_OR_EQUAL_TO = 3;
        static constexpr uint8_t CONDITION_LESS_THAN = 4;
        static constexpr uint8_t CONDITION_LESS_THAN_OR_EQUAL_TO = 5;
        static constexpr uint8_t OPERATION_SET = 0;
        static constexpr uint8_t OPERATION_ADD = 1;
        static constexpr uint8_t OPERATION_SUB = 2;
        static constexpr uint8_t OPERATION_MUL = 3;
        static constexpr uint8_t OPERATION_DIV = 4;
        static constexpr uint8_t OPERATION_MOD = 5;
        static constexpr uint8_t HIDE_INSTANT = 0;
        static constexpr uint8_t HIDE_FADEOUT = 1;
        static constexpr uint8_t HIDE_WIPE_DOWN = 2;
        static constexpr uint8_t HIDE_WIPE_UP = 3;
        static constexpr uint8_t HIDE_WIPE_IN = 4;
        static constexpr uint8_t HIDE_MASK_OUT = 5;
        static constexpr uint8_t HIDE_WHITE_OUT = 6;
        static constexpr uint8_t SHOW_INSTANT = 0;
        static constexpr uint8_t SHOW_FADEIN = 1;
        static constexpr uint8_t SHOW_UNWIPE_DOWN = 2;
        static constexpr uint8_t SHOW_UNWIPE_UP = 3;
        static constexpr uint8_t SHOW_UNWIPE_OUT = 4;
        static constexpr uint8_t SHOW_WHITE_IN = 5;
        static constexpr uint8_t PASSABILITY_SET = 0;
        static constexpr uint8_t PASSABILITY_CLEAR = 1;
        static constexpr uint8_t PASSABILITY_TOGGLE = 2;
        static constexpr uint8_t MOVE_UP = 0;
        static constexpr uint8_t MOVE_RIGHT = 1;
        static constexpr uint8_t MOVE_DOWN = 2;
        static constexpr uint8_t MOVE_LEFT = 3;
        static constexpr uint8_t MOVE_FORWARD = 4;
        static constexpr uint8_t MOVE_STRAFE_RIGHT = 5;
        static constexpr uint8_t MOVE_BACKWARD = 6;
        static constexpr uint8_t MOVE_STRAFE_LEFT = 7;
        static constexpr uint8_t MOVE_FACE_UP = 8;
        static constexpr uint8_t MOVE_FACE_RIGHT = 9;
        static constexpr uint8_t MOVE_FACE_DOWN = 10;
        static constexpr uint8_t MOVE_FACE_LEFT = 11;
        static constexpr uint8_t MOVE_TURN_LEFT = 12;
        static constexpr uint8_t MOVE_TURN_RIGHT = 13;
        static constexpr uint8_t MOVE_TURN_AROUND = 14;
        static constexpr uint8_t FLAG_SET = 0;
        static constexpr uint8_t FLAG_CLEAR = 1;
        static constexpr uint8_t FLAG_TOGGLE = 2;
        //END: Constants
      protected:
        static const char *typeString[];
        static const char *hideTransitions[];
        static const char *showTransitions[];
        static const char *facings[];
        static const char *bufferValues[];
        static const char *operations[];
        static const char *movements[];
        static const char *conditions[];
        static const uint8_t typeID[];
        QTreeWidgetItem *item;
        MapEvent *pMapEvent;
        Event *parent;
        Type type;
        uint32_t offset;
    };
    class TopLevelEvent: public Event
    {
      public:
        TopLevelEvent(MapEvent *pBase, Event *parent) : Event(pBase, parent, Type::TopLevelEvent) {events += Event::newEvent(pBase, this, Type::End);}
        TopLevelEvent(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~TopLevelEvent();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        void addChildEvent(Event *event, int location) {events.insert(location, event);item->insertChild(location, event->getItem());}
        void deleteChildEvent(int location) {delete events.takeAt(location);}
        Event *getChildEvent(int location) {return events[location];}
        Event *previousChildEvent(Event *event);
        Event *nextChildEvent(Event *event);
        int indexOfChildEvent(Event *event) {return events.indexOf(event);}
        int numChildEvents() {return events.count();}
        void setItem(QTreeWidgetItem *value);
      private:
        QList<Event*> events;
    };
    class IfEvent: public Event
    {
      public:
        IfEvent(MapEvent *pBase, Event *parent, Type t) : Event(pBase, parent, t) {events += Event::newEvent(pBase, this, Type::End); elseIf = false;}
        IfEvent(MapEvent *pBase, Event *parent, Type t, XMLNode eventNode);
        virtual ~IfEvent();
        void addChildEvent(Event *event, int location) {events.insert(location, event);item->insertChild(location, event->getItem());}
        void deleteChildEvent(int location) {delete events.takeAt(location);}
        Event *getChildEvent(int location) {return events[location];}
        Event *previousChildEvent(Event *event);
        Event *nextChildEvent(Event *event);
        int indexOfChildEvent(Event *event) {return events.indexOf(event);}
        int numChildEvents() {return events.count();}
        bool isIf() {return true;}
        bool isElse() {return elseIf;}
        void addItems(QTreeWidgetItem *item);
      protected:
        QList<Event*> events;
        bool elseIf;
    };
    class HideScreen: public Event
    {
      public:
        HideScreen(MapEvent *pBase, Event *parent);
        HideScreen(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~HideScreen();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t transition;
        uint8_t speed;
    };
    class ShowScreen: public Event
    {
      public:
        ShowScreen(MapEvent *pBase, Event *parent);
        ShowScreen(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShowScreen();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t transition;
        uint8_t speed;
    };
    class Teleport: public Event
    {
      public:
        Teleport(MapEvent *pBase, Event *parent);
        Teleport(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Teleport();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        int mapID;
        uint8_t x;
        uint8_t y;
        uint8_t facing;
    };
    class ShowDialog: public Event
    {
      public:
        ShowDialog(MapEvent *pBase, Event *parent);
        ShowDialog(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShowDialog();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString name;
    };
    class BufferMessage: public Event
    {
      public:
        BufferMessage(MapEvent *pBase, Event *parent);
        BufferMessage(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~BufferMessage();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString message;
    };
    class BufferValue: public Event
    {
      public:
        BufferValue(MapEvent *pBase, Event *parent);
        BufferValue(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~BufferValue();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t valueType;
        int32_t value;
    };
    class BufferCharacter: public Event
    {
      public:
        BufferCharacter(MapEvent *pBase, Event *parent);
        BufferCharacter(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~BufferCharacter();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        char character;
    };
    class ShowMessage: public Event
    {
      public:
        ShowMessage(MapEvent *pBase, Event *parent);
        ShowMessage(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShowMessage();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        void bufferMessage(QByteArray *bytes, QString line);
        QStringList message;
    };
    class ShowOneLiner: public Event
    {
      public:
        ShowOneLiner(MapEvent *pBase, Event *parent);
        ShowOneLiner(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShowOneLiner();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString message;
    };
    class ShowShopMenu: public Event
    {
      public:
        ShowShopMenu(MapEvent *pBase, Event *parent);
        ShowShopMenu(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShowShopMenu();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString toString(uint16_t items, const char **names);
        QString greeting;
        uint16_t consumables;
        uint16_t weapons;
        uint16_t helmets;
        uint16_t armor;
        uint16_t boots;
        uint16_t rings;
        uint16_t amulets;
    };
    class WaitFrames: public Event
    {
      public:
        WaitFrames(MapEvent *pBase, Event *parent);
        WaitFrames(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~WaitFrames();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t numFrames;
    };
    class Battle: public Event
    {
      public:
        Battle(MapEvent *pBase, Event *parent);
        Battle(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Battle();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString background;
        QString music;
        uint8_t monster;
    };
    class ShakeScreen: public Event
    {
      public:
        ShakeScreen(MapEvent *pBase, Event *parent);
        ShakeScreen(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShakeScreen();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t duration;
        uint8_t magnitude;
    };
    class Cutscene: public Event
    {
      public:
        Cutscene(MapEvent *pBase, Event *parent);
        Cutscene(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Cutscene();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString location;
    };
    class Jump: public Event
    {
      public:
        Jump(MapEvent *pBase, Event *parent);
        Jump(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Jump();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
        QString getLabel() {return label;}
        void setLabel(QString value) {label = value;}
      private:
        QString label;
    };
    class IfSwitch: public IfEvent
    {
      public:
        IfSwitch(MapEvent *pBase, Event *parent);
        IfSwitch(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfSwitch();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t switchID;
        bool isOn;
    };
    class IfVariable: public IfEvent
    {
      public:
        IfVariable(MapEvent *pBase, Event *parent);
        IfVariable(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfVariable();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        int32_t value;
        uint8_t variableID;
        uint8_t condition;
    };
    class IfHasItem: public IfEvent
    {
      public:
        IfHasItem(MapEvent *pBase, Event *parent);
        IfHasItem(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfHasItem();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t itemType;
        uint8_t itemID;
    };
    class IfExited: public IfEvent
    {
      public:
        IfExited(MapEvent *pBase, Event *parent);
        IfExited(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfExited();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t facing;
    };
    class IfYesNo: public IfEvent
    {
      public:
        IfYesNo(MapEvent *pBase, Event *parent);
        IfYesNo(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfYesNo();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        bool yes;
    };
    class IfValue: public IfEvent
    {
      public:
        IfValue(MapEvent *pBase, Event *parent);
        IfValue(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfValue();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        int32_t desiredValue;
        uint8_t valueToCheck;
        uint8_t condition;
    };
    class IfFacing: public IfEvent
    {
      public:
        IfFacing(MapEvent *pBase, Event *parent);
        IfFacing(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfFacing();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t direction;
    };
    class PlaySoundEffect: public Event
    {
      public:
        PlaySoundEffect(MapEvent *pBase, Event *parent);
        PlaySoundEffect(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~PlaySoundEffect();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString sfx;
        bool loop;
    };
    class PlayMusic: public Event
    {
      public:
        PlayMusic(MapEvent *pBase, Event *parent);
        PlayMusic(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~PlayMusic();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString bgm;
        bool loop;
    };
    class TurnSwitchOn: public Event
    {
      public:
        TurnSwitchOn(MapEvent *pBase, Event *parent);
        TurnSwitchOn(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~TurnSwitchOn();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t switchID;
    };
    class TurnSwitchOff: public Event
    {
      public:
        TurnSwitchOff(MapEvent *pBase, Event *parent);
        TurnSwitchOff(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~TurnSwitchOff();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t switchID;
    };
    class ToggleSwitch: public Event
    {
      public:
        ToggleSwitch(MapEvent *pBase, Event *parent);
        ToggleSwitch(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ToggleSwitch();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t switchID;
    };
    class TurnSwitchRangeOn: public Event
    {
      public:
        TurnSwitchRangeOn(MapEvent *pBase, Event *parent);
        TurnSwitchRangeOn(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~TurnSwitchRangeOn();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t startSwitchID;
        uint8_t endSwitchID;
    };
    class TurnSwitchRangeOff: public Event
    {
      public:
        TurnSwitchRangeOff(MapEvent *pBase, Event *parent);
        TurnSwitchRangeOff(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~TurnSwitchRangeOff();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t startSwitchID;
        uint8_t endSwitchID;
    };
    class ToggleSwitchRange: public Event
    {
      public:
        ToggleSwitchRange(MapEvent *pBase, Event *parent);
        ToggleSwitchRange(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ToggleSwitchRange();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t startSwitchID;
        uint8_t endSwitchID;
    };
    class ChangeVariable: public Event
    {
      public:
        ChangeVariable(MapEvent *pBase, Event *parent);
        ChangeVariable(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ChangeVariable();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        int32_t value;
        uint8_t variableID;
        uint8_t operation;
    };
    class ChangeSprite: public Event
    {
      public:
        ChangeSprite(MapEvent *pBase, Event *parent);
        ChangeSprite(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ChangeSprite();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString event;
        uint8_t spriteID;
    };
    class ChangeEventLocation: public Event
    {
      public:
        ChangeEventLocation(MapEvent *pBase, Event *parent);
        ChangeEventLocation(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ChangeEventLocation();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString event;
        uint8_t x;
        uint8_t y;
        bool offset;
    };
    class ChangeEventFlags: public Event
    {
      public:
        ChangeEventFlags(MapEvent *pBase, Event *parent);
        ChangeEventFlags(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ChangeEventFlags();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString event;
        uint8_t flags;
        uint8_t operation;
    };
    class ChangePassability: public Event
    {
      public:
        ChangePassability(MapEvent *pBase, Event *parent);
        ChangePassability(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ChangePassability();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t x;
        uint8_t y;
        uint8_t operation;
    };
    class GivePlayerItem: public Event
    {
      public:
        GivePlayerItem(MapEvent *pBase, Event *parent);
        GivePlayerItem(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~GivePlayerItem();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t itemType;
        uint8_t itemID;
        uint8_t amount;
    };
    class GivePlayerGold: public Event
    {
      public:
        GivePlayerGold(MapEvent *pBase, Event *parent);
        GivePlayerGold(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~GivePlayerGold();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        int16_t amount;
    };
    class ShowImage: public Event
    {
      public:
        ShowImage(MapEvent *pBase, Event *parent);
        ShowImage(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShowImage();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString location;
    };
    class MovePlayer: public Event
    {
      public:
        MovePlayer(MapEvent *pBase, Event *parent);
        MovePlayer(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~MovePlayer();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t movement;
        bool retainFacing;
    };
    class Label: public Event
    {
      public:
        Label(MapEvent *pBase, Event *parent);
        Label(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Label();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
        QString getName() {return name;}
      private:
        QString name;
    };
    class Comment: public Event
    {
      public:
        Comment(MapEvent *pBase, Event *parent);
        Comment(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Comment();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString comment;
    };
    class Else: public Event
    {
      public:
        Else(MapEvent *pBase, Event *parent);
        Else(MapEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Else();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        void addChildEvent(Event *event, int location) {events.insert(location, event);item->insertChild(location, event->getItem());}
        void deleteChildEvent(int location) {delete events.takeAt(location);}
        Event *getChildEvent(int location) {return events[location];}
        Event *previousChildEvent(Event *event);
        Event *nextChildEvent(Event *event);
        int indexOfChildEvent(Event *event) {return events.indexOf(event);}
        int numChildEvents() {return events.count();}
        QTreeWidgetItem *getItem();
        bool isIf() {return false;}
        bool isElse() {return true;}
      private:
        QList<Event*> events;
    };
    class End: public Event
    {
      public:
        End(MapEvent *pBase, Event *parent) : Event(pBase, parent, Type::End) {}
        virtual ~End() {}
        void compileEvent(QByteArray *bytes);
    };
    MapEvent(Map *m);
    MapEvent(Map *m, XMLNode topNode);
    ~MapEvent();
    XMLNode toXMLNode(bool onLoadEvent);
    void compileEvent(Globals::MapData *pMapData, int index, QByteArray *bytes);
    QString getName() {return name;}
    void setName(QString value) {name = value;}
    uint8_t getX() {return x;}
    void setX(uint8_t value) {x = value;}
    uint8_t getY() {return y;}
    void setY(uint8_t value) {y = value;}
    uint8_t getFlags() {return flags;}
    void setFlags(uint8_t value) {flags = value;}
    void setFlag(uint8_t flag, bool on) {flags &= ~flag;if (on) flags |= flag;}
    bool isFlagSet(uint8_t flag) {return (flags & flag) != 0;}
    uint8_t getSpriteID() {return spriteID;}
    void setSpriteID(uint8_t value) {spriteID = value;}
    TopLevelEvent *getTopLevelEvent() {return topLevelEvent;}
  protected:
    struct JumpLocation
    {
      Event *event;
      uint32_t pos;
    };
    QList<JumpLocation> jumps;
    QList<Event*> allEvents;
    Map *map;
  private:
    TopLevelEvent *topLevelEvent;
    QString name;
    uint8_t x;
    uint8_t y;
    uint8_t flags;
    uint8_t spriteID;
};

#endif //MAPEVENT_H
