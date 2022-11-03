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

#ifndef CONFIGUREEVENT_FLASHUI_H
#define CONFIGUREEVENT_FLASHUI_H

#include "ui_configureevent_flashui.h"
#include "colorpicker.h"
#include "image.h"

class ConfigureEvent_FlashUI : public QDialog, public Ui::ConfigureEvent_FlashUI
{
  Q_OBJECT
  public:
    ConfigureEvent_FlashUI(QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      colorPicker = new ColorPicker(this);
      colorPicker->setMinimumSize(QSize(colorPicker->sizeHint().width(), colorPicker->sizeHint().width()));
      colorPicker->setMaximumSize(QSize(colorPicker->sizeHint().width(), colorPicker->sizeHint().width()));
      colorPicker->setVisible(false);
      connect(colorPicker, SIGNAL(colorClicked(int)), this, SLOT(colorPicker_colorClicked(int)));
    }
    ~ConfigureEvent_FlashUI() {}
    void setDuration(int value) {numDuration->setValue(value);}
    int getDuration() {return numDuration->value();}
    void setColor(int index)
    {
      QRgb color = Image::palette[index];
      btnColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
      uiColor = index;
    }
    int getColor() {return uiColor;}
  protected slots:
    void on_btnColor_clicked()
    {
      colorPicker->move(btnColor->mapToGlobal(QPoint(0, 0)));
      colorPicker->show();
    }
    void colorPicker_colorClicked(int index)
    {
      QRgb color = Image::palette[index];
      btnColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
      colorPicker->hide();
      uiColor = index;
    }
  private:
    ColorPicker *colorPicker;
    int uiColor;
};

#endif //CONFIGUREEVENT_FLASHUI_H
