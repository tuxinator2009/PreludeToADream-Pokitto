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

#ifndef BATTLEANIMATIONSEDITOR_H
#define BATTLEANIMATIONSEDITOR_H

#include "battleanimation.h"
#include "colorpicker.h"
#include "image.h"
#include "ui_battleanimationseditor.h"

class BattleAnimationsEditor : public QDialog, Ui::BattleAnimationsEditor
{
  Q_OBJECT
  public:
    BattleAnimationsEditor(QWidget *parent=nullptr);
    ~BattleAnimationsEditor();
  protected slots:
    void on_btnNewAnimation_clicked();
    void on_btnSaveAnimation_clicked();
    void on_btnDeleteAnimation_clicked();
    void on_lstAnimations_itemSelectionChanged();
    void numSprite_valueChanged(int value);
    void on_optBackdrop_currentIndexChanged(int index);
    void on_numMonster_valueChanged(int value);
    void on_lstFrames_itemSelectionChanged();
    void on_btnDeleteFrame_clicked();
    void on_btnAddFrame_clicked();
    void on_btnPlayAnimation_clicked();
    void on_lstEffects_itemSelectionChanged();
    void on_btnDeleteEffect_clicked();
    void on_btnAddEffect_clicked();
    void on_numSpriteID_valueChanged(int value);
    void on_btnSpriteFlipped_toggled(bool on);
    void on_btnSpriteMirrored_toggled(bool on);
    void on_numSpriteX_valueChanged(int value);
    void on_numSpriteY_valueChanged(int value);
    void on_optSFX_currentIndexChanged(QString value);
    void on_btnPlaySFX_clicked();
    void on_sldAnimationVolume_sliderMoved(int value);
    void on_numShakeDuration_valueChanged(int value);
    void on_numShakeRate_valueChanged(int value);
    void on_numFlashUIDuration_valueChanged(int value);
    void on_btnFlashUIColor_clicked();
    void on_numFlashEnemyDuration_valueChanged(int value);
    void on_btnFlashEnemyColor1_clicked();
    void on_btnFlashEnemyColor2_clicked();
    void on_btnFlashEnemyColor3_clicked();
    void on_numFlashPlayerDuration_valueChanged(int value);
    void on_btnFlashPlayerColor1_clicked();
    void on_btnFlashPlayerColor2_clicked();
    void on_btnFlashPlayerColor3_clicked();
    void colorPicker_colorClicked(int index);
    void on_imgBattleAnimation_mousePressed(Qt::MouseButton button, QPoint pos);
    void on_imgBattleAnimation_mouseMoved(Qt::MouseButtons buttons, QPoint pos);
    void on_imgBattleAnimation_mouseReleased(Qt::MouseButton button, QPoint pos);
    void animationsImageChanged();
    void animationNextFrame();
  private:
    void refreshBattleAnimations();
    void renderAnimationFrame(int frame, bool playing);
    struct AnimationData
    {
      int frame;
      int xOffset;
      int yOffset;
      uint8_t uiColor;
      uint8_t battlerColors[3];
      uint8_t playerColors[3];
      uint8_t shakeScreen;
      uint8_t shakeRate;
      uint8_t flashBattler;
      uint8_t flashPlayer;
      uint8_t flashUI;
    };
    QToolButton *colorPickerButton;
    QSpinBox *numSprite[8];
    QLabel *lblSprite[8];
    QRect sprites[8];
    BattleAnimation *animation;
    ColorPicker *colorPicker;
    Image *animationImage;
    AnimationData animationData;
    QPoint offset;
    int selectedSprite;
};

#endif //BATTLEANIMATIONSEDITOR_H
