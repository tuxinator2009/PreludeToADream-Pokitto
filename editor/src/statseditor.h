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

#ifndef STATSEDITOR_H
#define STATSEDITOR_H

#include "ui_statseditor.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"

class StatsEditor : public QDialog, public Ui::StatsEditor
{
  Q_OBJECT
  public:
    StatsEditor(QWidget *parent=nullptr);
    ~StatsEditor();
  protected slots:
    void on_numHPStart_valueChanged(int value);
    void on_dblHPBase_valueChanged(double value);
    void on_dblHPExp_valueChanged(double value);
    void on_numMPStart_valueChanged(int value);
    void on_dblMPBase_valueChanged(double value);
    void on_dblMPExp_valueChanged(double value);
    void on_numAttackStart_valueChanged(int value);
    void on_dblAttackBase_valueChanged(double value);
    void on_dblAttackExp_valueChanged(double value);
    void on_numDefenseStart_valueChanged(int value);
    void on_dblDefenseBase_valueChanged(double value);
    void on_dblDefenseExp_valueChanged(double value);
    void on_numAgilityStart_valueChanged(int value);
    void on_dblAgilityBase_valueChanged(double value);
    void on_dblAgilityExp_valueChanged(double value);
    void on_numMagicStart_valueChanged(int value);
    void on_dblMagicBase_valueChanged(double value);
    void on_dblMagicExp_valueChanged(double value);
    void on_numExperienceStart_valueChanged(int value);
    void on_dblExperienceBase_valueChanged(double value);
    void on_dblExperienceExp_valueChanged(double value);
  private:
    void updateImage(Image *image, ImageView *view, const Globals::StatGrowth &growth, int maxValue, uint8_t color);
    static const uint8_t statColors[7];
    Image *hpImage;
    Image *mpImage;
    Image *attackImage;
    Image *defenseImage;
    Image *agilityImage;
    Image *magicImage;
    Image *experienceImage;
    bool ignoreEvents;
};

#endif //STATSEDITOR_H
