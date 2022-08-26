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

#ifndef CONFIGUREEVENT_SHOWSHOPMENU_H
#define CONFIGUREEVENT_SHOWSHOPMENU_H

#include "ui_configureevent_showshopmenu.h"
#include "globals.h"

class ConfigureEvent_ShowShopMenu : public QDialog, public Ui::ConfigureEvent_ShowShopMenu
{
  Q_OBJECT
  public:
    ConfigureEvent_ShowShopMenu(QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      for (int i = 0; i < 8; ++i)
      {
        if (i == 1)
          continue;
        for (int j = 0; j < 16; ++j)
          tblItems->item(j, (i != 0) ? (i - 1):i)->setText(Globals::items[i][j]);
      }
    }
    ~ConfigureEvent_ShowShopMenu() {}
    void setGreeting(QString value) {txtGreeting->setText(value);}
    QString getGreeting() {return txtGreeting->text();}
    void setConsumables(uint16_t value)
    {
      for (int i = 0; i < 16; ++i)
        tblItems->item(i, 0)->setCheckState(((value >> i) & 1) ? Qt::Checked:Qt::Unchecked);
    }
    uint16_t getConsumables()
    {
      uint16_t value = 0;
      for (int i = 0; i < 16; ++i)
      {
        if (tblItems->item(i, 0)->checkState() == Qt::Checked)
          value |= 1 << i;
      }
      return value;
    }
    void setWeapons(uint16_t value)
    {
      for (int i = 0; i < 16; ++i)
        tblItems->item(i, 1)->setCheckState(((value >> i) & 1) ? Qt::Checked:Qt::Unchecked);
    }
    uint16_t getWeapons()
    {
      uint16_t value = 0;
      for (int i = 0; i < 16; ++i)
      {
        if (tblItems->item(i, 1)->checkState() == Qt::Checked)
          value |= 1 << i;
      }
      return value;
    }
    void setHelmets(uint16_t value)
    {
      for (int i = 0; i < 16; ++i)
        tblItems->item(i, 2)->setCheckState(((value >> i) & 1) ? Qt::Checked:Qt::Unchecked);
    }
    uint16_t getHelmets()
    {
      uint16_t value = 0;
      for (int i = 0; i < 16; ++i)
      {
        if (tblItems->item(i, 2)->checkState() == Qt::Checked)
          value |= 1 << i;
      }
      return value;
    }
    void setArmor(uint16_t value)
    {
      for (int i = 0; i < 16; ++i)
        tblItems->item(i, 3)->setCheckState(((value >> i) & 1) ? Qt::Checked:Qt::Unchecked);
    }
    uint16_t getArmor()
    {
      uint16_t value = 0;
      for (int i = 0; i < 16; ++i)
      {
        if (tblItems->item(i, 3)->checkState() == Qt::Checked)
          value |= 1 << i;
      }
      return value;
    }
    void setBoots(uint16_t value)
    {
      for (int i = 0; i < 16; ++i)
        tblItems->item(i, 4)->setCheckState(((value >> i) & 1) ? Qt::Checked:Qt::Unchecked);
    }
    uint16_t getBoots()
    {
      uint16_t value = 0;
      for (int i = 0; i < 16; ++i)
      {
        if (tblItems->item(i, 4)->checkState() == Qt::Checked)
          value |= 1 << i;
      }
      return value;
    }
    void setRings(uint16_t value)
    {
      for (int i = 0; i < 16; ++i)
        tblItems->item(i, 5)->setCheckState(((value >> i) & 1) ? Qt::Checked:Qt::Unchecked);
    }
    uint16_t getRings()
    {
      uint16_t value = 0;
      for (int i = 0; i < 16; ++i)
      {
        if (tblItems->item(i, 5)->checkState() == Qt::Checked)
          value |= 1 << i;
      }
      return value;
    }
    void setAmulets(uint16_t value)
    {
      for (int i = 0; i < 16; ++i)
        tblItems->item(i, 6)->setCheckState(((value >> i) & 1) ? Qt::Checked:Qt::Unchecked);
    }
    uint16_t getAmulets()
    {
      uint16_t value = 0;
      for (int i = 0; i < 16; ++i)
      {
        if (tblItems->item(i, 6)->checkState() == Qt::Checked)
          value |= 1 << i;
      }
      return value;
    }
};

#endif //CONFIGUREEVENT_SHOWSHOPMENU_H
