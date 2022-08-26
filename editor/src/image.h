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

#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QSize>

class Image : public QObject
{
  Q_OBJECT
  public:
    Image(QString fileLocation, QObject *parent=nullptr, bool autoReload=true);
    Image(QSize s, QObject *parent=nullptr);
    Image(const Image &other, QRect r, QObject *parent=nullptr);
    ~Image();
    uint8_t getPixel(QPoint p) const;
    void setPixel(QPoint p, uint8_t pixel);
    void fillRect(QRect r, uint8_t index);
    void drawRect(QRect r, uint8_t index);
    void drawHLine(QPoint pos, int width, uint8_t index);
    void drawVLine(QPoint pos, int height, uint8_t index);
    void recolor(uint8_t color1, uint8_t color2);
    void recolor(uint8_t color1, uint8_t color2, uint8_t color3);
    void getPixels(QRect r, uint8_t *buffer) const;
    void blitImage(const Image &other, QRect src, QPoint dest, bool transparent, bool flipped=false, bool mirrored=false);
    int getWidth() {return size.width();}
    int getHeight() {return size.height();}
    QSize getSize() {return size;}
    QImage toQImage(QRect r, bool transparent=true) const;
    QImage toQImage(bool transparent=true) {return toQImage(QRect(QPoint(0, 0), size), transparent);}
    static const QVector<QRgb> palette;
  signals:
    void imageChanged();
  public slots:
    void reloadImage(QString location=QString());
  private:
    QString filename;
    QSize size;
    uint8_t *pixels;
};

#endif //IMAGE_H
