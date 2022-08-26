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

#ifndef CONFIGUREEVENT_BATTLE_H
#define CONFIGUREEVENT_BATTLE_H

#include <QMessageBox>
#include "ui_configureevent_battle.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"

class ConfigureEvent_Battle : public QDialog, public Ui::ConfigureEvent_Battle
{
  Q_OBJECT
  public:
    ConfigureEvent_Battle(QWidget *parent=nullptr) : QDialog(parent)
    {
      preview = new Image(QSize(96, 64));
      setupUi(this);
      optBackground->addItems(Globals::backdrops);
      optMusic->addItems(Globals::bgms);
      connect(optBackground, SIGNAL(currentTextChanged(QString)), this, SLOT(updatePreview()));
      connect(numMonster, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
      gfxPreview->setSize(QSize(96*4, 64*4));
      gfxPreview->setImage(preview);
      gfxPreview->setTransparent(false);
    }
    ~ConfigureEvent_Battle()
    {
      Globals::audio->stop();
      delete preview;
    }
    void setBackground(QString value) {optBackground->setCurrentText(value);}
    QString getBackground() {return optBackground->currentText();}
    void setMusic(QString value) {optMusic->setCurrentText(value);}
    QString getMusic() {return optMusic->currentText();}
    void setMonster(int value) {numMonster->setValue(value);}
    int getMonster() {return numMonster->value();}
  public slots:
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
    void updatePreview()
    {
      Image bg(QString("%1/backdrops/%2.png").arg(Globals::datadir).arg(optBackground->currentText()), this, false);
      int monsterID = numMonster->value();
      preview->blitImage(bg, QRect(0, 0, 96, 64), QPoint(0, 0), false);
      preview->blitImage(*Globals::battlers, QRect((monsterID % 16) * 48, (monsterID / 16) * 48, 48, 48), QPoint(24, 4), true);
      gfxPreview->imageChanged();
    }
  private:
    Image *preview;
};

#endif //CONFIGUREEVENT_BATTLE
