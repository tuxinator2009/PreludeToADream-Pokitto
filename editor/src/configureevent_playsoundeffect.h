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

#ifndef CONFIGUREEVENT_PLAYSOUNDEFFECT_H
#define CONFIGUREEVENT_PLAYSOUNDEFFECT_H

#include <QMessageBox>
#include "ui_configureevent_playsoundeffect.h"
#include "globals.h"

class ConfigureEvent_PlaySoundEffect : public QDialog, public Ui::ConfigureEvent_PlaySoundEffect
{
  Q_OBJECT
  public:
    ConfigureEvent_PlaySoundEffect(QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      optSoundEffect->addItems(Globals::sfx);
    }
    ~ConfigureEvent_PlaySoundEffect() {Globals::audio->stop();}
    void setSoundEffect(QString value) {optSoundEffect->setCurrentText(value);}
    QString getSoundEffect() {return optSoundEffect->currentText();}
    void setLoop(bool value) {chkLoop->setChecked(value);}
    bool getLoop() {return chkLoop->isChecked();}
  protected slots:
    void on_btnPlaySoundEffect_clicked()
    {
      Globals::audio->setMedia(QUrl::fromLocalFile(QString("%1/sfx/%2.wav").arg(Globals::datadir).arg(optSoundEffect->currentText())));
      Globals::audio->play();
      if (Globals::audio->error() != QMediaPlayer::NoError)
        QMessageBox::critical(this, "Playback Failed", QString("Failed to play %1.ogg\nReason: %2").arg(optSoundEffect->currentText()).arg(Globals::audio->errorString()));
    }
};

#endif //CONFIGUREEVENT_PLAYSOUNDEFFECT_H
