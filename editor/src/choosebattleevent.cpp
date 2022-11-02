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
#include "battleevent.h"
#include "choosebattleevent.h"

ChooseBattleEvent::ChooseBattleEvent(bool allowElse, QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  buttons = new QButtonGroup(this);
  buttons->addButton(btnShakeScreen, static_cast<int>(BattleEvent::Event::Type::ShakeScreen));
  buttons->addButton(btnFlashBattler, static_cast<int>(BattleEvent::Event::Type::FlashBattler));
  buttons->addButton(btnFlashUI, static_cast<int>(BattleEvent::Event::Type::FlashUi));
  buttons->addButton(btnBasicAttack, static_cast<int>(BattleEvent::Event::Type::BasicAttack));
  buttons->addButton(btnCastSpell, static_cast<int>(BattleEvent::Event::Type::CastSpell));
  buttons->addButton(btnPlaySoundEffect, static_cast<int>(BattleEvent::Event::Type::PlaySoundEffect));
  buttons->addButton(btnBufferMessage, static_cast<int>(BattleEvent::Event::Type::BufferMessage));
  buttons->addButton(btnBufferValue, static_cast<int>(BattleEvent::Event::Type::BufferValue));
  buttons->addButton(btnBufferCharacter, static_cast<int>(BattleEvent::Event::Type::BufferCharacter));
  buttons->addButton(btnShowMessage, static_cast<int>(BattleEvent::Event::Type::ShowMessage));
  buttons->addButton(btnJump, static_cast<int>(BattleEvent::Event::Type::Jump));
  buttons->addButton(btnIfCounter, static_cast<int>(BattleEvent::Event::Type::IfCounter));
  buttons->addButton(btnIfStatus, static_cast<int>(BattleEvent::Event::Type::IfStatus));
  buttons->addButton(btnIfStat, static_cast<int>(BattleEvent::Event::Type::IfStat));
  buttons->addButton(btnChangeBattlerSprite, static_cast<int>(BattleEvent::Event::Type::ChangeBattlerSprite));
  buttons->addButton(btnChangeBackgroundImage, static_cast<int>(BattleEvent::Event::Type::ChangeBackgroundImage));
  buttons->addButton(btnPlayBattleAnimation, static_cast<int>(BattleEvent::Event::Type::PlayBattleAnimation));
  buttons->addButton(btnWaitFrames, static_cast<int>(BattleEvent::Event::Type::WaitFrames));
  buttons->addButton(btnWaitButtons, static_cast<int>(BattleEvent::Event::Type::WaitButtons));
  buttons->addButton(btnInflictStatus, static_cast<int>(BattleEvent::Event::Type::InflictStatus));
  buttons->addButton(btnConsumeMP, static_cast<int>(BattleEvent::Event::Type::ConsumeMP));
  buttons->addButton(btnRandom, static_cast<int>(BattleEvent::Event::Type::Random));
  buttons->addButton(btnEndEventProcessing, static_cast<int>(BattleEvent::Event::Type::EndEventProcessing));
  buttons->addButton(btnLabel, static_cast<int>(BattleEvent::Event::Type::Label));
  buttons->addButton(btnComment, static_cast<int>(BattleEvent::Event::Type::Comment));
  buttons->addButton(btnElse, static_cast<int>(BattleEvent::Event::Type::Else));
  buttons->addButton(btnCancel, -1);
  btnElse->setEnabled(allowElse);
  connect(buttons, SIGNAL(buttonClicked(int)), this, SLOT(done(int)));
}

ChooseBattleEvent::~ChooseBattleEvent()
{
}
