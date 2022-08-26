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

#ifndef CONFIGUREEVENT_CHANGEEVENTFLAGS_H
#define CONFIGUREEVENT_CHANGEEVENTFLAGS_H

#include "ui_configureevent_changeeventflags.h"

class ConfigureEvent_ChangeEventFlags : public QDialog, public Ui::ConfigureEvent_ChangeEventFlags
{
  Q_OBJECT
  public:
    static constexpr uint8_t FLAGS_ACTIVATE_BUTTON = 1;
    static constexpr uint8_t FLAGS_DOOR = 2;
    static constexpr uint8_t FLAGS_EXIT = 4;
    static constexpr uint8_t FLAGS_OFFSET = 8;
    static constexpr uint8_t FLAGS_NULL = 255;
    ConfigureEvent_ChangeEventFlags(QWidget *parent=nullptr) : QDialog(parent) {setupUi(this);}
    void addEvent(QString value) {optEvent->addItem(value);}
    void setEvent(QString value) {optEvent->setCurrentText(value);}
    QString getEvent() {return optEvent->currentText();}
    void setOperation(int value) {optOperation->setCurrentIndex(value);}
    int getOperation() {return optOperation->currentIndex();}
    void setFlags(uint8_t value)
    {
      chkButton->setChecked(value & FLAGS_ACTIVATE_BUTTON);
      chkDoor->setChecked(value & FLAGS_DOOR);
      chkExit->setChecked(value & FLAGS_EXIT);
      chkOffset->setChecked(value & FLAGS_OFFSET);
    }
    uint8_t getFlags()
    {
      uint8_t flags = 0;
      if (chkButton->isChecked())
        flags |= FLAGS_ACTIVATE_BUTTON;
      if (chkDoor->isChecked())
        flags |= FLAGS_DOOR;
      if (chkExit->isChecked())
        flags |= FLAGS_EXIT;
      if (chkOffset->isChecked())
        flags |= FLAGS_OFFSET;
      return flags;
    }
};

#endif //CONFIGUREEVENT_CHANGEEVENTFLAGS_H
