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

#ifndef CONFIGUREEVENT_PLAYMUSIC_H
#define CONFIGUREEVENT_PLAYMUSIC_H

#include <QMessageBox>
#include "ui_configureevent_playmusic.h"
#include "globals.h"

class ConfigureEvent_PlayMusic : public QDialog, public Ui::ConfigureEvent_PlayMusic
{
  Q_OBJECT
  public:
    ConfigureEvent_PlayMusic(QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      optMusic->addItems(Globals::bgms);
    }
    ~ConfigureEvent_PlayMusic() {Globals::audio->stop();}
    void setMusic(QString value) {optMusic->setCurrentText(value);}
    QString getMusic() {return optMusic->currentText();}
    void setLoop(bool value) {chkLoop->setChecked(value);}
    bool getLoop() {return chkLoop->isChecked();}
  protected slots:
    void on_btnPlayMusic_toggled(bool on)
    {
      if (on)
      {
        Globals::audio->setMedia(QUrl::fromLocalFile(QString("%1/music/%2.ogg").arg(Globals::datadir).arg(optMusic->currentText())));
        Globals::audio->play();
        if (Globals::audio->error() != QMediaPlayer::NoError)
          QMessageBox::critical(this, "Playback Failed", QString("Failed to play %1.wav\nReason: %2").arg(optMusic->currentText()).arg(Globals::audio->errorString()));
        btnPlayMusic->setIcon(QIcon(":/icons/stop.png"));
      }
      else
      {
        Globals::audio->stop();
        btnPlayMusic->setIcon(QIcon(":/icons/play.png"));
      }
    }
};

#endif //CONFIGUREEVENT_PLAYMUSIC_H
