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

#include <QLabel>
#include <QMouseEvent>
#include <QWidget>
#include <QWheelEvent>
#include "image.h"
#include "imageview.h"

ImageView::ImageView(QWidget *parent) : QLabel(parent)
{
  image = nullptr;
  transparent = false;
}

ImageView::~ImageView()
{
  image = nullptr;
}

void ImageView::setSize(QSize s)
{
  setMinimumSize(s);
  setMaximumSize(s);
  resize(s);
}

void ImageView::setImage(Image *i)
{
  image = i;
  connect(image, SIGNAL(imageChanged()), this, SLOT(imageChanged()));
}

void ImageView::imageChanged()
{
  setPixmap(QPixmap::fromImage(image->toQImage(transparent).scaled(size())));
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
  emit mousePressed(event->button(), event->pos());
  event->accept();
  lastPos = event->globalPos();
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
  emit mouseMoved(event->buttons(), event->pos(), event->globalPos() - lastPos);
  event->accept();
  lastPos = event->globalPos();
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
  emit mouseReleased(event->button(), event->pos());
  event->accept();
  lastPos = event->globalPos();
}

void ImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit mouseDoubleClicked(event->button(), event->pos());
  event->accept();
  lastPos = event->globalPos();
}

void ImageView::wheelEvent(QWheelEvent *event)
{
  QPoint point = (event->angleDelta().isNull()) ? event->pixelDelta():event->angleDelta();
  if (point.y() < 0)
    emit zoomOut(event->pos());
  else if (point.y() > 0)
    emit zoomIn(event->pos());
  event->accept();
}
