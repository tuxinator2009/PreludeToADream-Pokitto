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

#include <QEvent>
#include "globals.h"
#include "spritepicker.h"

SpritePicker::SpritePicker(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
  setupUi(this);
  imgSprites->setSize(Globals::sprites->getSize());
  resize(sizeHint());
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());
  imgSprites->setImage(Globals::sprites);
  imgSprites->setTransparent(true);
  imgSprites->imageChanged();
}

SpritePicker::~SpritePicker()
{
}

void SpritePicker::on_imgSprites_mousePressed(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    clickPos = pos;
    pressTime.start();
  }
}

void SpritePicker::on_imgSprites_mouseReleased(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton && pressTime.elapsed() <= 250 && QPoint(pos.x() - clickPos.x(), pos.y() - clickPos.y()).manhattanLength() <= 4)
    emit spriteClicked(pos.y() / 24 * 16 + pos.x() / 16);
}

void SpritePicker::leaveEvent(QEvent *event)
{
  event->accept();
  this->hide();
}
