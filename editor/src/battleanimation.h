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

#ifndef BATTLEANIMATION_H
#define BATTLEANIMATION_H

#include <QByteArray>
#include <QString>
#include <QList>

class BattleAnimation
{
  public:
    struct Effect
    {
      uint8_t effect;
    };
    struct EffectSprite : public Effect
    {
      uint8_t spriteID;
      uint8_t x;
      uint8_t y;
      bool flipped;
      bool mirrored;
    };
    struct EffectPlaySFX : public Effect
    {
      QString sfx;
    };
    struct EffectShakeScreen : public Effect
    {
      uint8_t duration;
      uint8_t rate;
    };
    struct EffectFlashUI : public Effect
    {
      uint8_t duration;
      uint8_t color;
    };
    struct EffectFlashBattler : public Effect
    {
      uint8_t duration;
      uint8_t color1;
      uint8_t color2;
      uint8_t color3;
    };
    struct EffectFlashPlayer : public Effect
    {
      uint8_t duration;
      uint8_t color1;
      uint8_t color2;
      uint8_t color3;
    };
    static constexpr uint8_t SPRITE_FLIPPED = 1;
    static constexpr uint8_t SPRITE_MIRRORED = 2;
    static constexpr uint8_t EFFECT_NONE = 0;
    static constexpr uint8_t EFFECT_SPRITE = 1;
    static constexpr uint8_t EFFECT_PLAY_SFX = 2;
    static constexpr uint8_t EFFECT_SHAKE_SCREEN = 3;
    static constexpr uint8_t EFFECT_FLASH_UI = 4;
    static constexpr uint8_t EFFECT_FLASH_BATTLER = 5;
    static constexpr uint8_t EFFECT_FLASH_PLAYER = 6;
    BattleAnimation(QString name);
    ~BattleAnimation();
    void loadAnimation();
    void saveAnimation();
    QByteArray compileAnimation();
    int numFrames() {return frames.count();}
    int numEffects(int frame) {return frames[frame].count();}
    Effect *getEffect(int frame, int effect) {return frames[frame][effect];}
    int getSpriteID(int sprite) {return sprites[sprite];}
    void setSpriteID(int sprite, uint8_t id) {sprites[sprite] = id;}
    void addEffect(int frame, Effect *effect) {frames[frame] += effect;}
    void deleteEffect(int frame, int effect) {delete frames[frame].takeAt(effect);}
    void addFrame() {frames += QList<Effect*>();}
    void deleteFrame(int frame)
    {
      while (numEffects(frame) > 0)
        deleteEffect(frame, 0);
      frames.removeAt(frame);
    }
  private:
    struct AnimationFrame
    {
      uint8_t effect[8];
      uint8_t value1[8];
      uint8_t value2[8];
      uint8_t value3[8];
      uint8_t value4[8];
    };
    QString fileName;
    QList<QList<Effect*> > frames;
    uint8_t sprites[8];
};

#endif //BATTLEANIMATION_H
    
