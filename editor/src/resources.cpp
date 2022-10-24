/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2020 Justin (tuxinator2009) Davis                                *
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

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include "globals.h"
#include "resources.h"

Resources::Resources(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
  refreshMusic();
  refreshSFX();
  refreshBackdrops();
  lblMusicVolume->setMinimumWidth(lblMusicVolume->sizeHint().width());
  lblMusicVolume->setMaximumWidth(lblMusicVolume->sizeHint().height());
  lblSFXVolume->setMinimumWidth(lblSFXVolume->sizeHint().width());
  lblSFXVolume->setMaximumWidth(lblSFXVolume->sizeHint().height());
  playing = nullptr;
}

Resources::~Resources()
{
  Globals::audio->stop();
}

void Resources::on_tabWidget_currentChanged()
{
  if (playing != nullptr)
  {
    Globals::audio->stop();
    playing->setCheckState(Qt::Unchecked);
    playing = nullptr;
  }
}

void Resources::on_lstMusic_itemChanged(QListWidgetItem *item)
{
  if (item->checkState() == Qt::Unchecked && item == playing)
  {
    playing = nullptr;
    Globals::audio->stop();
  }
  else if (item->checkState() == Qt::Checked)
  {
    if (playing != nullptr)
    {
      Globals::audio->stop();
      playing->setCheckState(Qt::Unchecked);
    }
    playing = item;
    Globals::audio->setMedia(QUrl::fromLocalFile(QString("%1/music/%2.ogg").arg(Globals::datadir).arg(item->text())));
    Globals::audio->play();
    if (Globals::audio->error() != QMediaPlayer::NoError)
      QMessageBox::critical(this, "Playback Failed", QString("Failed to play %1.ogg\nReason: %2").arg(item->text()).arg(Globals::audio->errorString()));
  }
}

void Resources::on_sldMusicVolume_sliderMoved(int value)
{
  Globals::audio->setVolume(value);
  sldSFXVolume->setValue(value);
  lblMusicVolume->setText(QString("%1%").arg(value));
  lblSFXVolume->setText(QString("%1%").arg(value));
}

void Resources::on_lstSFX_itemChanged(QListWidgetItem *item)
{
  if (item->checkState() == Qt::Unchecked && item == playing)
  {
    playing = nullptr;
    Globals::audio->stop();
  }
  else if (item->checkState() == Qt::Checked)
  {
    if (playing != nullptr)
    {
      Globals::audio->stop();
      playing->setCheckState(Qt::Unchecked);
    }
    playing = item;
    Globals::audio->setMedia(QUrl::fromLocalFile(QString("%1/sfx/%2.wav").arg(Globals::datadir).arg(item->text())));
    Globals::audio->play();
    if (Globals::audio->error() != QMediaPlayer::NoError)
      QMessageBox::critical(this, "Playback Failed", QString("Failed to play %1.wav\nReason: %2").arg(item->text()).arg(Globals::audio->errorString()));
  }
}

void Resources::on_sldSFXVolume_sliderMoved(int value)
{
  Globals::audio->setVolume(value);
  sldMusicVolume->setValue(value);
  lblMusicVolume->setText(QString("%1%").arg(value));
  lblSFXVolume->setText(QString("%1%").arg(value));
}

void Resources::refreshMusic()
{
  Globals::refreshMusic();
  lstMusic->clear();
  for (int i = 0; i < Globals::bgms.count(); ++i)
  {
    QListWidgetItem *item = new QListWidgetItem(Globals::bgms[i]);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    item->setCheckState(Qt::Unchecked);
    lstMusic->addItem(item);
  }
}

void Resources::refreshSFX()
{
  Globals::refreshSFX();
  lstSFX->clear();
  for (int i = 0; i < Globals::sfx.count(); ++i)
  {
    QListWidgetItem *item = new QListWidgetItem(Globals::sfx[i]);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    item->setCheckState(Qt::Unchecked);
    lstSFX->addItem(item);
  }
}

void Resources::refreshBackdrops()
{
  Globals::refreshBackdrops();
  lstBackdrops->clear();
  for (int i = 0; i < Globals::backdrops.count(); ++i)
    lstBackdrops->addItem(new QListWidgetItem(QIcon(QPixmap(QString("%1/backdrops/%2.png").arg(Globals::datadir).arg(Globals::backdrops[i])).scaled(192, 128)), Globals::backdrops[i]));
}
