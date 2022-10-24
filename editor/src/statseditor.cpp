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
#include "statseditor.h"

const uint8_t StatsEditor::statColors[7] = {175, 207, 183, 191, 7, 231, 247};

StatsEditor::StatsEditor(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  ignoreEvents = true;
  numHPStart->setValue(Globals::statsGrowth[0].start);
  dblHPBase->setValue(Globals::statsGrowth[0].base);
  dblHPExp->setValue(Globals::statsGrowth[0].exponent);
  hpImage = new Image(QSize(99, 192));
  imgHP->setImage(hpImage);
  updateImage(hpImage, imgHP, Globals::statsGrowth[0], 9999, statColors[0]);
  numMPStart->setValue(Globals::statsGrowth[1].start);
  dblMPBase->setValue(Globals::statsGrowth[1].base);
  dblMPExp->setValue(Globals::statsGrowth[1].exponent);
  mpImage = new Image(QSize(99, 192));
  imgMP->setImage(mpImage);
  updateImage(mpImage, imgMP, Globals::statsGrowth[1], 255, statColors[1]);
  numAttackStart->setValue(Globals::statsGrowth[2].start);
  dblAttackBase->setValue(Globals::statsGrowth[2].base);
  dblAttackExp->setValue(Globals::statsGrowth[2].exponent);
  attackImage = new Image(QSize(99, 192));
  imgAttack->setImage(attackImage);
  updateImage(attackImage, imgAttack, Globals::statsGrowth[2], 999, statColors[2]);
  numDefenseStart->setValue(Globals::statsGrowth[3].start);
  dblDefenseBase->setValue(Globals::statsGrowth[3].base);
  dblDefenseExp->setValue(Globals::statsGrowth[3].exponent);
  defenseImage = new Image(QSize(99, 192));
  imgDefense->setImage(defenseImage);
  updateImage(defenseImage, imgDefense, Globals::statsGrowth[3], 999, statColors[3]);
  numAgilityStart->setValue(Globals::statsGrowth[4].start);
  dblAgilityBase->setValue(Globals::statsGrowth[4].base);
  dblAgilityExp->setValue(Globals::statsGrowth[4].exponent);
  agilityImage = new Image(QSize(99, 192));
  imgAgility->setImage(agilityImage);
  updateImage(agilityImage, imgAgility, Globals::statsGrowth[4], 999, statColors[4]);
  numMagicStart->setValue(Globals::statsGrowth[5].start);
  dblMagicBase->setValue(Globals::statsGrowth[5].base);
  dblMagicExp->setValue(Globals::statsGrowth[5].exponent);
  magicImage = new Image(QSize(99, 192));
  imgMagic->setImage(magicImage);
  updateImage(magicImage, imgMagic, Globals::statsGrowth[5], 999, statColors[5]);
  numExperienceStart->setValue(Globals::statsGrowth[6].start);
  dblExperienceBase->setValue(Globals::statsGrowth[6].base);
  dblExperienceExp->setValue(Globals::statsGrowth[6].exponent);
  experienceImage = new Image(QSize(99, 192));
  imgExperience->setImage(experienceImage);
  updateImage(experienceImage, imgExperience, Globals::statsGrowth[6], 9999, statColors[6]);
  ignoreEvents = false;
}

StatsEditor::~StatsEditor()
{
  delete hpImage;
  delete mpImage;
  delete attackImage;
  delete defenseImage;
  delete agilityImage;
  delete magicImage;
  delete experienceImage;
}

void StatsEditor::on_numHPStart_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[0].start = value;
  updateImage(hpImage, imgHP, Globals::statsGrowth[0], 9999, statColors[0]);
}

void StatsEditor::on_dblHPBase_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[0].base = value;
  updateImage(hpImage, imgHP, Globals::statsGrowth[0], 9999, statColors[0]);
}

void StatsEditor::on_dblHPExp_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[0].exponent = value;
  updateImage(hpImage, imgHP, Globals::statsGrowth[0], 9999, statColors[0]);
}

void StatsEditor::on_numMPStart_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[1].start = value;
  updateImage(mpImage, imgMP, Globals::statsGrowth[1], 255, statColors[1]);
}

void StatsEditor::on_dblMPBase_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[1].base = value;
  updateImage(mpImage, imgMP, Globals::statsGrowth[1], 255, statColors[1]);
}

void StatsEditor::on_dblMPExp_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[1].exponent = value;
  updateImage(mpImage, imgMP, Globals::statsGrowth[1], 255, statColors[1]);
}

void StatsEditor::on_numAttackStart_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[2].start = value;
  updateImage(attackImage, imgAttack, Globals::statsGrowth[2], 999, statColors[2]);
}

void StatsEditor::on_dblAttackBase_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[2].base = value;
  updateImage(attackImage, imgAttack, Globals::statsGrowth[2], 999, statColors[2]);
}

void StatsEditor::on_dblAttackExp_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[2].exponent = value;
  updateImage(attackImage, imgAttack, Globals::statsGrowth[2], 999, statColors[2]);
}

void StatsEditor::on_numDefenseStart_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[3].start = value;
  updateImage(defenseImage, imgDefense, Globals::statsGrowth[3], 999, statColors[3]);
}

void StatsEditor::on_dblDefenseBase_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[3].base = value;
  updateImage(defenseImage, imgDefense, Globals::statsGrowth[3], 999, statColors[3]);
}

void StatsEditor::on_dblDefenseExp_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[3].exponent = value;
  updateImage(defenseImage, imgDefense, Globals::statsGrowth[3], 999, statColors[3]);
}

void StatsEditor::on_numAgilityStart_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[4].start = value;
  updateImage(agilityImage, imgAgility, Globals::statsGrowth[4], 999, statColors[4]);
}

void StatsEditor::on_dblAgilityBase_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[4].base = value;
  updateImage(agilityImage, imgAgility, Globals::statsGrowth[4], 999, statColors[4]);
}

void StatsEditor::on_dblAgilityExp_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[4].exponent = value;
  updateImage(agilityImage, imgAgility, Globals::statsGrowth[4], 999, statColors[4]);
}

void StatsEditor::on_numMagicStart_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[5].start = value;
  updateImage(magicImage, imgMagic, Globals::statsGrowth[5], 999, statColors[5]);
}

void StatsEditor::on_dblMagicBase_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[5].base = value;
  updateImage(magicImage, imgMagic, Globals::statsGrowth[5], 999, statColors[5]);
}

void StatsEditor::on_dblMagicExp_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[5].exponent = value;
  updateImage(magicImage, imgMagic, Globals::statsGrowth[5], 999, statColors[5]);
}

void StatsEditor::on_numExperienceStart_valueChanged(int value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[6].start = value;
  updateImage(experienceImage, imgExperience, Globals::statsGrowth[6], 9999, statColors[6]);
}

void StatsEditor::on_dblExperienceBase_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[6].base = value;
  updateImage(experienceImage, imgExperience, Globals::statsGrowth[6], 9999, statColors[6]);
}

void StatsEditor::on_dblExperienceExp_valueChanged(double value)
{
  if (ignoreEvents)
    return;
  Globals::statsGrowth[6].exponent = value;
  updateImage(experienceImage, imgExperience, Globals::statsGrowth[6], 9999, statColors[6]);
}

void StatsEditor::updateImage(Image *image, ImageView *view, const Globals::StatGrowth &growth, int maxValue, uint8_t color)
{
  image->fillRect(QRect(0, 0, 99, 192), 0);
  for (int i = 0; i < 99; ++i)
  {
    int stat = Globals::getStatForLevel(i, growth);
    stat = stat * 192 / maxValue;
    image->drawVLine(QPoint(i, 191 - stat), stat, color);
  }
  view->imageChanged();
}
