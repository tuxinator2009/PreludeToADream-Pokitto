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

#include <QObject>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QSize>
#include "globals.h"
#include "image.h"

const QVector<QRgb> Image::palette =
{
  0x00000000,0xFF202420,0xFF484848,0xFF686C68,0xFF909090,0xFFB0B4B0,0xFFD8D8D8,0xFFF8FCF8,0xFF201410,0xFF402828,0xFF604040,0xFF805450,0xFFA06868,0xFFC08080,0xFFE09490,0xFFF8A8A8,
  0xFF201810,0xFF403428,0xFF605040,0xFF806C50,0xFFA08868,0xFFC0A480,0xFFE0C090,0xFFF8DCA8,0xFF182010,0xFF384028,0xFF586040,0xFF708050,0xFF90A068,0xFFB0C080,0xFFD0E090,0xFFE8FCA8,
  0xFF102010,0xFF284028,0xFF406040,0xFF588050,0xFF70A068,0xFF88C080,0xFFA0E090,0xFFB8FCA8,0xFF102018,0xFF284030,0xFF406048,0xFF508060,0xFF68A080,0xFF80C098,0xFF90E0B0,0xFFA8FCC8,
  0xFF101C18,0xFF284038,0xFF406058,0xFF508078,0xFF68A098,0xFF80C0B8,0xFF90E0D8,0xFFA8FCF8,0xFF101820,0xFF283040,0xFF404C60,0xFF506480,0xFF6880A0,0xFF8098C0,0xFF90B0E0,0xFFA8CCF8,
  0xFF101420,0xFF282840,0xFF404060,0xFF585480,0xFF7068A0,0xFF8880C0,0xFFA094E0,0xFFB8A8F8,0xFF181420,0xFF382840,0xFF584060,0xFF705480,0xFF9068A0,0xFFB080C0,0xFFD094E0,0xFFE8A8F8,
  0xFF201418,0xFF402830,0xFF604050,0xFF805468,0xFFA06888,0xFFC080A0,0xFFE094C0,0xFFF8A8D8,0xFF200808,0xFF401410,0xFF602020,0xFF802828,0xFFA03430,0xFFC04040,0xFFE04848,0xFFF85450,
  0xFF201408,0xFF402C10,0xFF604420,0xFF805C28,0xFFA07430,0xFFC08840,0xFFE0A048,0xFFF8B850,0xFF182008,0xFF304010,0xFF506020,0xFF688028,0xFF88A030,0xFFA0C040,0xFFC0E048,0xFFD8FC50,
  0xFF082008,0xFF184010,0xFF286020,0xFF388028,0xFF48A030,0xFF58C040,0xFF68E048,0xFF78FC50,0xFF082010,0xFF104020,0xFF206038,0xFF288048,0xFF30A060,0xFF40C070,0xFF48E080,0xFF50FC98,
  0xFF082018,0xFF104038,0xFF206058,0xFF288078,0xFF30A098,0xFF40C0B8,0xFF48E0D8,0xFF50FCF8,0xFF081020,0xFF102440,0xFF203860,0xFF284C80,0xFF3060A0,0xFF4070C0,0xFF4884E0,0xFF5098F8,
  0xFF080820,0xFF181440,0xFF282060,0xFF382880,0xFF4834A0,0xFF5840C0,0xFF6048E0,0xFF7054F8,0xFF180820,0xFF301440,0xFF502060,0xFF682880,0xFF8834A0,0xFFA040C0,0xFFC048E0,0xFFD854F8,
  0xFF200810,0xFF401428,0xFF602040,0xFF802858,0xFFA03470,0xFFC04088,0xFFE048A0,0xFFF854B8,0xFF200000,0xFF400000,0xFF600000,0xFF800000,0xFFA00000,0xFFC00000,0xFFE00000,0xFFF80000,
  0xFF201000,0xFF402000,0xFF603000,0xFF804400,0xFFA05400,0xFFC06400,0xFFE07800,0xFFF88800,0xFF182000,0xFF384000,0xFF586000,0xFF708000,0xFF90A000,0xFFB0C000,0xFFC8E000,0xFFE8FC00,
  0xFF082000,0xFF184000,0xFF206000,0xFF308000,0xFF38A000,0xFF48C000,0xFF50E000,0xFF60FC00,0xFF002000,0xFF004008,0xFF006008,0xFF008010,0xFF00A018,0xFF00C018,0xFF00E020,0xFF00FC28,
  0xFF002010,0xFF004028,0xFF006040,0xFF008058,0xFF00A070,0xFF00C080,0xFF00E098,0xFF00FCB0,0xFF001420,0xFF002C40,0xFF004460,0xFF005C80,0xFF0074A0,0xFF008CC0,0xFF00A0E0,0xFF00B8F8,
  0xFF000420,0xFF000C40,0xFF001060,0xFF001880,0xFF001CA0,0xFF0024C0,0xFF0028E0,0xFF0030F8,0xFF080020,0xFF100040,0xFF200060,0xFF280080,0xFF3800A0,0xFF4000C0,0xFF4800E0,0xFF5800F8,
  0xFF180020,0xFF380040,0xFF500060,0xFF700080,0xFF8800A0,0xFFA800C0,0xFFC800E0,0xFFE000F8,0xFF200010,0xFF400020,0xFF600030,0xFF800048,0xFFA00058,0xFFC00068,0xFFE00078,0xFFF80088
};

Image::Image(QString fileLocation, QObject *parent, bool autoReload) : QObject(parent)
{
  QImage image(fileLocation);
  filename = fileLocation;
  size = image.size();
  pixels = new uint8_t[size.width() * size.height()];
  reloadImage();
  if (autoReload)
  {
    Globals::watchdog->addPath(fileLocation);
    connect(Globals::watchdog, SIGNAL(fileChanged(QString)), this, SLOT(reloadImage(QString)));
    connect(Globals::watchdog, SIGNAL(fileChanged(QString)), this, SIGNAL(imageChanged()));
  }
}

Image::Image(QSize s, QObject *parent) : QObject(parent)
{
  size = s;
  pixels = new uint8_t[size.width() * size.height()];
  for (int i = 0; i < size.width() * size.height(); ++i)
    pixels[i] = 0;
}

Image::Image(const Image &other, QRect r, QObject *parent) : QObject(parent)
{
  size = r.size();
  pixels = new uint8_t[size.width() * size.height()];
  this->blitImage(other, r, QPoint(0, 0), false);
}

Image::~Image()
{
  delete[] pixels;
}

bool Image::isEmpty()
{
  uint8_t *pixel = pixels;
  for (int i = 0; i < size.width() * size.height(); ++i, ++pixel)
  {
    if (*pixel != 0)
      return false;
  }
  return true;
}

uint8_t Image::getPixel(QPoint p) const
{
  if (p.x() < 0 || p.x() >= size.width())
    return 0;
  if (p.y() < 0 || p.y() >= size.height())
    return 0;
  return pixels[p.y() * size.width() + p.x()];
}

void Image::setPixel(QPoint p, uint8_t pixel)
{
  if (p.x() < 0 || p.x() >= size.width())
    return;
  if (p.y() < 0 || p.y() >= size.height())
    return;
  pixels[p.y() * size.width() + p.x()] = pixel;
}

void Image::fillRect(QRect r, uint8_t index)
{
  for (int y = 0; y < r.height(); ++y)
  {
    for (int x = 0; x < r.width(); ++x)
    {
      if (x + r.x() >= 0 && x + r.x() < size.width() && y + r.y() >= 0 && y + r.y() < size.height())
        pixels[(y + r.y()) * size.width() + (x + r.x())] = index;
    }
  }
}

void Image::drawRect(QRect r, uint8_t index)
{
  drawHLine(r.topLeft(), r.width(), index);
  drawHLine(r.bottomLeft(), r.width(), index);
  drawVLine(r.topLeft(), r.height(), index);
  drawVLine(r.topRight(), r.height(), index);
}

void Image::drawHLine(QPoint pos, int width, uint8_t index)
{
  if (pos.y() < 0 || pos.y() >= size.height())
    return;
  for (int x = 0; x < width; ++x)
  {
    if (x + pos.x() >= 0 && x + pos.x() < size.width())
      pixels[pos.y() * size.width() + x + pos.x()] = index;
  }
}

void Image::drawVLine(QPoint pos, int height, uint8_t index)
{
  if (pos.x() < 0 || pos.x() >= size.width())
    return;
  for (int y = 0; y < height; ++y)
  {
    if (y + pos.y() >= 0 && y + pos.y() < size.height())
      pixels[(pos.y() + y) * size.width() + pos.x()] = index;
  }
}

void Image::recolor(uint8_t color1, uint8_t color2)
{
  uint8_t *pixel = pixels;
  for (int i = 0; i < size.width() * size.height(); ++i)
  {
    if (*pixel == color1)
      *pixel = color2;
    ++pixel;
  }
}

void Image::recolor(uint8_t color1, uint8_t color2, uint8_t color3)
{
  uint8_t *pixel = pixels;
  for (int i = 0; i < size.width() * size.height(); ++i)
  {
    if (*pixel != 0)
    {
      if (*pixel < color2)
        *pixel = (*pixel % 8) + color1;
      else if (*pixel < color3)
        *pixel = (*pixel % 8) + color2;
      else
        *pixel = (*pixel % 8) + color3;
    }
    ++pixel;
  }
}

void Image::getPixels(QRect r, uint8_t *buffer) const
{
  for (int y = 0; y < r.height(); ++y)
  {
    for (int x = 0; x < r.width(); ++x)
      buffer[y * r.width() + x] = pixels[(y + r.y()) * size.width() + (x + r.x())];
  }
}

void Image::blitImage(const Image &other, QRect src, QPoint dest, bool transparent, bool flipped, bool mirrored)
{
  for (int y = 0; y < src.height(); ++y)
  {
    for (int x = 0; x < src.width(); ++x)
    {
      if (x + dest.x() < 0 || x + dest.x() >= size.width())
        continue;
      if (y + dest.y() < 0 || y + dest.y() >= size.height())
        continue;
      int srcX = src.x() + ((mirrored) ? (src.width() - x - 1):x);
      int srcY = src.y() + ((flipped) ? (src.height() - y - 1):y);
      if (srcX >= 0 && srcX < other.size.width() && srcY >= 0 && srcY < other.size.height())
      {
        uint8_t pixel = other.getPixel(QPoint(srcX, srcY));
        if (!transparent || pixel != 0)
          pixels[(y + dest.y()) * size.width() + (x + dest.x())] = pixel;
      }
    }
  }
}

QImage Image::toQImage(QRect r, bool transparent) const
{
  QImage image(r.size(), QImage::Format_Indexed8);
  image.setColorTable(palette);
  if (!transparent)
    image.setColor(0, QRgb(0xFF000000));
  for (int y = 0; y < r.height(); ++y)
  {
    for (int x = 0; x < r.width(); ++x)
      image.setPixel(x, y, pixels[(y + r.y()) * size.width() + (x + r.x())]);
  }
  return image;
}

QByteArray Image::toByteArray(int frameW, int frameH)
{
  QByteArray bytes;
  for (int y = 0; y < size.height(); y += frameH)
  {
    for (int x = 0; x < size.width(); x += frameW)
    {
      for (int y2 = 0; y2 < frameH; ++y2)
      {
        for (int x2 = 0; x2 < frameW; ++x2)
          bytes += (char)pixels[(y + y2) * size.width() + x + x2];
      }
    }
  }
  return bytes;
}

void Image::reloadImage(QString location)
{
  if (location != filename && !location.isNull())
    return;
  QImage image(filename);
  if (image.isNull())
    return;
  for (int y = 0; y < size.height(); ++y)
  {
    for (int x = 0; x < size.width(); ++x)
      pixels[y * size.width() + x] = image.pixelIndex(x, y);
  }
}
