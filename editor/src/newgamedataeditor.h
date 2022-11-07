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

#ifndef NEWGAMEDATAEDITOR_H
#define NEWGAMEDATAEDITOR_H

#include "ui_newgamedataeditor.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"
#include "map.h"
#include "mapevent.h"
#include "xmlParser.h"

class NewGameDataEditor : public QDialog, public Ui::NewGameDataEditor
{
  Q_OBJECT
  public:
    NewGameDataEditor(QWidget *parent=nullptr);
    ~NewGameDataEditor();
  protected slots:
    void on_leDefaultName_textChanged(QString text);
    void on_btnFacingUp_toggled(bool on);
    void on_btnFacingRight_toggled(bool on);
    void on_btnFacingDown_toggled(bool on);
    void on_btnFacingLeft_toggled(bool on);
    void on_treeMaps_itemClicked(QTreeWidgetItem *item);
    void on_imgMapView_mousePressed(Qt::MouseButton button, QPoint pos);
    void on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta);
    void on_imgMapView_mouseReleased(Qt::MouseButton button, QPoint pos);
    void on_imgMapView_zoomOut(QPoint pos);
    void on_imgMapView_zoomIn(QPoint pos);
    void updateZoom();
    void on_treeEvents_insertEvent(QTreeWidgetItem *item);
    void on_treeEvents_editEvent(QTreeWidgetItem *item);
    void on_treeEvents_deleteEvent(QTreeWidgetItem *item);
  private:
    void buildMapTree(QTreeWidgetItem *parent, QTreeWidgetItem *itemToCopy);
    void updateMapView();
    Map *map;
    Image *mapImage;
    QPoint location;
    QPoint zoomPos;
    int currentZoom;
    static const int zoom[5];
};

#endif
