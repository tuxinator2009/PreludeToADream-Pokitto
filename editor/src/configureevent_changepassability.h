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

#ifndef CONFIGUREEVENT_CHANGEPASSABILITY_H
#define CONFIGUREEVENT_CHANGEPASSABILITY_H

#include "ui_configureevent_changepassability.h"
#include "image.h"
#include "map.h"

class ConfigureEvent_ChangePassability : public QDialog, public Ui::ConfigureEvent_ChangePassability
{
  Q_OBJECT
  public:
    ConfigureEvent_ChangePassability(Map *m, QWidget *parent=nullptr);
    ~ConfigureEvent_ChangePassability();
    void setLocation(int x, int y)
    {
      location = QPoint(x, y);
      panMapArea->ensureVisible(x * 16, y * 16);
      updateMapView();
    }
    int getX() {return location.x();}
    int getY() {return location.y();}
    void setOperation(int value) {optOperation->setCurrentIndex(value);}
    int getOperation() {return optOperation->currentIndex();}
  protected slots:
    void on_imgMapView_mousePressed(Qt::MouseButton button, QPoint pos);
    void on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta);
    void on_imgMapView_zoomOut(QPoint pos);
    void on_imgMapView_zoomIn(QPoint pos);
    void updateZoom();
  private:
    void updateMapView();
    Map *map;
    Image *mapImage;
    QPoint location;
    QPoint zoomPos;
    int currentZoom;
    static const int zoom[5];
};

#endif //CONFIGUREEVENT_CHANGEPASSABILITY_H
