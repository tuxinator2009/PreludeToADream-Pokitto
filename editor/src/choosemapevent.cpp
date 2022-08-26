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

#include <QButtonGroup>
#include "choosemapevent.h"
#include "mapevent.h"

ChooseMapEvent::ChooseMapEvent(bool allowElse, QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  buttons = new QButtonGroup(this);
  buttons->addButton(btnHideScreen, static_cast<int>(MapEvent::Event::Type::HideScreen));
  buttons->addButton(btnShowScreen, static_cast<int>(MapEvent::Event::Type::ShowScreen));
  buttons->addButton(btnTeleport, static_cast<int>(MapEvent::Event::Type::Teleport));
  buttons->addButton(btnShowDialog, static_cast<int>(MapEvent::Event::Type::ShowDialog));
  buttons->addButton(btnHideDialog, static_cast<int>(MapEvent::Event::Type::HideDialog));
  buttons->addButton(btnBufferMessage, static_cast<int>(MapEvent::Event::Type::BufferMessage));
  buttons->addButton(btnBufferValue, static_cast<int>(MapEvent::Event::Type::BufferValue));
  buttons->addButton(btnBufferCharacter, static_cast<int>(MapEvent::Event::Type::BufferCharacter));
  buttons->addButton(btnShowMessage, static_cast<int>(MapEvent::Event::Type::ShowMessage));
  buttons->addButton(btnShowOneLiner, static_cast<int>(MapEvent::Event::Type::ShowOneLiner));
  buttons->addButton(btnShowShopMenu, static_cast<int>(MapEvent::Event::Type::ShowShopMenu));
  buttons->addButton(btnShowGold, static_cast<int>(MapEvent::Event::Type::ShowGold));
  buttons->addButton(btnHideGold, static_cast<int>(MapEvent::Event::Type::HideGold));
  buttons->addButton(btnWaitButtons, static_cast<int>(MapEvent::Event::Type::WaitButtons));
  buttons->addButton(btnWaitFrames, static_cast<int>(MapEvent::Event::Type::WaitFrames));
  buttons->addButton(btnBattle, static_cast<int>(MapEvent::Event::Type::Battle));
  buttons->addButton(btnShakeScreen, static_cast<int>(MapEvent::Event::Type::ShakeScreen));
  buttons->addButton(btnCutscene, static_cast<int>(MapEvent::Event::Type::Cutscene));
  buttons->addButton(btnJump, static_cast<int>(MapEvent::Event::Type::Jump));
  buttons->addButton(btnIfSwitch, static_cast<int>(MapEvent::Event::Type::IfSwitch));
  buttons->addButton(btnIfVariable, static_cast<int>(MapEvent::Event::Type::IfVariable));
  buttons->addButton(btnIfHasItem, static_cast<int>(MapEvent::Event::Type::IfHasItem));
  buttons->addButton(btnIfExited, static_cast<int>(MapEvent::Event::Type::IfExited));
  buttons->addButton(btnIfYesNo, static_cast<int>(MapEvent::Event::Type::IfYesNo));
  buttons->addButton(btnIfValue, static_cast<int>(MapEvent::Event::Type::IfValue));
  buttons->addButton(btnIfFacing, static_cast<int>(MapEvent::Event::Type::IfFacing));
  buttons->addButton(btnPlaySoundEffect, static_cast<int>(MapEvent::Event::Type::PlaySoundEffect));
  buttons->addButton(btnPlayMusic, static_cast<int>(MapEvent::Event::Type::PlayMusic));
  buttons->addButton(btnPauseMusic, static_cast<int>(MapEvent::Event::Type::PauseMusic));
  buttons->addButton(btnResumeMusic, static_cast<int>(MapEvent::Event::Type::ResumeMusic));
  buttons->addButton(btnWaitMusic, static_cast<int>(MapEvent::Event::Type::WaitMusic));
  buttons->addButton(btnWaitSoundEffect, static_cast<int>(MapEvent::Event::Type::WaitSoundEffect));
  buttons->addButton(btnTurnSwitchOn, static_cast<int>(MapEvent::Event::Type::TurnSwitchOn));
  buttons->addButton(btnTurnSwitchOff, static_cast<int>(MapEvent::Event::Type::TurnSwitchOff));
  buttons->addButton(btnToggleSwitch, static_cast<int>(MapEvent::Event::Type::ToggleSwitch));
  buttons->addButton(btnTurnSwitchRangeOn, static_cast<int>(MapEvent::Event::Type::TurnSwitchRangeOn));
  buttons->addButton(btnTurnSwitchRangeOff, static_cast<int>(MapEvent::Event::Type::TurnSwitchRangeOff));
  buttons->addButton(btnToggleSwitchRange, static_cast<int>(MapEvent::Event::Type::ToggleSwitchRange));
  buttons->addButton(btnChangeVariable, static_cast<int>(MapEvent::Event::Type::ChangeVariable));
  buttons->addButton(btnChangeSprite, static_cast<int>(MapEvent::Event::Type::ChangeSprite));
  buttons->addButton(btnChangeEventLocation, static_cast<int>(MapEvent::Event::Type::ChangeEventLocation));
  buttons->addButton(btnChangeEventFlags, static_cast<int>(MapEvent::Event::Type::ChangeEventFlags));
  buttons->addButton(btnChangePassability, static_cast<int>(MapEvent::Event::Type::ChangePassability));
  buttons->addButton(btnGivePlayerItem, static_cast<int>(MapEvent::Event::Type::GivePlayerItem));
  buttons->addButton(btnGivePlayerGold, static_cast<int>(MapEvent::Event::Type::GivePlayerGold));
  buttons->addButton(btnHeal, static_cast<int>(MapEvent::Event::Type::Heal));
  buttons->addButton(btnShowImage, static_cast<int>(MapEvent::Event::Type::ShowImage));
  buttons->addButton(btnHideImage, static_cast<int>(MapEvent::Event::Type::HideImage));
  buttons->addButton(btnMovePlayer, static_cast<int>(MapEvent::Event::Type::MovePlayer));
  buttons->addButton(btnEndEventProcessing, static_cast<int>(MapEvent::Event::Type::EndEventProcessing));
  buttons->addButton(btnLabel, static_cast<int>(MapEvent::Event::Type::Label));
  buttons->addButton(btnComment, static_cast<int>(MapEvent::Event::Type::Comment));
  buttons->addButton(btnElse, static_cast<int>(MapEvent::Event::Type::Else));
  buttons->addButton(btnCancel, -1);
  btnElse->setEnabled(allowElse);
  connect(buttons, SIGNAL(buttonClicked(int)), this, SLOT(done(int)));
}

ChooseMapEvent::~ChooseMapEvent()
{
}
