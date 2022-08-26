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
#include "image.h"
#include "imageview.h"
#include "map.h"
#include "mapspritepicker.h"

MapSpritePicker::MapSpritePicker(Map *map, QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
  setupUi(this);
  image = new Image(QSize(128, 48));
  imgSprites->setSize(QSize(256, 96));
  imgSprites->setTransparent(true);
  imgSprites->setImage(image);
  for (int i = 0; i < 16; ++i)
  {
    uint8_t spriteID = map->getSpriteID(i);
    image->blitImage(*Globals::sprites, QRect(spriteID % 16 * 16, spriteID / 16 * 24, 16, 24), QPoint(i % 8 * 16, i / 8 * 24), true);
  }
  imgSprites->imageChanged();
  pressTime.start();
}

MapSpritePicker::~MapSpritePicker()
{
}

void MapSpritePicker::on_imgSprites_mousePressed(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    clickPos = pos;
    pressTime.start();
  }
}

void MapSpritePicker::on_imgSprites_mouseReleased(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton && pressTime.elapsed() <= 250 && QPoint(pos.x() - clickPos.x(), pos.y() - clickPos.y()).manhattanLength() <= 4)
    emit spriteClicked(pos.y() / 48 * 8 + pos.x() / 32);
}

void MapSpritePicker::leaveEvent(QEvent *event)
{
  event->accept();
  this->hide();
}
