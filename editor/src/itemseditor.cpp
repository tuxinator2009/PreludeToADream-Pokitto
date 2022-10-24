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

#include "globals.h"
#include "itemseditor.h"

const char *ItemsEditor::spellIcons[4] = {":/icons/spellResistance0.png",":/icons/spellResistance05.png",":/icons/spellResistance1.png",":/icons/spellResistance2.png"};

ItemsEditor::ItemsEditor(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  itemType = 0;
  wStats->setEnabled(false);
  wResistance->setEnabled(false);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[0][i].name);
  lstItems->item(0)->setSelected(true);
  lstItems->setCurrentItem(lstItems->item(0));
  on_lstItems_itemSelectionChanged();
  resize(sizeHint());
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());
}

ItemsEditor::~ItemsEditor()
{
}

void ItemsEditor::on_btnConsumables_toggled(bool on)
{
  if (!on)
    return;
  itemType = 0;
  wStats->setEnabled(false);
  wResistance->setEnabled(false);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_btnTradeables_toggled(bool on)
{
  if (!on)
    return;
  itemType = 1;
  wStats->setEnabled(false);
  wResistance->setEnabled(false);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_btnWeapons_toggled(bool on)
{
  if (!on)
    return;
  itemType = 2;
  wStats->setEnabled(true);
  wResistance->setEnabled(true);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_btnHelmets_toggled(bool on)
{
  if (!on)
    return;
  itemType = 3;
  wStats->setEnabled(true);
  wResistance->setEnabled(true);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_btnArmor_toggled(bool on)
{
  if (!on)
    return;
  itemType = 4;
  wStats->setEnabled(true);
  wResistance->setEnabled(true);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_btnBoots_toggled(bool on)
{
  if (!on)
    return;
  itemType = 5;
  wStats->setEnabled(true);
  wResistance->setEnabled(true);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_btnRings_toggled(bool on)
{
  if (!on)
    return;
  itemType = 6;
  wStats->setEnabled(true);
  wResistance->setEnabled(true);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_btnAmulets_toggled(bool on)
{
  if (!on)
    return;
  itemType = 7;
  wStats->setEnabled(true);
  wResistance->setEnabled(true);
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
    lstItems->item(i)->setText(Globals::items[itemType][i].name);
  on_lstItems_itemSelectionChanged();
}

void ItemsEditor::on_lstItems_itemSelectionChanged()
{
  int itemID = lstItems->currentRow();
  if (itemID < 0)
    return;
  ignoreEvents = true;
  leName->setText(Globals::items[itemType][itemID].name);
  leDescription->setText(Globals::items[itemType][itemID].description);
  numPrice->setValue(Globals::items[itemType][itemID].price);
  if (itemType >= 2)
  {
    numAttack->setValue(Globals::equipmentStats[itemType - 2][itemID].stats[0]);
    numDefense->setValue(Globals::equipmentStats[itemType - 2][itemID].stats[1]);
    numAgility->setValue(Globals::equipmentStats[itemType - 2][itemID].stats[2]);
    numMagic->setValue(Globals::equipmentStats[itemType - 2][itemID].stats[3]);
    for (int i = 0; i < 7; ++i)
      lstSpells->item(i)->setIcon(QIcon(spellIcons[(Globals::equipmentStats[itemType - 2][itemID].spellResistance >> (i * 2)) & 3]));
    numPoison->setValue(Globals::equipmentStats[itemType - 2][itemID].statusResistance[0]);
    numSlow->setValue(Globals::equipmentStats[itemType - 2][itemID].statusResistance[1]);
  }
  ignoreEvents = false;
}

void ItemsEditor::on_leName_textChanged(QString value)
{
  if (ignoreEvents)
    return;
  lstItems->item(lstItems->currentRow())->setText(value);
  Globals::items[itemType][lstItems->currentRow()].name = value;
}

void ItemsEditor::on_numPrice_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::items[itemType][lstItems->currentRow()].price = value;
}

void ItemsEditor::on_leDescription_textChanged(QString value)
{
  if (ignoreEvents)
    return;
  Globals::items[itemType][lstItems->currentRow()].description = value;
}

void ItemsEditor::on_numAttack_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].stats[0] = value;
}

void ItemsEditor::on_numDefense_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].stats[1] = value;
}

void ItemsEditor::on_numAgility_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].stats[2] = value;
}

void ItemsEditor::on_numMagic_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].stats[3] = value;
}

void ItemsEditor::on_lstSpells_itemClicked(QListWidgetItem *item)
{
  int spellID = lstSpells->row(item);
  uint16_t resistance = (Globals::equipmentStats[itemType - 2][lstItems->currentRow()].spellResistance >> (spellID * 2)) & 3;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].spellResistance &= ~(3 << (spellID * 2));
  resistance = (resistance + 1) % 4;
  item->setIcon(QIcon(spellIcons[resistance]));
  resistance <<= spellID * 2;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].spellResistance |= resistance;
}

void ItemsEditor::on_numPoison_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].statusResistance[0] = value;
}

void ItemsEditor::on_numSlow_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::equipmentStats[itemType - 2][lstItems->currentRow()].statusResistance[1] = value;
}
