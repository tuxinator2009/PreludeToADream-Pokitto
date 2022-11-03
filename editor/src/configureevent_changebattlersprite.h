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

#ifndef CONFIGUREEVENT_CHANGEBATTLERSPRITE_H
#define CONFIGUREEVENT_CHANGEBATTLERSPRITE_H

#include "ui_configureevent_changebattlersprite.h"
#include "globals.h"
#include "image.h"
#include "imageview.h"

class ConfigureEvent_ChangeBattlerSprite : public QDialog, public Ui::ConfigureEvent_ChangeBattlerSprite
{
  Q_OBJECT
  public:
    ConfigureEvent_ChangeBattlerSprite(QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      image = new Image(QSize(768, 768));
      imgBattlerSprites->setImage(image);
      imgBattlerSprites->setTransparent(true);
      battlersImageChanged();
      connect(Globals::battlers, SIGNAL(imageChanged()), this, SLOT(battlersImageChanged()));
    }
    ~ConfigureEvent_ChangeBattlerSprite() {delete image;}
    void setSpriteID(int value)
    {
      spriteID = value;
      battlersImageChanged();
    }
    int getSpriteID() {return spriteID;}
  protected slots:
    void on_imgBattlerSprites_mousePressed(Qt::MouseButton button, QPoint pos)
    {
      if (button == Qt::LeftButton)
      {
        spriteID = pos.y() / 48 * 16 + pos.x() / 48;
        battlersImageChanged();
      }
    }
    void on_imgBattlerSprites_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta)
    {
      Q_UNUSED(delta);
      if ((buttons & Qt::LeftButton) != 0)
      {
        spriteID = pos.y() / 48 * 16 + pos.x() / 48;
        battlersImageChanged();
      }
    }
    void on_imgBattlerSprites_mouseReleased(Qt::MouseButton button, QPoint pos)
    {
      if (button == Qt::LeftButton)
      {
        spriteID = pos.y() / 48 * 16 + pos.x() / 48;
        battlersImageChanged();
      }
    }
    void battlersImageChanged()
    {
      image->blitImage(*Globals::battlers, QRect(0, 0, 768, 768), QPoint(0, 0), false);
      image->drawRect(QRect(spriteID % 16 * 48, spriteID / 16 * 48, 48, 48), 223);
      imgBattlerSprites->imageChanged();
    }
  private:
    Image *image;
    int spriteID;
};

#endif //CONFIGUREEVENT_CHANGEBATTLERSPRITE_H
