/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2020 Justin (tuxinator2009) Davis                                *
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

#include <QWidget>
#include "map.h"
#include "mapflags.h"

MapFlags::MapFlags(QWidget *parent) : QWidget(parent)
{
  setupUi(this);
  connect(chkClipUp, SIGNAL(toggled(bool)), this, SLOT(updateFlags()));
  connect(chkClipDown, SIGNAL(toggled(bool)), this, SLOT(updateFlags()));
  connect(chkClipLeft, SIGNAL(toggled(bool)), this, SLOT(updateFlags()));
  connect(chkClipRight, SIGNAL(toggled(bool)), this, SLOT(updateFlags()));
  connect(chkDark, SIGNAL(toggled(bool)), this, SLOT(updateFlags()));
  connect(chkDream, SIGNAL(toggled(bool)), this, SLOT(updateFlags()));
  connect(chkSpirit, SIGNAL(toggled(bool)), this, SLOT(updateFlags()));
}

MapFlags::~MapFlags()
{
}

void MapFlags::setFlags(uint8_t flags)
{
  chkClipUp->setChecked(flags & Map::FLAG_CLIP_UP);
  chkClipDown->setChecked(flags & Map::FLAG_CLIP_DOWN);
  chkClipLeft->setChecked(flags & Map::FLAG_CLIP_LEFT);
  chkClipRight->setChecked(flags & Map::FLAG_CLIP_RIGHT);
  chkDark->setChecked(flags & Map::FLAG_DARK);
  chkDream->setChecked(flags & Map::FLAG_DREAM);
  chkSpirit->setChecked(flags & Map::FLAG_SPIRIT);
}

void MapFlags::updateFlags()
{
  uint8_t flags = 0;
  if (chkClipUp->isChecked())
    flags |= Map::FLAG_CLIP_UP;
  if (chkClipDown->isChecked())
    flags |= Map::FLAG_CLIP_DOWN;
  if (chkClipLeft->isChecked())
    flags |= Map::FLAG_CLIP_LEFT;
  if (chkClipRight->isChecked())
    flags |= Map::FLAG_CLIP_RIGHT;
  if (chkDark->isChecked())
    flags |= Map::FLAG_DARK;
  if (chkDream->isChecked())
    flags |= Map::FLAG_DREAM;
  if (chkSpirit->isChecked())
    flags |= Map::FLAG_SPIRIT;
  emit flagsChanged(flags);
}

void MapFlags::leaveEvent(QEvent *event)
{
  event->accept();
  this->hide();
}
