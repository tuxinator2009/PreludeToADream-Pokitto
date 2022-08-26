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

#ifndef CONFIGUREEVENT_CHANGESPRITE_H
#define CONFIGUREEVENT_CHANGESPRITE_H

#include "ui_configureevent_changesprite.h"
#include "globals.h"
#include "image.h"
#include "map.h"

class ConfigureEvent_ChangeSprite : public QDialog, public Ui::ConfigureEvent_ChangeSprite
{
  Q_OBJECT
  public:
    ConfigureEvent_ChangeSprite(Map *m, QWidget *parent=nullptr) : QDialog(parent)
    {
      setupUi(this);
      map = m;
      image = new Image(QSize(128, 48), this);
      gfxSprites->setSize(256, 96);
      gfxSprites->setTransparent(true);
      gfxSprites->setImage(image);
    }
    ~ConfigureEvent_ChangeSprite() {}
    void addEvent(QString value) {optEvent->addItem(value);}
    void setEvent(QString value) {optEvent->setCurrentText(value);}
    QString getEvent() {return optEvent->currentText();}
    void setSprite(int value)
    {
      spriteID = value;
      drawSprites();
    }
    int getSprite() {return spriteID;}
  protected slots:
    void on_gfxSprites_mousePressed(Qt::MouseButton button, QPoint pos)
    {
      if (button == Qt::LeftButton)
      {
        spriteID = pos.y() / 48 * 8 + pos.x() / 32;
        drawSprites();
      }
    }
  private:
    void drawSprites()
    {
      image->fillRect(QRect(0, 0, 128, 48), 0);
      for (int i = 0; i < 16; ++i)
      {
        int id = map->getSpriteID(i);
        image->blitImage(*Globals::sprites, QRect(id % 16 * 16, id / 16 * 24, 16, 24), QPoint(i % 8 * 16, i / 8 * 24), true);
      }
      image->drawRect(QRect(spriteID % 8 * 16, spriteID / 8 * 24, 16, 24), 255);
      gfxSprites->imageChanged();
    }
    Map *map;
    Image *image;
    int spriteID;
};

#endif //CONFIGUREEVENT_CHANGESPRITE_H
    
