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

#ifndef RESIZEMAP_H
#define RESIZEMAP_H

#include "ui_resizemap.h"

class ResizeMap : public QDialog, public Ui::ResizeMap
{
  Q_OBJECT
  public:
    ResizeMap(QWidget *parent=nullptr) : QDialog(parent) {setupUi(this);}
    ~ResizeMap() {}
    void setWidth(int value) {numWidth->setValue(value);}
    int getWidth() {return numWidth->value();}
    void setHeight(int value) {numHeight->setValue(value);}
    int getHeight() {return numHeight->value();}
  protected slots:
    void on_numWidth_valueChanged(int value)
    {
      lblTileWidth->setText(QString::number(value * 32));
      lblStepWidth->setText(QString::number(value * 16));
    }
    void on_numHeight_valueChanged(int value)
    {
      lblTileHeight->setText(QString::number(value * 32));
      lblStepHeight->setText(QString::number(value * 16));
    }
};

#endif //RESIZEMAP_H
