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

#ifndef CONFIGUREEVENT_BUFFERVALUE_H
#define CONFIGUREEVENT_BUFFERVALUE_H

#include "ui_configureevent_buffervalue.h"

class ConfigureEvent_BufferValue : public QDialog, public Ui::ConfigureEvent_BufferValue
{
  Q_OBJECT
  public:
    ConfigureEvent_BufferValue(bool isMapEvent=true, QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      if (!isMapEvent)
      {
        for (int i = 0; i < 14; ++i)
          optValue->setItemText(i, QString("Player %1").arg(optValue->itemText(i)));
        optValue->insertItem(14, "Monster Name");
        optValue->insertItem(15, "Monster HP");
        optValue->insertItem(16, "Monster MaxHP");
        optValue->insertItem(17, "Monster Mp");
        optValue->insertItem(19, "Monster Experience");
        optValue->insertItem(20, "Monster Gold");
        optValue->insertItem(21, "Monster Attack");
        optValue->insertItem(22, "Monster Defense");
        optValue->insertItem(23, "Monster Agility");
        optValue->insertItem(24, "Monster Magic");
      }
    }
    ~ConfigureEvent_BufferValue() {}
    int getValue() {return optValue->currentIndex();}
    void setValue(int value) {optValue->setCurrentIndex(value);}
    int getVariableID() {return numVariableID->value();}
    void setVariableID(int value) {numVariableID->setValue(value);}
    int getSuppliedValue() {return numSuppliedValue->value();}
    void setSuppliedValue(int value) {numSuppliedValue->setValue(value);}
  protected slots:
    void on_optValue_currentIndexChanged(QString text)
    {
      frmVariable->setEnabled(text == "Variable");
      frmSupplied->setEnabled(text == "Supplied");
    }
};

#endif //CONFIGUREEVENT_BUFFERVALUE_H
