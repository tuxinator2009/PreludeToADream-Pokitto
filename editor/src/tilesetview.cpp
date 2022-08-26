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

#include <QGraphicsView>
#include "globals.h"
#include "tileset.h"
#include "tilesetview.h"

TilesetView::TilesetView(QWidget *parent) : QGraphicsView(parent)
{
  scene = new QGraphicsScene(0, 0, 128, 64, this);
  setScene(scene);
  tilesItem = new QGraphicsPixmapItem(QPixmap(128, 64));
  tilesItem->setZValue(0);
  scene->addItem(tilesItem);
  selection = new QGraphicsRectItem(0, 0, 0, 0);
  selection->setZValue(1);
  selection->setBrush(Qt::NoBrush);
  selection->setPen(QPen(QColor(0, 255, 255)));
  scene->addItem(selection);
  setBackgroundBrush(QBrush(QPixmap(":/icons/bg.png")));
  scale(2, 2);
}

TilesetView::~TilesetView()
{
}

void TilesetView::changeTileset(int id)
{
  tilesItem->setPixmap(QPixmap::fromImage(Globals::tilesets[id]->getImage()->toQImage(QRect(0, 0, 128, 64))));
}

void TilesetView::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton)
  {
    QRect rect;
    QPoint end = mapToScene(event->pos()).toPoint();
    end.setX(end.x() / 8);
    end.setY(end.y() / 8);
    if (end.x() < 0)
      end.setX(0);
    if (end.x() >= 16)
      end.setX(15);
    if (end.y() < 0)
      end.setY(0);
    if (end.y() >= 8)
      end.setY(7);
    if (end.x() < start.x())
    {
      rect.setLeft(end.x());
      rect.setRight(start.x());
    }
    else
    {
      rect.setLeft(start.x());
      rect.setRight(end.x());
    }
    if (end.y() < start.y())
    {
      rect.setTop(end.y());
      rect.setBottom(start.y());
    }
    else
    {
      rect.setTop(start.y());
      rect.setBottom(end.y());
    }
    selection->setRect(QRect(rect.x() * 8, rect.y() * 8, rect.width() * 8, rect.height() * 8));
  }
}

void TilesetView::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    start = mapToScene(event->pos()).toPoint();
    start.setX(start.x() / 8);
    start.setY(start.y() / 8);
    if (start.x() < 0)
      start.setX(0);
    if (start.x() >= 16)
      start.setX(15);
    if (start.y() < 0)
      start.setY(0);
    if (start.y() >= 8)
      start.setY(7);
    selection->setRect(start.x() * 8, start.y() * 8, 8, 8);
  }
}

void TilesetView::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    QRect rect;
    QPoint end = mapToScene(event->pos()).toPoint();
    end.setX(end.x() / 8);
    end.setY(end.y() / 8);
    if (end.x() < 0)
      end.setX(0);
    if (end.x() >= 16)
      end.setX(15);
    if (end.y() < 0)
      end.setY(0);
    if (end.y() >= 8)
      end.setY(7);
    if (end.x() < start.x())
    {
      rect.setLeft(end.x());
      rect.setRight(start.x());
    }
    else
    {
      rect.setLeft(start.x());
      rect.setRight(end.x());
    }
    if (end.y() < start.y())
    {
      rect.setTop(end.y());
      rect.setBottom(start.y());
    }
    else
    {
      rect.setTop(start.y());
      rect.setBottom(end.y());
    }
    selection->setRect(QRect(rect.x() * 8, rect.y() * 8, rect.width() * 8, rect.height() * 8));
    emit selectionChanged(rect);
  }
}
