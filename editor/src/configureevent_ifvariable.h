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

#ifndef CONFIGUREEVENT_IFVARIABLE_H
#define CONFIGUREEVENT_IFVARIABLE_H

#include "ui_configureevent_ifvariable.h"

class ConfigureEvent_IfVariable : public QDialog, public Ui::ConfigureEvent_IfVariable
{
  Q_OBJECT
  public:
    ConfigureEvent_IfVariable(QWidget *parent=nullptr) : QDialog(parent) {setupUi(this);}
    ~ConfigureEvent_IfVariable() {}
    void enableElseIf() {chkElseIf->setEnabled(true);}
    void setElseIf(bool value) {(value) ? chkElseIf->setChecked(true):chkIf->setChecked(true);}
    bool isElseIf() {return chkElseIf->isChecked();}
    void setVariableID(int value) {numVariableID->setValue(value);}
    int getVariableID() {return numVariableID->value();}
    void setCondition(int value) {optCondition->setCurrentIndex(value);}
    int getCondition() {return optCondition->currentIndex();}
    void setValue(int value) {numValue->setValue(value);}
    int getValue() {return numValue->value();}
};

#endif //CONFIGUREEVENT_IFVARIABLE_H
