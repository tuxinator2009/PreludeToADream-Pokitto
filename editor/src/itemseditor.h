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

#ifndef ITEMSEDITOR_H
#define ITEMSEDITOR_H

#include "ui_itemseditor.h"

class ItemsEditor : public QDialog, public Ui::ItemsEditor
{
  Q_OBJECT
  public:
    ItemsEditor(QWidget *parent=nullptr);
    ~ItemsEditor();
  protected slots:
    void on_btnConsumables_toggled(bool on);
    void on_btnTradeables_toggled(bool on);
    void on_btnWeapons_toggled(bool on);
    void on_btnHelmets_toggled(bool on);
    void on_btnArmor_toggled(bool on);
    void on_btnBoots_toggled(bool on);
    void on_btnRings_toggled(bool on);
    void on_btnAmulets_toggled(bool on);
    void on_lstItems_itemSelectionChanged();
    void on_leName_textChanged(QString value);
    void on_numPrice_valueChanged(int value);
    void on_leDescription_textChanged(QString value);
    void on_numAttack_valueChanged(int value);
    void on_numDefense_valueChanged(int value);
    void on_numAgility_valueChanged(int value);
    void on_numMagic_valueChanged(int value);
    void on_lstSpells_itemClicked(QListWidgetItem *item);
    void on_numPoison_valueChanged(int value);
    void on_numSlow_valueChanged(int value);
  private:
    int itemType;
    bool ignoreEvents;
};

#endif //ITEMSEDITOR_H
