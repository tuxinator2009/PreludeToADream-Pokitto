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

#include "colorpicker.h"
#include "image.h"

ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent)
{
  setupUi(this);
  for (int row = 0; row < 32; ++row)
  {
    for (int col = 0; col < 8; ++col)
    {
      QWidget *w = new QWidget();
      QRgb color = Image::palette[row * 8 + col];
      w->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
      tblPalette->setCellWidget(row, col, w);
    }
  }
}

ColorPicker::~ColorPicker()
{
}

void ColorPicker::selectColor(int index)
{
  tblPalette->setCurrentCell(index / 8, index % 8);
  tblPalette->item(index / 8, index % 8)->setSelected(true);
}

void ColorPicker::on_tblPalette_cellClicked(int row, int column)
{
  emit colorClicked(row * 8 + column);
}

void ColorPicker::leaveEvent(QEvent *event)
{
  event->accept();
  this->hide();
}
