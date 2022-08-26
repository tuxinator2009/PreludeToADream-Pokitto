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

#ifndef CONFIGUREEVENT_IFHASITEM_H
#define CONFIGUREEVENT_IFHASITEM_H

#include "ui_configureevent_ifhasitem.h"
#include "globals.h"

class ConfigureEvent_IfHasItem : public QDialog, public Ui::ConfigureEvent_IfHasItem
{
  Q_OBJECT
  public:
    ConfigureEvent_IfHasItem(QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      for (int type = 0; type < 8; ++type)
      {
        for (int item = 0; item < 16; ++item)
          tblItems->item(item, type)->setText(Globals::items[type][item]);
      }
    }
    ~ConfigureEvent_IfHasItem() {}
    void enableElseIf() {chkElseIf->setEnabled(true);}
    void setElseIf(bool value) {(value) ? chkElseIf->setChecked(true):chkIf->setChecked(true);}
    bool isElseIf() {return chkElseIf->isChecked();}
    void setItem(int type, int item)
    {
      tblItems->setCurrentCell(item, type);
      tblItems->item(item, type)->setSelected(true);
    }
    int getType() {return tblItems->currentColumn();}
    int getItem() {return tblItems->currentRow();}
};

#endif //CONFIGUREEVENT_IFHASITEM_H
