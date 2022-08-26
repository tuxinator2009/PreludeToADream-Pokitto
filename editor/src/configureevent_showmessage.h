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

#ifndef CONFIGUREEVENT_SHOWMESSAGE_H
#define CONFIGUREEVENT_SHOWMESSAGE_H

#include "ui_configureevent_showmessage.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"

class ConfigureEvent_ShowMessage : public QDialog, public Ui::ConfigureEvent_ShowMessage
{
  Q_OBJECT
  public:
    ConfigureEvent_ShowMessage(QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      gfxFont->setSize(QSize(256, 112));
      gfxFont->setTransparent(false);
      gfxFont->setImage(Globals::font);
      gfxFont->imageChanged();
    }
    QString getMessage() {return txtMessage->toPlainText();}
    void setMessage(QString value) {txtMessage->setPlainText(value);}
  protected slots:
    void on_btnInsertValue_clicked()
    {
      QString value = optValue->currentText();
      txtMessage->insertPlainText(value.mid(value.indexOf('(') + 1, value.indexOf(')') - value.indexOf('(') - 1));
    }
    void on_gfxFont_mouseReleased(Qt::MouseButton button, QPoint pos)
    {
      if (button == Qt::LeftButton)
        txtMessage->insertPlainText(QChar(Globals::codes[pos.y() / 16 * 16 + pos.x() / 16]));
    }
};

#endif //CONFIGUREEVENT_SHOWMESSAGE_H
