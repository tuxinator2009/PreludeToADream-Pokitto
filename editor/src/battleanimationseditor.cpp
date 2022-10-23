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

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTimer>
#include "battleanimation.h"
#include "battleanimationseditor.h"
#include "colorpicker.h"
#include "globals.h"
#include "image.h"

BattleAnimationsEditor::BattleAnimationsEditor(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  refreshBattleAnimations();
  Globals::refreshSFX();
  optSFX->addItems(Globals::sfx);
  Globals::refreshBackdrops();
  optBackdrop->addItems(Globals::backdrops);
  colorPicker = new ColorPicker(this);
  colorPicker->setMinimumSize(QSize(colorPicker->sizeHint().width(), colorPicker->sizeHint().width()));
  colorPicker->setMaximumSize(QSize(colorPicker->sizeHint().width(), colorPicker->sizeHint().width()));
  colorPicker->setVisible(false);
  connect(colorPicker, SIGNAL(colorClicked(int)), this, SLOT(colorPicker_colorClicked(int)));
  connect(Globals::animations, SIGNAL(imageChanged()), this, SLOT(animationsImageChanged()));
  for (int i = 0; i < 8; ++i)
  {
    numSprite[i] = wSprites->findChild<QSpinBox*>(QString("numSprite%1").arg(i));
    lblSprite[i] = wSprites->findChild<QLabel*>(QString("lblSprite%1").arg(i));
    connect(numSprite[i], SIGNAL(valueChanged(int)), this, SLOT(numSprite_valueChanged(int)));
  }
  animation = nullptr;
  animationImage = new Image(QSize(284, 240), this);
  imgBattleAnimation->setSize(QSize(568, 480));
  imgBattleAnimation->setImage(animationImage);
  selectedSprite = -1;
  offset = QPoint(0, 0);
}

BattleAnimationsEditor::~BattleAnimationsEditor()
{
  if (animation != nullptr)
    delete animation;
  Globals::audio->stop();
}

void BattleAnimationsEditor::on_btnNewAnimation_clicked()
{
  QString location = QInputDialog::getText(this, "New Battle Animation", "File Name:");
  if (!location.isEmpty())
  {
    if (lstAnimations->findItems(location, Qt::MatchFixedString).count() > 0)
    {
      QMessageBox::critical(this, "Animation Already Exists", "The specified animation file already exists.");
      return;
    }
    if (animation != nullptr)
      delete animation;
    animation = new BattleAnimation(location);
    animation->saveAnimation();
    lstAnimations->addItem(location);
    lstAnimations->setCurrentRow(lstAnimations->count() - 1);
    lstAnimations->item(lstAnimations->count() - 1)->setSelected(true);
    lstAnimations->sortItems();
  }
}

void BattleAnimationsEditor::on_btnSaveAnimation_clicked()
{
  if (animation != nullptr)
    animation->saveAnimation();
}

void BattleAnimationsEditor::on_btnDeleteAnimation_clicked()
{
  if (QFile::remove(QString("%1/animations/%2.xml").arg(Globals::datadir).arg(lstAnimations->currentItem()->text())))
  {
    QMessageBox::critical(this, "Error Deleting Animation", QString("Error: failed to delete %1.xml").arg(lstAnimations->currentItem()->text()));
    return;
  }
  delete lstAnimations->takeItem(lstAnimations->currentRow());
  delete animation;
  animation = nullptr;
}

void BattleAnimationsEditor::on_lstAnimations_itemSelectionChanged()
{
  if (animation == nullptr)
  {
    delete animation;
    animation = nullptr;
  }
  if (lstAnimations->count() == 0)
    return;
  animation = new BattleAnimation(lstAnimations->currentItem()->text());
  animation->loadAnimation();
  lstFrames->clear();
  lstEffects->clear();
  for (int i = 0; i < animation->numFrames(); ++i)
    lstFrames->addItem(QString("<%1>").arg(i));
  for (int i = 0; i < 8; ++i)
  {
    int value = animation->getSpriteID(i);
    sprites[i] = QRect(value % 16 * 32, value / 16 * 32, 32, 32);
    numSprite[i]->setValue(value);
    lblSprite[i]->setPixmap(QPixmap::fromImage(Globals::animations->toQImage(sprites[i]).scaled(64, 64)));
  }
  if (animation->numFrames() == 1)
    btnDeleteFrame->setEnabled(false);
  else if (animation->numFrames() == 51)
    btnAddFrame->setEnabled(false);
  lstFrames->setCurrentRow(0, QItemSelectionModel::SelectCurrent);
  renderAnimationFrame(0, false);
}

void BattleAnimationsEditor::numSprite_valueChanged(int value)
{
  int sprite = sender()->objectName().back().toLatin1() - '0';
  animation->setSpriteID(sprite, value);
  sprites[sprite] = QRect(value % 16 * 32, value / 16 * 32, 32, 32);
  lblSprite[sprite]->setPixmap(QPixmap::fromImage(Globals::animations->toQImage(sprites[sprite]).scaled(64, 64)));
}

void BattleAnimationsEditor::on_optBackdrop_currentIndexChanged(int index)
{
  Q_UNUSED(index);
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_numMonster_valueChanged(int value)
{
  Q_UNUSED(value);
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_lstFrames_itemSelectionChanged()
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  int frame = lstFrames->currentRow();
  lstEffects->clear();
  for (int i = 0; i < animation->numEffects(frame); ++i)
  {
    BattleAnimation::Effect *effect = animation->getEffect(frame, i);
    if (effect->effect == BattleAnimation::EFFECT_SPRITE)
      lstEffects->addItem("<Sprite>");
    else if (effect->effect == BattleAnimation::EFFECT_PLAY_SFX)
      lstEffects->addItem("<Play SFX>");
    else if (effect->effect == BattleAnimation::EFFECT_SHAKE_SCREEN)
      lstEffects->addItem("<Shake Screen>");
    else if (effect->effect == BattleAnimation::EFFECT_FLASH_UI)
      lstEffects->addItem("<Flash UI>");
    else if (effect->effect == BattleAnimation::EFFECT_FLASH_BATTLER)
      lstEffects->addItem("<Flash Battler>");
    else if (effect->effect == BattleAnimation::EFFECT_FLASH_PLAYER)
      lstEffects->addItem("<Flash Player>");
  }
  if (animation->numEffects(frame) == 0)
    btnDeleteEffect->setEnabled(false);
  else if (animation->numEffects(frame) == 8)
    btnAddEffect->setEnabled(false);
  if (animation->numEffects(frame) > 0)
    lstEffects->setCurrentRow(0, QItemSelectionModel::SelectCurrent);
  renderAnimationFrame(frame, false);
}

void BattleAnimationsEditor::on_btnDeleteFrame_clicked()
{
  int row = lstFrames->currentRow();
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  animation->deleteFrame(lstFrames->currentRow());
  delete lstFrames->takeItem(lstFrames->currentRow());
  if (row >= lstFrames->count())
    row = lstFrames->count() - 1;
  lstFrames->setCurrentRow(row, QItemSelectionModel::SelectCurrent);
  btnAddFrame->setEnabled(true);
  if (animation->numFrames() == 1)
    btnDeleteFrame->setEnabled(false);
}

void BattleAnimationsEditor::on_btnAddFrame_clicked()
{
  if (animation == nullptr)
    return;
  lstFrames->addItem(QString("<%1>").arg(animation->numFrames()));
  animation->addFrame();
  lstFrames->setCurrentRow(lstFrames->count() - 1, QItemSelectionModel::SelectCurrent);
  if (animation->numFrames() == 51)
    btnAddFrame->setEnabled(false);
  btnDeleteFrame->setEnabled(true);
}

void BattleAnimationsEditor::on_btnPlayAnimation_clicked()
{
  animationData.frame = 0;
  animationData.xOffset = 0;
  animationData.yOffset = 0;
  animationData.uiColor = 7;
  for (int i = 0; i < 3; ++i)
  {
    animationData.battlerColors[i] = 0;
    animationData.playerColors[i] = 0;
  }
  animationData.shakeScreen = 0;
  animationData.shakeRate = 0;
  animationData.flashBattler = 0;
  animationData.flashPlayer = 0;
  animationData.flashUI = 0;
  frmAnimations->setEnabled(false);
  frmSprites->setEnabled(false);
  wEffects->setEnabled(false);
  animationNextFrame();
}

void BattleAnimationsEditor::on_lstEffects_itemSelectionChanged()
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  BattleAnimation::Effect *effect = animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow());
  stkEffect->setCurrentIndex(effect->effect);
  if (effect->effect == BattleAnimation::EFFECT_SPRITE)
  {
    BattleAnimation::EffectSprite *sprite = static_cast<BattleAnimation::EffectSprite*>(effect);
    numSpriteID->setValue(sprite->spriteID);
    btnSpriteFlipped->setChecked(sprite->flipped);
    btnSpriteMirrored->setChecked(sprite->mirrored);
    numSpriteX->setValue(sprite->x);
    numSpriteY->setValue(sprite->y);
  }
  else if (effect->effect == BattleAnimation::EFFECT_PLAY_SFX)
  {
    BattleAnimation::EffectPlaySFX *playSFX = static_cast<BattleAnimation::EffectPlaySFX*>(effect);
    optSFX->setCurrentText(playSFX->sfx);
  }
  else if (effect->effect == BattleAnimation::EFFECT_SHAKE_SCREEN)
  {
    BattleAnimation::EffectShakeScreen *shakeScreen = static_cast<BattleAnimation::EffectShakeScreen*>(effect);
    numShakeDuration->setValue(shakeScreen->duration);
    numShakeRate->setValue(shakeScreen->rate);
  }
  else if (effect->effect == BattleAnimation::EFFECT_FLASH_UI)
  {
    BattleAnimation::EffectFlashUI *flashUI = static_cast<BattleAnimation::EffectFlashUI*>(effect);
    QRgb color = Image::palette[flashUI->color];
    numFlashUIDuration->setValue(flashUI->duration);
    btnFlashUIColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
  }
  else if (effect->effect == BattleAnimation::EFFECT_FLASH_BATTLER)
  {
    BattleAnimation::EffectFlashBattler *flashBattler = static_cast<BattleAnimation::EffectFlashBattler*>(effect);
    QRgb color = Image::palette[flashBattler->color1];
    numFlashEnemyDuration->setValue(flashBattler->duration);
    btnFlashEnemyColor1->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
    color = Image::palette[flashBattler->color2];
    btnFlashEnemyColor2->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
    color = Image::palette[flashBattler->color3];
    btnFlashEnemyColor3->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
  }
  else if (effect->effect == BattleAnimation::EFFECT_FLASH_PLAYER)
  {
    BattleAnimation::EffectFlashPlayer *flashPlayer = static_cast<BattleAnimation::EffectFlashPlayer*>(effect);
    QRgb color = Image::palette[flashPlayer->color1];
    numFlashPlayerDuration->setValue(flashPlayer->duration);
    btnFlashPlayerColor1->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
    color = Image::palette[flashPlayer->color2];
    btnFlashPlayerColor2->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
    color = Image::palette[flashPlayer->color3];
    btnFlashPlayerColor3->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
  }
}

void BattleAnimationsEditor::on_btnDeleteEffect_clicked()
{
  int row = lstEffects->currentRow();
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  animation->deleteEffect(lstFrames->currentRow(), lstEffects->currentRow());
  delete lstEffects->takeItem(row);
  if (row >= lstEffects->count() - 1)
    row = lstEffects->count() - 1;
  if (row > 0)
    lstEffects->setCurrentRow(row, QItemSelectionModel::SelectCurrent);
  if (animation->numEffects(lstFrames->currentRow()) == 0)
    btnDeleteEffect->setEnabled(false);
  btnAddEffect->setEnabled(true);
}

void BattleAnimationsEditor::on_btnAddEffect_clicked()
{
  QString effect;
  bool ok;
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  effect = QInputDialog::getItem(this, "Animation Effect", "", QStringList() << "Sprite" << "Play SFX" << "Shake Screen" << "Flash UI" << "Flash Battler" << "Flash Player", 0, false, &ok);
  if (!ok)
    return;
  if (effect == "Sprite")
  {
    BattleAnimation::EffectSprite *sprite = new BattleAnimation::EffectSprite;
    sprite->effect = BattleAnimation::EFFECT_SPRITE;
    sprite->spriteID = 0;
    sprite->flipped = false;
    sprite->mirrored = false;
    sprite->x = 0;
    sprite->y = 0;
    animation->addEffect(lstFrames->currentRow(), sprite);
    lstEffects->addItem("<Sprite>");
  }
  else if (effect == "Play SFX")
  {
    BattleAnimation::EffectPlaySFX *playSFX = new BattleAnimation::EffectPlaySFX;
    playSFX->effect = BattleAnimation::EFFECT_PLAY_SFX;
    playSFX->sfx = Globals::sfx[0];
    animation->addEffect(lstFrames->currentRow(), playSFX);
    lstEffects->addItem("<Play SFX>");
  }
  else if (effect == "Shake Screen")
  {
    BattleAnimation::EffectShakeScreen *shakeScreen = new BattleAnimation::EffectShakeScreen;
    shakeScreen->effect = BattleAnimation::EFFECT_SHAKE_SCREEN;
    shakeScreen->duration = 0;
    shakeScreen->rate = 0;
    animation->addEffect(lstFrames->currentRow(), shakeScreen);
    lstEffects->addItem("<Shake Screen>");
  }
  else if (effect == "Flash UI")
  {
    BattleAnimation::EffectFlashUI *flashUI = new BattleAnimation::EffectFlashUI;
    flashUI->effect = BattleAnimation::EFFECT_FLASH_UI;
    flashUI->duration = 0;
    flashUI->color = 0;
    animation->addEffect(lstFrames->currentRow(), flashUI);
    lstEffects->addItem("<Flash UI>");
  }
  else if (effect == "Flash Battler")
  {
    BattleAnimation::EffectFlashBattler *flashBattler = new BattleAnimation::EffectFlashBattler;
    flashBattler->effect = BattleAnimation::EFFECT_FLASH_BATTLER;
    flashBattler->duration = 0;
    flashBattler->color1 = 0;
    flashBattler->color2 = 0;
    flashBattler->color3 = 0;
    animation->addEffect(lstFrames->currentRow(), flashBattler);
    lstEffects->addItem("<Flash Battler>");
  }
  else if (effect == "Flash Player")
  {
    BattleAnimation::EffectFlashPlayer *flashPlayer = new BattleAnimation::EffectFlashPlayer;
    flashPlayer->effect = BattleAnimation::EFFECT_FLASH_BATTLER;
    flashPlayer->duration = 0;
    flashPlayer->color1 = 0;
    flashPlayer->color2 = 0;
    flashPlayer->color3 = 0;
    animation->addEffect(lstFrames->currentRow(), flashPlayer);
    lstEffects->addItem("<Flash Player>");
  }
  lstEffects->setCurrentRow(lstEffects->count() - 1, QItemSelectionModel::SelectCurrent);
  if (lstEffects->count() == 8)
    btnAddEffect->setEnabled(false);
  btnDeleteEffect->setEnabled(true);
}

void BattleAnimationsEditor::on_numSpriteID_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectSprite*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->spriteID = value;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_btnSpriteFlipped_toggled(bool on)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectSprite*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->flipped = on;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_btnSpriteMirrored_toggled(bool on)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectSprite*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->mirrored = on;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_numSpriteX_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectSprite*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->x = value;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_numSpriteY_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectSprite*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->y = value;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_optSFX_currentIndexChanged(QString value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectPlaySFX*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->sfx = value;
}

void BattleAnimationsEditor::on_btnPlaySFX_clicked()
{
  Globals::audio->setMedia(QUrl::fromLocalFile(QString("%1/sfx/%2.wav").arg(Globals::datadir).arg(optSFX->currentText())));
  Globals::audio->play();
  if (Globals::audio->error() != QMediaPlayer::NoError)
    QMessageBox::critical(this, "Playback Failed", QString("Failed to play %1.ogg\nReason: %2").arg(optSFX->currentText()).arg(Globals::audio->errorString()));
}

void BattleAnimationsEditor::on_sldAnimationVolume_sliderMoved(int value)
{
  Globals::audio->setVolume(value);
  lblAnimationVolume->setText(QString("%1%").arg(value));
}

void BattleAnimationsEditor::on_numShakeDuration_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectShakeScreen*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->duration = value;
}

void BattleAnimationsEditor::on_numShakeRate_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectShakeScreen*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->rate = value;
}

void BattleAnimationsEditor::on_numFlashUIDuration_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectFlashUI*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->duration = value;
}

void BattleAnimationsEditor::on_btnFlashUIColor_clicked()
{
  colorPickerButton = btnFlashUIColor;
  colorPicker->move(colorPickerButton->mapTo(this, QPoint(colorPickerButton->width() - colorPicker->width(), 0)));
  colorPicker->show();
}

void BattleAnimationsEditor::on_numFlashEnemyDuration_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectFlashBattler*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->duration = value;
}

void BattleAnimationsEditor::on_btnFlashEnemyColor1_clicked()
{
  colorPickerButton = btnFlashEnemyColor1;
  colorPicker->move(colorPickerButton->mapTo(this, QPoint(colorPickerButton->width() - colorPicker->width(), 0)));
  colorPicker->show();
}

void BattleAnimationsEditor::on_btnFlashEnemyColor2_clicked()
{
  colorPickerButton = btnFlashEnemyColor2;
  colorPicker->move(colorPickerButton->mapTo(this, QPoint(colorPickerButton->width() - colorPicker->width(), 0)));
  colorPicker->show();
}

void BattleAnimationsEditor::on_btnFlashEnemyColor3_clicked()
{
  colorPickerButton = btnFlashEnemyColor3;
  colorPicker->move(colorPickerButton->mapTo(this, QPoint(colorPickerButton->width() - colorPicker->width(), 0)));
  colorPicker->show();
}

void BattleAnimationsEditor::on_numFlashPlayerDuration_valueChanged(int value)
{
  if (animation == nullptr)
    return;
  if (lstFrames->count() == 0)
    return;
  if (lstFrames->currentRow() < 0 || lstFrames->currentRow() >= animation->numFrames())
    return;
  if (lstEffects->count() == 0)
    return;
  if (lstEffects->currentRow() < 0 || lstEffects->currentRow() >= animation->numEffects(lstFrames->currentRow()))
    return;
  static_cast<BattleAnimation::EffectFlashPlayer*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->duration = value;
}

void BattleAnimationsEditor::on_btnFlashPlayerColor1_clicked()
{
  colorPickerButton = btnFlashPlayerColor1;
  colorPicker->move(colorPickerButton->mapTo(this, QPoint(colorPickerButton->width() - colorPicker->width(), 0)));
  colorPicker->show();
}

void BattleAnimationsEditor::on_btnFlashPlayerColor2_clicked()
{
  colorPickerButton = btnFlashPlayerColor2;
  colorPicker->move(colorPickerButton->mapTo(this, QPoint(colorPickerButton->width() - colorPicker->width(), 0)));
  colorPicker->show();
}

void BattleAnimationsEditor::on_btnFlashPlayerColor3_clicked()
{
  colorPickerButton = btnFlashPlayerColor3;
  colorPicker->move(colorPickerButton->mapTo(this, QPoint(colorPickerButton->width() - colorPicker->width(), 0)));
  colorPicker->show();
}

void BattleAnimationsEditor::colorPicker_colorClicked(int index)
{
  QRgb color = Image::palette[index];
  colorPickerButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color)));
  colorPicker->hide();
  if (colorPickerButton == btnFlashUIColor)
    static_cast<BattleAnimation::EffectFlashUI*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->color = index;
  else if (colorPickerButton == btnFlashEnemyColor1)
    static_cast<BattleAnimation::EffectFlashBattler*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->color1 = index;
  else if (colorPickerButton == btnFlashEnemyColor2)
    static_cast<BattleAnimation::EffectFlashBattler*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->color2 = index;
  else if (colorPickerButton == btnFlashEnemyColor3)
    static_cast<BattleAnimation::EffectFlashBattler*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->color3 = index;
  else if (colorPickerButton == btnFlashPlayerColor1)
    static_cast<BattleAnimation::EffectFlashPlayer*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->color1 = index;
  else if (colorPickerButton == btnFlashPlayerColor2)
    static_cast<BattleAnimation::EffectFlashPlayer*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->color2 = index;
  else if (colorPickerButton == btnFlashPlayerColor3)
    static_cast<BattleAnimation::EffectFlashPlayer*>(animation->getEffect(lstFrames->currentRow(), lstEffects->currentRow()))->color3 = index;
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::on_imgBattleAnimation_mousePressed(Qt::MouseButton button, QPoint pos)
{
  if (button == Qt::LeftButton)
  {
    int frame = lstFrames->currentRow();
    selectedSprite = -1;
    pos /= 2;
    for (int i = 0; i < animation->numEffects(frame); ++i)
    {
      BattleAnimation::Effect *effect = animation->getEffect(frame, i);
      if (effect->effect == BattleAnimation::EFFECT_SPRITE)
      {
        BattleAnimation::EffectSprite *sprite = static_cast<BattleAnimation::EffectSprite*>(effect);
        QRect rect = QRect(sprite->x, sprite->y, 32, 32);
        if (rect.contains(pos))
        {
          offset = QPoint(pos.x() - sprite->x, pos.y() - sprite->y);
          selectedSprite = i;
          lstEffects->setCurrentRow(i);
          lstEffects->item(i)->setSelected(true);
        }
      }
    }
  }
}

void BattleAnimationsEditor::on_imgBattleAnimation_mouseMoved(Qt::MouseButtons buttons, QPoint pos)
{
  pos /= 2;
  if ((buttons & Qt::LeftButton) != 0 && selectedSprite != -1)
  {
    BattleAnimation::EffectSprite *sprite = static_cast<BattleAnimation::EffectSprite*>(animation->getEffect(lstFrames->currentRow(), selectedSprite));
    int x = pos.x() - offset.x();
    int y = pos.y() - offset.y();
    if (x < 0)
      x = 0;
    else if (x > 252)
      x = 252;
    if (y < 0)
      y = 0;
    else if (y > 208)
      y = 208;
    sprite->x = x;
    sprite->y = y;
    numSpriteX->setValue(x);
    numSpriteY->setValue(y);
    renderAnimationFrame(lstFrames->currentRow(), false);
  }
}

void BattleAnimationsEditor::on_imgBattleAnimation_mouseReleased(Qt::MouseButton button, QPoint pos)
{
  Q_UNUSED(button);
  Q_UNUSED(pos);
  selectedSprite = -1;
}

void BattleAnimationsEditor::animationsImageChanged()
{
  for (int i = 0; i < 8; ++i)
  {
    int value = animation->getSpriteID(i);
    lblSprite[i]->setPixmap(QPixmap::fromImage(Globals::animations->toQImage(QRect(value % 16 * 32, value / 16 * 32, 32, 32)).scaled(64, 64)));
  }
  renderAnimationFrame(lstFrames->currentRow(), false);
}

void BattleAnimationsEditor::animationNextFrame()
{
  int frame = animationData.frame / 3;
  for (int i = 0; i < animation->numEffects(frame); ++i)
  {
    auto effect = animation->getEffect(frame, i);
    if (effect->effect == BattleAnimation::EFFECT_PLAY_SFX)
    {
      BattleAnimation::EffectPlaySFX *playSFX = static_cast<BattleAnimation::EffectPlaySFX*>(effect);
      if (animationData.frame % 3 == 0)
      {
        Globals::audio->setMedia(QUrl::fromLocalFile(QString("%1/sfx/%2.wav").arg(Globals::datadir).arg(playSFX->sfx)));
        Globals::audio->play();
      }
    }
    else if (effect->effect == BattleAnimation::EFFECT_SHAKE_SCREEN)
    {
      BattleAnimation::EffectShakeScreen *shakeScreen = static_cast<BattleAnimation::EffectShakeScreen*>(effect);
      animationData.shakeScreen = shakeScreen->duration;
      animationData.shakeRate = shakeScreen->rate;
      animationData.xOffset = QRandomGenerator::global()->bounded(-6, 3);
      animationData.yOffset = QRandomGenerator::global()->bounded(-8, 1);
    }
    else if (effect->effect == BattleAnimation::EFFECT_FLASH_UI)
    {
      BattleAnimation::EffectFlashUI *flashUI = static_cast<BattleAnimation::EffectFlashUI*>(effect);
      animationData.flashUI = flashUI->duration;
      animationData.uiColor = flashUI->color;
    }
    else if (effect->effect == BattleAnimation::EFFECT_FLASH_BATTLER)
    {
      BattleAnimation::EffectFlashBattler *flashBattler = static_cast<BattleAnimation::EffectFlashBattler*>(effect);
      animationData.flashBattler = flashBattler->duration;
      animationData.battlerColors[0] = flashBattler->color1;
      animationData.battlerColors[1] = flashBattler->color2;
      animationData.battlerColors[2] = flashBattler->color3;
    }
    else if (effect->effect == BattleAnimation::EFFECT_FLASH_PLAYER)
    {
      BattleAnimation::EffectFlashPlayer *flashPlayer = static_cast<BattleAnimation::EffectFlashPlayer*>(effect);
      animationData.flashPlayer = flashPlayer->duration;
      animationData.playerColors[0] = flashPlayer->color1;
      animationData.playerColors[1] = flashPlayer->color2;
      animationData.playerColors[2] = flashPlayer->color3;
    }
  }
  if (animationData.shakeScreen > 0 && animationData.frame % animationData.shakeRate == 0)
  {
    --animationData.shakeScreen;
    if (animationData.shakeScreen == 0)
    {
      animationData.xOffset = 0;
      animationData.yOffset = 0;
    }
    else
    {
      animationData.xOffset = QRandomGenerator::global()->bounded(-6, 3);
      animationData.yOffset = QRandomGenerator::global()->bounded(-8, 1);
    }
  }
  if (animationData.flashUI > 0)
    --animationData.flashUI;
  if (animationData.flashBattler > 0)
    --animationData.flashBattler;
  if (animationData.flashPlayer > 0)
    --animationData.flashPlayer;
  renderAnimationFrame(frame, true);
  ++animationData.frame;
  if (animationData.frame < animation->numFrames() * 3)
    QTimer::singleShot(30, this, SLOT(animationNextFrame()));
  else
  {
    frmAnimations->setEnabled(true);
    frmSprites->setEnabled(true);
    wEffects->setEnabled(true);
    renderAnimationFrame(lstFrames->currentRow(), false);
  }
}

void BattleAnimationsEditor::refreshBattleAnimations()
{
  QDir folder(QString("%1/animations").arg(Globals::datadir));
  QStringList animations = folder.entryList(QStringList() << "*.anim", QDir::Files, QDir::Name);
  animations.replaceInStrings(".anim", "", Qt::CaseInsensitive);
  lstAnimations->clear();
  lstAnimations->addItems(animations);
}

void BattleAnimationsEditor::renderAnimationFrame(int frame, bool playing)
{
  Image backdrop(QString("%1/backdrops/%2.png").arg(Globals::datadir).arg(optBackdrop->currentText()), nullptr, false);
  Image battler(*Globals::battlers, QRect((numMonster->value() % 16) * 48, (numMonster->value() / 16) * 48, 48, 48));
  Image player(*Globals::player, QRect(16, 0, 16, 16));
  Image battle(":/images/battle.png", nullptr, false);
  Image frameNumbers(":/images/frameNumber.png", nullptr, false);
  animationImage->fillRect(QRect(0, 0, 284, 240), 2);
  if (playing)
  {
    if (animationData.flashUI > 0)
      battle.recolor(7, animationData.uiColor);
    if (animationData.flashBattler > 0)
      battler.recolor(animationData.battlerColors[0], animationData.battlerColors[1], animationData.battlerColors[2]);
    if (animationData.flashPlayer > 0)
      player.recolor(animationData.playerColors[0], animationData.playerColors[1], animationData.playerColors[2]);
    animationImage->blitImage(battle, QRect(0, 0, 220, 176), QPoint(32 + animationData.xOffset, 32 + animationData.yOffset), false);
    animationImage->drawRect(QRect(31, 31, 222, 178), 0);
    animationImage->blitImage(backdrop, QRect(0, 0, 96, 64), QPoint(142 + animationData.xOffset, 80 + animationData.yOffset), false);
    animationImage->blitImage(battler, QRect(0, 0, 48, 48), QPoint(166, 84), true);
    animationImage->blitImage(player, QRect(0, 0, 16, 16), QPoint(182, 127), true);
    for (int i = 0; i < animation->numEffects(frame); ++i)
    {
      auto effect = animation->getEffect(frame, i);
      if (effect->effect == BattleAnimation::EFFECT_SPRITE)
      {
        BattleAnimation::EffectSprite *sprite = static_cast<BattleAnimation::EffectSprite*>(effect);
        animationImage->blitImage(*Globals::animations, sprites[sprite->spriteID], QPoint(sprite->x, sprite->y), true, sprite->flipped, sprite->mirrored);
      }
    }
  }
  else
  {
    for (int i = 0; i < animation->numEffects(frame); ++i)
    {
      auto effect = animation->getEffect(frame, i);
      if (effect->effect == BattleAnimation::EFFECT_FLASH_UI)
        battle.recolor(7, static_cast<BattleAnimation::EffectFlashUI*>(effect)->color);
      else if (effect->effect == BattleAnimation::EFFECT_FLASH_BATTLER)
      {
        BattleAnimation::EffectFlashBattler *flashBattler = static_cast<BattleAnimation::EffectFlashBattler*>(effect);
        battler.recolor(flashBattler->color1, flashBattler->color2, flashBattler->color3);
      }
      else if (effect->effect == BattleAnimation::EFFECT_FLASH_PLAYER)
      {
        BattleAnimation::EffectFlashPlayer *flashPlayer = static_cast<BattleAnimation::EffectFlashPlayer*>(effect);
        player.recolor(flashPlayer->color1, flashPlayer->color2, flashPlayer->color3);
      }
    }
    animationImage->blitImage(battle, QRect(0, 0, 220, 176), QPoint(32, 32), false);
    animationImage->drawRect(QRect(31, 31, 222, 178), 0);
    animationImage->blitImage(backdrop, QRect(0, 0, 96, 64), QPoint(142, 80), false);
    animationImage->blitImage(battler, QRect(0, 0, 48, 48), QPoint(166, 84), true);
    animationImage->blitImage(player, QRect(0, 0, 16, 16), QPoint(182, 127), true);
    if (frame > 0)
    {
      for (int i = 0; i < animation->numEffects(frame - 1); ++i)
      {
        auto effect = animation->getEffect(frame - 1, i);
        if (effect->effect == BattleAnimation::EFFECT_SPRITE)
        {
          BattleAnimation::EffectSprite *sprite = static_cast<BattleAnimation::EffectSprite*>(effect);
          animationImage->drawRect(QRect(sprite->x - 1, sprite->y - 1, 34, 34), 231);
          animationImage->blitImage(frameNumbers, QRect(i * 5, 7, 5, 7), QPoint(sprite->x - 1, sprite->y - 8), false);
        }
      }
    }
    for (int i = 0; i < animation->numEffects(frame); ++i)
    {
      auto effect = animation->getEffect(frame, i);
      if (effect->effect == BattleAnimation::EFFECT_SPRITE)
      {
        BattleAnimation::EffectSprite *sprite = static_cast<BattleAnimation::EffectSprite*>(effect);
        animationImage->blitImage(*Globals::animations, sprites[sprite->spriteID], QPoint(sprite->x, sprite->y), true, sprite->flipped, sprite->mirrored);
        animationImage->drawRect(QRect(sprite->x - 1, sprite->y - 1, 34, 34), 175);
        animationImage->blitImage(frameNumbers, QRect(i * 5, 0, 5, 7), QPoint(sprite->x - 1, sprite->y - 8), false);
      }
    }
  }
  imgBattleAnimation->imageChanged();
}
