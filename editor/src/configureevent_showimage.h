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

#ifndef CONFIGUREEVENT_SHOWIMAGE_H
#define CONFIGUREEVENT_SHOWIMAGE_H

#include <QDir>
#include "ui_configureevent_showimage.h"
#include "globals.h"

class ConfigureEvent_ShowImage : public QDialog, public Ui::ConfigureEvent_ShowImage
{
  Q_OBJECT
  public:
    ConfigureEvent_ShowImage(QWidget *parent=nullptr) : QDialog(parent)
    {
      QStringList files = QDir(QString("%1/screens").arg(Globals::datadir)).entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
      setupUi(this);
      files.replaceInStrings(".png", "", Qt::CaseInsensitive);
      optImage->addItems(files);
      optImage->setCurrentIndex(0);
    }
    ~ConfigureEvent_ShowImage() {}
    void setLocation(QString value) {optImage->setCurrentText(value);}
    QString getLocation() {return optImage->currentText();}
  protected slots:
    void on_optImage_currentIndexChanged(QString text)
    {
      lblImage->setPixmap(QPixmap(QString("%1/screens/%2.png").arg(Globals::datadir).arg(text)).scaled(440, 352));
    }
};

#endif //CONFIGUREEVENT_SHOWIMAGE_H
