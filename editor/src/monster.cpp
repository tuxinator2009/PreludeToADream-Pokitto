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

#include <QByteArray>
#include <QString>
#include "battleevent.h"
#include "globals.h"
#include "monster.h"
#include "xmlParser.h"

Monster::Monster()
{
  name = "NO NAME";
  experience = 0;
  hp = 0;
  attack = 0;
  defense = 0;
  agility = 0;
  magic = 0;
  spellResistance = 43690;
  poisonResistance = 0;
  slowResistance = 0;
  mp = 0;
  gold = 0;
  introEvent = new BattleEvent();
  outroEvent = new BattleEvent();
  turnEvent = new BattleEvent();
}

Monster::Monster(XMLNode monsterNode)
{
  name = monsterNode.getAttribute("name");
  experience = QString(monsterNode.getAttribute("experience")).toInt();
  hp = QString(monsterNode.getAttribute("hp")).toInt();
  attack = QString(monsterNode.getAttribute("attack")).toInt();
  defense = QString(monsterNode.getAttribute("defense")).toInt();
  agility = QString(monsterNode.getAttribute("agility")).toInt();
  magic = QString(monsterNode.getAttribute("magic")).toInt();
  spellResistance = QString(monsterNode.getAttribute("spellResistance")).toInt();
  poisonResistance = QString(monsterNode.getAttribute("poisonResistance")).toInt();
  slowResistance = QString(monsterNode.getAttribute("slowResistance")).toInt();
  mp = QString(monsterNode.getAttribute("mp")).toInt();
  gold = QString(monsterNode.getAttribute("gold")).toInt();
  introEvent = new BattleEvent(monsterNode.getChildNode("intro"));
  outroEvent = new BattleEvent(monsterNode.getChildNode("outro"));
  turnEvent = new BattleEvent(monsterNode.getChildNode("turn"));
}

Monster::~Monster()
{
  delete introEvent;
  delete outroEvent;
  delete turnEvent;
}

XMLNode Monster::toXMLNode()
{
  XMLNode monsterNode = XMLNode::createXMLTopNode("monster");
  monsterNode.addAttribute("name", name.toLocal8Bit().data());
  monsterNode.addAttribute("experience", QString::number(experience).toLocal8Bit().data());
  monsterNode.addAttribute("hp", QString::number(hp).toLocal8Bit().data());
  monsterNode.addAttribute("attack", QString::number(attack).toLocal8Bit().data());
  monsterNode.addAttribute("defense", QString::number(defense).toLocal8Bit().data());
  monsterNode.addAttribute("agility", QString::number(agility).toLocal8Bit().data());
  monsterNode.addAttribute("magic", QString::number(magic).toLocal8Bit().data());
  monsterNode.addAttribute("spellResistance", QString::number(spellResistance).toLocal8Bit().data());
  monsterNode.addAttribute("poisonResistance", QString::number(poisonResistance).toLocal8Bit().data());
  monsterNode.addAttribute("slowResistance", QString::number(slowResistance).toLocal8Bit().data());
  monsterNode.addAttribute("mp", QString::number(mp).toLocal8Bit().data());
  monsterNode.addAttribute("gold", QString::number(gold).toLocal8Bit().data());
  monsterNode.addChild(introEvent->toXMLNode("intro"));
  monsterNode.addChild(outroEvent->toXMLNode("outro"));
  monsterNode.addChild(turnEvent->toXMLNode("turn"));
  return monsterNode;
}

QByteArray Monster::compile()
{
  QByteArray bytes;
  QByteArray eventData;
  Globals::Value16 value16;
  value16.uValue = experience;
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  value16.uValue = hp;
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  value16.uValue = attack;
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  value16.uValue = defense;
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  value16.uValue = agility;
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  value16.uValue = magic;
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  bytes.append(4, 0);
  value16.uValue = spellResistance;
  bytes += value16.bytes[0];
  bytes += value16.bytes[1];
  bytes += poisonResistance;
  bytes += slowResistance;
  bytes += mp;
  bytes += gold;
  introEvent->compileEvent(&eventData);
  value16.uValue = eventData.size();
  bytes[14] = value16.bytes[0];
  bytes[15] = value16.bytes[1];
  outroEvent->compileEvent(&eventData);
  value16.uValue = eventData.size();
  bytes[16] = value16.bytes[0];
  bytes[17] = value16.bytes[1];
  turnEvent->compileEvent(&eventData);
  bytes += eventData;
  return bytes;
}
