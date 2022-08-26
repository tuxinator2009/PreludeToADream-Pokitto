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

#ifndef SPRITEPICKER_H
#define SPRITEPICKER_H

#include <QWidget>
#include <QElapsedTimer>
#include "ui_spritepicker.h"

class SpritePicker : public QWidget, protected Ui::SpritePicker
{
  Q_OBJECT
  public:
    SpritePicker(QWidget *parent=nullptr, Qt::WindowFlags f = Qt::Popup);
    ~SpritePicker();
  signals:
    void spriteClicked(uint8_t spriteID);
  protected slots:
    void on_imgSprites_mousePressed(Qt::MouseButton button, QPoint pos);
    void on_imgSprites_mouseReleased(Qt::MouseButton button, QPoint pos);
  private:
    void leaveEvent(QEvent *event);
    QPoint clickPos;
    QElapsedTimer pressTime;
};

#endif //SPRITEPICKER_H
