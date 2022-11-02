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

#ifndef BATTLEEVENT_H
#define BATTLEEVENT_H

#include <QList>
#include <QTreeWidgetItem>
#include "globals.h"
#include "xmlParser.h"

class BattleEvent
{
  public:
    class Event
    {
      public:
        enum class Type
        {
          ShakeScreen,
          FlashBattler,
          FlashUi,
          BasicAttack,
          UseSkill,
          CastSpell,
          PlaySoundEffect,
          BufferMessage,
          BufferValue,
          BufferCharacter,
          ShowMessage,
          Jump,
          IfCounter,
          IfStatus,
          IfStat,
          ChangeBattlerSprite,
          ChangeBackgroundImage,
          PlayBattleAnimation,
          WaitFrames,
          WaitButtons,
          InflictStatus,
          ConsumeMP,
          Random,
          EndEventProcessing,
          Label,
          Comment,
          Else,
          End,
          TopLevelEvent,
          IfEvent,
          NUM_EVENT_TYPES
        };
        Event(BattleEvent *pBase, Event *p, Type t);
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
        static Event *newEvent(BattleEvent *pBase, Event *parent, Type t);
        static Event *newEvent(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        //BEGIN Constants
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
        static constexpr uint8_t VALUE_VARIABLE = 25;
        static constexpr uint8_t VALUE_SUPPLIED = 26;
        static constexpr uint8_t CONDITION_EQUAL_TO = 0;
        static constexpr uint8_t CONDITION_NOT_EQUAL_TO = 1;
        static constexpr uint8_t CONDITION_GREATER_THAN = 2;
        static constexpr uint8_t CONDITION_GREATER_THAN_OR_EQUAL_TO = 3;
        static constexpr uint8_t CONDITION_LESS_THAN = 4;
        static constexpr uint8_t CONDITION_LESS_THAN_OR_EQUAL_TO = 5;
        //END: Constants
      protected:
        static const char *typeString[];
        static const char *bufferValues[];
        static const char *conditions[];
        static const char *statuses[];
        static const uint8_t typeID[];
        QTreeWidgetItem *item;
        BattleEvent *pBattleEvent;
        Event *parent;
        Type type;
        uint32_t offset;
    };
    class TopLevelEvent: public Event
    {
      public:
        TopLevelEvent(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::TopLevelEvent) {events += Event::newEvent(pBase, this, Type::End);}
        TopLevelEvent(BattleEvent *pBase, Event *parent, XMLNode eventNode);
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
        IfEvent(BattleEvent *pBase, Event *parent, Type t) : Event(pBase, parent, t) {events += Event::newEvent(pBase, this, Type::End); elseIf = false;}
        IfEvent(BattleEvent *pBase, Event *parent, Type t, XMLNode eventNode);
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
    class ShakeScreen : public Event
    {
      public:
        ShakeScreen(BattleEvent *pBase, Event *parent);
        ShakeScreen(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShakeScreen();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t duration;
        uint8_t rate;
    };
    class FlashBattler : public Event
    {
      public:
        FlashBattler(BattleEvent *pBase, Event *parent);
        FlashBattler(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~FlashBattler();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t duration;
        uint8_t color1;
        uint8_t color2;
        uint8_t color3;
    };
    class FlashUi : public Event
    {
      public:
        FlashUi(BattleEvent *pBase, Event *parent);
        FlashUi(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~FlashUi();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t duration;
        uint8_t color;
    };
    class BasicAttack : public Event
    {
      public:
        BasicAttack(BattleEvent *pBase, Event *parent);
        BasicAttack(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~BasicAttack();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString animation;
    };
    class UseSkill : public Event //TODO: possibly delete
    {
      public:
        UseSkill(BattleEvent *pBase, Event *parent);
        UseSkill(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~UseSkill();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
    };
    class CastSpell : public Event
    {
      public:
        CastSpell(BattleEvent *pBase, Event *parent);
        CastSpell(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~CastSpell();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString animation;
        uint8_t spellType;
        uint8_t level;
        uint8_t mp;
    };
    class PlaySoundEffect : public Event
    {
      public:
        PlaySoundEffect(BattleEvent *pBase, Event *parent);
        PlaySoundEffect(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~PlaySoundEffect();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString sfx;
        bool loop;
    };
    class BufferMessage : public Event
    {
      public:
        BufferMessage(BattleEvent *pBase, Event *parent);
        BufferMessage(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~BufferMessage();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString message;
    };
    class BufferValue : public Event
    {
      public:
        BufferValue(BattleEvent *pBase, Event *parent);
        BufferValue(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~BufferValue();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t valueType;
        int32_t value;
    };
    class BufferCharacter : public Event
    {
      public:
        BufferCharacter(BattleEvent *pBase, Event *parent);
        BufferCharacter(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~BufferCharacter();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        char character;
    };
    class ShowMessage : public Event
    {
      public:
        ShowMessage(BattleEvent *pBase, Event *parent);
        ShowMessage(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ShowMessage();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        void bufferMessage(QByteArray *bytes, QString line);
        QStringList message;
    };
    class Jump : public Event
    {
      public:
        Jump(BattleEvent *pBase, Event *parent);
        Jump(BattleEvent *pBase, Event *parent, XMLNode eventNode);
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
    class IfCounter : public IfEvent
    {
      public:
        IfCounter(BattleEvent *pBase, Event *parent);
        IfCounter(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfCounter();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t counter;
        uint8_t value;
        uint8_t condition;
    };
    class IfStatus : public IfEvent
    {
      public:
        IfStatus(BattleEvent *pBase, Event *parent);
        IfStatus(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfStatus();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t condition;
        uint8_t status;
        uint8_t level;
        bool self;
    };
    class IfStat : public IfEvent
    {
      public:
        IfStat(BattleEvent *pBase, Event *parent);
        IfStat(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~IfStat();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint16_t value;
        uint8_t condition;
        bool hp;
    };
    class ChangeBattlerSprite : public Event
    {
      public:
        ChangeBattlerSprite(BattleEvent *pBase, Event *parent);
        ChangeBattlerSprite(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ChangeBattlerSprite();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t spriteID;
    };
    class ChangeBackgroundImage : public Event
    {
      public:
        ChangeBackgroundImage(BattleEvent *pBase, Event *parent);
        ChangeBackgroundImage(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ChangeBackgroundImage();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString image;
    };
    class PlayBattleAnimation : public Event
    {
      public:
        PlayBattleAnimation(BattleEvent *pBase, Event *parent);
        PlayBattleAnimation(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~PlayBattleAnimation();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString animation;
    };
    class WaitFrames : public Event
    {
      public:
        WaitFrames(BattleEvent *pBase, Event *parent);
        WaitFrames(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~WaitFrames();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t numFrames;
    };
    class InflictStatus : public Event
    {
      public:
        InflictStatus(BattleEvent *pBase, Event *parent);
        InflictStatus(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~InflictStatus();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        QString successMessage;
        QString failMessage;
        uint8_t status;
        uint8_t level;
        uint8_t chance;
        bool self;
    };
    class ConsumeMP : public Event
    {
      public:
        ConsumeMP(BattleEvent *pBase, Event *parent);
        ConsumeMP(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~ConsumeMP();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t amount;
    };
    class Random : public Event
    {
      public:
        Random(BattleEvent *pBase, Event *parent);
        Random(BattleEvent *pBase, Event *parent, XMLNode eventNode);
        virtual ~Random();
        XMLNode toXMLNode();
        void compileEvent(QByteArray *bytes);
        bool configureEvent(QWidget *parentWidget);
        QTreeWidgetItem *getItem();
      private:
        uint8_t counter;
        uint8_t max;
    };
    class Label: public Event
    {
      public:
        Label(BattleEvent *pBase, Event *parent);
        Label(BattleEvent *pBase, Event *parent, XMLNode eventNode);
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
        Comment(BattleEvent *pBase, Event *parent);
        Comment(BattleEvent *pBase, Event *parent, XMLNode eventNode);
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
        Else(BattleEvent *pBase, Event *parent);
        Else(BattleEvent *pBase, Event *parent, XMLNode eventNode);
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
        End(BattleEvent *pBase, Event *parent) : Event(pBase, parent, Type::End) {}
        virtual ~End() {}
        void compileEvent(QByteArray *bytes);
    };
    BattleEvent() {topLevelEvent = new TopLevelEvent(this, nullptr);}
    BattleEvent(XMLNode topNode) {topLevelEvent = new TopLevelEvent(this, nullptr, topNode);}
    ~BattleEvent() {delete topLevelEvent;}
    XMLNode toXMLNode(const char *name);
    void compileEvent(QByteArray *bytes);
    TopLevelEvent *getTopLevelEvent() {return topLevelEvent;}
  protected:
    struct JumpLocation
    {
      Event *event;
      uint32_t pos;
    };
    QList<JumpLocation> jumps;
    QList<Event*> allEvents;
  private:
    TopLevelEvent *topLevelEvent;
};

#endif //BATTLEEVENT_H
