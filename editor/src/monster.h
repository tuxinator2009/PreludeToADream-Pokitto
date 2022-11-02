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

#ifndef MONSTER_H
#define MONSTER_H

#include <QByteArray>
#include <QString>
#include "battleevent.h"
#include "xmlParser.h"

class Monster
{
  public:
    Monster();
    Monster(XMLNode monsterNode);
    ~Monster();
    XMLNode toXMLNode();
    QByteArray compile();
    BattleEvent *getIntroEvent() {return introEvent;}
    BattleEvent *getOutroEvent() {return outroEvent;}
    BattleEvent *getTurnEvent() {return turnEvent;}
    QString getName() {return name;}
    void setName(QString value) {name = value;}
    uint16_t getExperience() {return experience;}
    void setExperience(uint16_t value) {experience = value;}
    uint16_t getHP() {return hp;}
    void setHP(uint16_t value) {hp = value;}
    uint16_t getAttack() {return attack;}
    void setAttack(uint16_t value) {attack = value;}
    uint16_t getDefense() {return defense;}
    void setDefense(uint16_t value) {defense = value;}
    uint16_t getAgility() {return agility;}
    void setAgility(uint16_t value) {agility = value;}
    uint16_t getMagic() {return magic;}
    void setMagic(uint16_t value) {magic = value;}
    uint16_t getSpellResistance() {return spellResistance;}
    void setSpellResistance(uint16_t value) {spellResistance = value;}
    uint8_t getPoisonResistance() {return poisonResistance;}
    void setPoisonResistance(uint8_t value) {poisonResistance = value;}
    uint8_t getSlowResistance() {return slowResistance;}
    void setSlowResistance(uint8_t value) {slowResistance = value;}
    uint8_t getMP() {return mp;}
    void setMP(uint8_t value) {mp = value;}
    uint8_t getGold() {return gold;}
    void setGold(uint8_t value) {gold = value;}
  private:
    BattleEvent *introEvent;
    BattleEvent *outroEvent;
    BattleEvent *turnEvent;
    QString name;
    uint16_t experience;
    uint16_t hp;
    uint16_t attack;
    uint16_t defense;
    uint16_t agility;
    uint16_t magic;
    uint16_t spellResistance;
    uint8_t poisonResistance;
    uint8_t slowResistance;
    uint8_t mp;
    uint8_t gold;
};

#endif //MONSTER_H
