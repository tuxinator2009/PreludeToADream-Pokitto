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

#ifndef CONFIGUREEVENT_CHANGEEVENTLOCATION_H
#define CONFIGUREEVENT_CHANGEEVENTLOCATION_H

#include "ui_configureevent_changeeventlocation.h"
#include "image.h"
#include "map.h"

class ConfigureEvent_ChangeEventLocation : public QDialog, public Ui::ConfigureEvent_ChangeEventLocation
{
  Q_OBJECT
  public:
    ConfigureEvent_ChangeEventLocation(Map *m, int thisID, QWidget *parent=nullptr);
    ~ConfigureEvent_ChangeEventLocation();
    void addEvent(QString value) {optEvent->addItem(value);}
    void setEvent(QString value) {optEvent->setCurrentText(value);}
    QString getEvent() {return optEvent->currentText();}
    void setLocation(int x, int y, bool off)
    {
      location = QPoint(x, y);
      offset = off;
      panMapArea->ensureVisible(x * 16, y * 16);
      updateMapView();
    }
    int getX() {return location.x();}
    int getY() {return location.y();}
    bool getOffset() {return offset;}
  protected slots:
    void on_optEvent_currentIndexChanged(int index);
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
    int thisEventID;
    bool offset;
    static const int zoom[5];
};

#endif //CONFIGUREEVENT_CHANGEEVENTLOCATION_H
