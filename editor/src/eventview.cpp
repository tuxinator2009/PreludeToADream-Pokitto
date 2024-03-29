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

#include <QKeyEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVariant>
#include "eventview.h"

EventView::EventView(QWidget *parent) : QTreeWidget(parent)
{
  connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SIGNAL(insertEvent(QTreeWidgetItem*)));
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(rightClicked(const QPoint&)));
}

EventView::~EventView()
{
}

void EventView::keyPressEvent(QKeyEvent *event)
{
  QTreeWidgetItem *item = currentItem();
  if (item == nullptr)
    return;
  if (event->key() == Qt::Key_Delete)
    emit deleteEvent(currentItem());
  else if (event->key() == Qt::Key_Space)
    emit editEvent(currentItem());
  else if (event->key() == Qt::Key_Return)
    emit insertEvent(currentItem());
  event->accept();
}

void EventView::rightClicked(const QPoint &pos)
{
  QTreeWidgetItem *item = itemAt(pos);
  if (item != nullptr)
    emit editEvent(item);
}
