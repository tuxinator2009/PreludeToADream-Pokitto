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

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QLabel>
#include <QWidget>
#include "image.h"

class ImageView : public QLabel
{
  Q_OBJECT
  public:
    ImageView(QWidget *parent=nullptr);
    ~ImageView();
    void setSize(QSize s);
    void setSize(int w, int h) {setSize(QSize(w, h));}
    void setImage(Image *i);
    void setTransparent(bool value) {transparent = value;}
  public slots:
    void imageChanged();
  signals:
    void mousePressed(Qt::MouseButton button, QPoint pos);
    void mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta);
    void mouseReleased(Qt::MouseButton button, QPoint pos);
    void mouseDoubleClicked(Qt::MouseButton button, QPoint pos);
    void zoomIn(QPoint pos);
    void zoomOut(QPoint pos);
  private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    Image *image;
    QPoint lastPos;
    bool transparent;
};

#endif //IMAGEVIEW_H
