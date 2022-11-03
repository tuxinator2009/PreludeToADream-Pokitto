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

#ifndef CONFIGUREEVENT_CASTSPELL_H
#define CONFIGUREEVENT_CASTSPELL_H

#include <QDir>
#include "ui_configureevent_castspell.h"
#include "globals.h"

class ConfigureEvent_CastSpell : public QDialog, public Ui::ConfigureEvent_CastSpell
{
  Q_OBJECT
  public:
    ConfigureEvent_CastSpell(QWidget *parent=nullptr) : QDialog(parent)
    {
      QDir folder(QString("%1/animations").arg(Globals::datadir));
      QStringList animations = folder.entryList(QStringList() << "*.anim", QDir::Files, QDir::Name);
      animations.replaceInStrings(".anim", "", Qt::CaseInsensitive);
      setupUi(this);
      optAnimation->addItems(animations);
    }
    ~ConfigureEvent_CastSpell() {}
    void setAnimation(QString value) {optAnimation->setCurrentText(value);}
    QString getAnimation() {return optAnimation->currentText();}
    void setSpellType(int value) {optSpellType->setCurrentIndex(value);}
    int getSpellType() {return optSpellType->currentIndex();}
    void setLevel(int value) {numLevel->setValue(value);}
    int getLevel() {return numLevel->value();}
    void setMP(int value) {numMP->setValue(value);}
    int getMP() {return numMP->value();}
};

#endif //CONFIGUREEVENT_CASTSPELL_H
