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
#include "skillsspellseditor.h"

SkillsSpellsEditor::SkillsSpellsEditor(bool editSkills, QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  lblSkillSpell->setText((editSkills) ? "Skills":"Spells");
  wLearned->setVisible(editSkills);
  setWindowTitle((editSkills) ? "Skills Editor":"Spells Editor");
  editingSkills = editSkills;
  ignoreEvents = true;
  for (int i = 0; i < 16; ++i)
  {
    if (editSkills)
      lstSkillsSpells->item(i)->setText(Globals::skills[i].name);
    else
      lstSkillsSpells->item(i)->setText(Globals::spells[i].name);
  }
  lstSkillsSpells->item(0)->setSelected(true);
  lstSkillsSpells->setCurrentItem(lstSkillsSpells->item(0));
  on_lstSkillsSpells_itemSelectionChanged();
  resize(sizeHint());
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());
}

SkillsSpellsEditor::~SkillsSpellsEditor()
{
}

void SkillsSpellsEditor::on_lstSkillsSpells_itemSelectionChanged()
{
  int id = lstSkillsSpells->currentRow();
  if (id < 0)
    return;
  ignoreEvents = true;
  if (editingSkills)
  {
    leName->setText(Globals::skills[id].name);
    leDescription->setText(Globals::skills[id].description);
    numMP->setValue(Globals::skills[id].price);
    numLearned->setValue(Globals::skillLearned[id]);
  }
  else
  {
    leName->setText(Globals::spells[id].name);
    leDescription->setText(Globals::spells[id].description);
    numMP->setValue(Globals::spells[id].price);
  }
  ignoreEvents = false;
}

void SkillsSpellsEditor::on_leName_textChanged(QString text)
{
  if (ignoreEvents)
    return;
  lstSkillsSpells->item(lstSkillsSpells->currentRow())->setText(text);
  if (editingSkills)
    Globals::skills[lstSkillsSpells->currentRow()].name = text;
  else
    Globals::spells[lstSkillsSpells->currentRow()].name = text;
}

void SkillsSpellsEditor::on_numMP_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  if (editingSkills)
    Globals::skills[lstSkillsSpells->currentRow()].price = value;
  else
    Globals::spells[lstSkillsSpells->currentRow()].price = value;
}

void SkillsSpellsEditor::on_leDescription_textChanged(QString text)
{
  if (ignoreEvents)
    return;
  if (editingSkills)
    Globals::skills[lstSkillsSpells->currentRow()].description = text;
  else
    Globals::spells[lstSkillsSpells->currentRow()].description = text;
}

void SkillsSpellsEditor::on_numLearned_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  if (editingSkills)
    Globals::skillLearned[lstSkillsSpells->currentRow()] = value;
}
