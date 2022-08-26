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

#ifndef MAPEVENTEDITOR_H
#define MAPEVENTEDITOR_H

#include "ui_mapeventeditor.h"
#include "image.h"
#include "mapspritepicker.h"

class MapEvent;

class MapEventEditor : public QDialog, public Ui::MapEventEditor
{
  Q_OBJECT
  public:
    MapEventEditor(MapEvent *e, bool onLoadEvent, QWidget *parent=nullptr);
    ~MapEventEditor();
  protected slots:
    void on_leName_textEdited(QString text);
    void on_chkButton_toggled(bool on);
    void on_chkDoor_toggled(bool on);
    void on_chkExit_toggled(bool on);
    void on_imgSprite_mouseDoubleClicked();
    void on_treeEvents_insertEvent(QTreeWidgetItem *item);
    void on_treeEvents_editEvent(QTreeWidgetItem *item);
    void on_treeEvents_deleteEvent(QTreeWidgetItem *item);
    void spriteClicked(uint8_t spriteID);
  private:
    MapEvent *mapEvent;
    MapSpritePicker *spritePicker;
    Image *sprite;
};

#endif //MAPEVENTEDITOR_H
