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

#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QScrollBar>
#include <QTimer>
#include <algorithm>
#include "battleanimation.h"
#include "compiledata.h"
#include "globals.h"
#include "image.h"
#include "map.h"
#include "monster.h"
#include "tileset.h"

CompileData::CompileData(QTreeWidgetItem *root, QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  rootMapItem = root;
  audioDecoder = new QAudioDecoder(this);
  audioFormat.setChannelCount(1);
  audioFormat.setCodec("audio/x-raw");
  audioFormat.setSampleType(QAudioFormat::UnSignedInt);
  audioFormat.setSampleRate(8000);
  audioFormat.setSampleSize(8);
  audioDecoder->setAudioFormat(audioFormat);
  connect(audioDecoder, SIGNAL(bufferReady()), this, SLOT(audioBufferReady()));
  connect(audioDecoder, SIGNAL(error(QAudioDecoder::Error)), this, SLOT(audioError(QAudioDecoder::Error)));
  connect(audioDecoder, SIGNAL(finished()), this, SLOT(audioFinished()));
  QTimer::singleShot(10, this, SLOT(compileData()));
}

CompileData::~CompileData()
{
}

void CompileData::compileData()
{
  QElapsedTimer timer;
  qint64 timeTaken;
  timer.start();
  if (!compileAnimations())
    return;
  if (!compileBackdrops())
    return;
  if (!compileBattlers())
    return;
  if (!compileMaps())
    return;
  if (!compileMusic())
    return;
  if (!compileScreens())
    return;
  if (!compileSFX())
    return;
  if (!compileSprites())
    return;
  if (!compileTilesets())
    return;
  if (!packData())
    return;
  if (!generateResourcesHeader())
    return;
  timeTaken = timer.elapsed();
  outputText(Qt::green, QString("Compiled in %1.%2 seconds\n").arg(timeTaken / 1000).arg(timeTaken % 1000, 3, 10, QChar('0')));
}

void CompileData::on_btnCompileHW_clicked()
{
}

void CompileData::on_btnCompileSIM_clicked()
{
}

void CompileData::on_btnLaunchSIM_clicked()
{
}

void CompileData::audioBufferReady()
{
  QAudioBuffer buffer = audioDecoder->read();
  audioData.append(buffer.data<char>(), buffer.byteCount());
}

void CompileData::audioError(QAudioDecoder::Error error)
{
  outputText(Qt::red, QString("error %1: %2\n").arg(static_cast<int>(error)).arg(audioDecoder->errorString()));
  decoding = false;
  decodeError = true;
}

void CompileData::audioFinished()
{
  outputText(Qt::red, QString("%1 bytes\n").arg(audioData.size()));
  decoding = false;
}

bool CompileData::compileAnimations()
{
  QDir folder(QString("%1/animations").arg(Globals::datadir));
  QStringList animations = folder.entryList(QStringList() << "*.xml", QDir::Files, QDir::Name);
  animations.replaceInStrings(".xml", "", Qt::CaseInsensitive);
  outputText(Qt::green, "Compiling animations\n");
  for (auto animation : animations)
  {
    BattleAnimation anim(animation);
    outputText(Qt::lightGray, QString("  %1\n").arg(animation));
    anim.loadAnimation();
    dataFiles += DataFile(QString("/animations/%1.xml").arg(animation), anim.compileAnimation());
  }
  return true;
}

bool CompileData::compileBackdrops()
{
  outputText(Qt::green, "Compiling backdrops\n");
  for (auto backdrop : Globals::backdrops)
  {
    Image image(QString("%1/backdrops/%2.png").arg(Globals::datadir).arg(backdrop), nullptr, false);
    dataFiles += DataFile(QString("/backdrops/%2.png").arg(backdrop), image.toByteArray(8, 8));
    outputText(Qt::lightGray, QString("  %1.png\n").arg(backdrop));
  }
  return true;
}

bool CompileData::compileBattlers()
{
  QString monsterFile = QString("%1/battlers/monster000.xml").arg(Globals::datadir);
  int monsterID = 0;
  outputText(Qt::green, "Compiling battlers\n");
  outputText(Qt::lightGray, "  battlers.png\n");
  dataFiles += DataFile("/battlers.png", Globals::battlers->toByteArray(48, 48));
  while (QFile::exists(monsterFile))
  {
    XMLNode monsterNode = XMLNode::parseFile(monsterFile.toLocal8Bit().data(), "monster");
    Monster monster(monsterNode);
    outputText(Qt::lightGray, QString("  %1 (monster%2.xml)\n").arg(monster.getName(), -8).arg(monsterID, 3, 10, QChar('0')));
    dataFiles += DataFile(QString("/battlers/%1").arg(monster.getName()), monster.compile());
    ++monsterID;
    monsterFile = QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(monsterID, 3, 10, QChar('0'));
  }
  return true;
}

bool CompileData::compileMaps()
{
  QList<QTreeWidgetItem*> items;
  outputText(Qt::green, "Compiling maps\n");
  for (int i = 0; i < rootMapItem->childCount(); ++i)
    items += rootMapItem->child(i);
  while (!items.isEmpty())
  {
    QTreeWidgetItem *item = items.takeFirst();
    QTreeWidgetItem *parent = item;
    XMLNode mapNode = XMLNode::parseFile(QString("%1/maps/%2").arg(Globals::datadir).arg(item->text(1)).toLocal8Bit().data(), "ptad-map");
    Map map(mapNode);
    for (int i = item->childCount() - 1; i >= 0; --i)
      items.push_front(item->child(i));
    while (parent != nullptr)
    {
      outputText(Qt::lightGray, "  ");
      parent = parent->parent();
    }
    outputText(Qt::lightGray, QString("%1 (%2) ").arg(item->text(0)).arg(item->text(1)));
    dataFiles += DataFile(item->text(2), map.compile());
    outputText(Qt::red, QString("%1 bytes\n").arg(dataFiles.last().bytes.size()));
  }
  return true;
}

bool CompileData::compileMusic()
{
  int maxLength = 0;
  outputText(Qt::green, "Compiling background music\n");
  for (auto bgm : Globals::bgms)
  {
    if (bgm.length() > maxLength)
      maxLength = bgm.length();
  }
  maxLength += 5;
  for (auto bgm : Globals::bgms)
  {
    QString fileLocation = QString("%1/music/%2.ogg").arg(Globals::datadir).arg(bgm);
    outputText(Qt::lightGray, QString("  %1").arg(bgm + ".ogg", -maxLength));
    audioDecoder->setSourceFilename(fileLocation);
    audioDecoder->start();
    decoding = true;
    decodeError = false;
    while (decoding)
    {
      QCoreApplication::processEvents();
      if (decodeError)
        return false;
    }
    dataFiles += DataFile(QString("/music/%2.ogg").arg(bgm), audioData);
    audioData.clear();
  }
  return true;
}

bool CompileData::compileScreens()
{
  QStringList screens = QDir(QString("%1/screens").arg(Globals::datadir)).entryList(QStringList() << "*.png", QDir::Files, QDir::Name);
  outputText(Qt::green, "Compiling screens\n");
  for (auto screen : screens)
  {
    Image image(QString("%1/screens/%2").arg(Globals::datadir).arg(screen), nullptr, false);
    outputText(Qt::lightGray, QString("  %1\n").arg(screen));
    dataFiles += DataFile(QString("/screens/%1").arg(screen), image.toByteArray(220, 176));
  }
  return true;
}

bool CompileData::compileSFX()
{
  int maxLength = 0;
  outputText(Qt::green, "Compiling sfx\n");
  for (auto sfx : Globals::sfx)
  {
    if (sfx.length() > maxLength)
      maxLength = sfx.length();
  }
  maxLength += 5;
  for (auto sfx : Globals::sfx)
  {
    QString fileLocation = QString("%1/sfx/%2.wav").arg(Globals::datadir).arg(sfx);
    outputText(Qt::lightGray, QString("  %1").arg(sfx + ".wav", -maxLength));
    audioDecoder->setSourceFilename(fileLocation);
    audioDecoder->start();
    decoding = true;
    decodeError = false;
    while (decoding)
    {
      QCoreApplication::processEvents();
      if (decodeError)
        return false;
    }
    dataFiles += DataFile(QString("/sfx/%2.wav").arg(sfx), audioData);
    audioData.clear();
  }
  return true;
}

bool CompileData::compileSprites()
{
  outputText(Qt::green, "Compiling sprites\n");
  dataFiles += DataFile("/sprites.png", Globals::sprites->toByteArray(16, 24));
  return true;
}

bool CompileData::compileTilesets()
{
  outputText(Qt::green, "Compiling tilesets\n");
  for (auto tileset : Globals::tilesets)
  {
    outputText(Qt::lightGray, QString("  %1\n").arg(tileset->getName()));
    dataFiles += DataFile(QString("/tilesets/%1").arg(tileset->getLocation()).replace(".png", ".dat"), tileset->compile());
    dataFiles += DataFile(QString("/tilesets/%1").arg(tileset->getLocation()), tileset->getImage()->toByteArray(8, 8));
  }
  return true;
}

bool CompileData::packData()
{
  QFile file(QString("%1/../data/preludetoadream_new.pak").arg(Globals::datadir));
  QByteArray headerBytes;
  FileHeader header[dataFiles.count()];
  int headerBlock = 0;
  size_t headerSize = dataFiles.count() * sizeof(FileHeader) + 4;
  outputText(Qt::green, "Packing data\n");
  std::sort(dataFiles.begin(), dataFiles.end());
  std::reverse(dataFiles.begin(), dataFiles.end());
  while (headerSize >= BLOCK_SIZE)
  {
    DataBlock block;
    block.availableBytes = 0;
    blocks += block;
    headerSize -= BLOCK_SIZE;
  }
  if (headerSize != 0)
  {
    DataBlock block;
    block.availableBytes = BLOCK_SIZE - headerSize;
    blocks += block;
  }
  for (int i = 0; i < dataFiles.size(); ++i)
  {
    bool createBlocks = true;
    header[i].hash = Globals::hash(dataFiles[i].location.toLocal8Bit().data(), dataFiles[i].location.size());
    header[i].start = blocks.size() * BLOCK_SIZE;
    outputText(Qt::lightGray, QString("  %1 (%2) ").arg(dataFiles[i].location).arg(header[i].hash, 8, 16, QChar('0')));
    outputText(Qt::red, QString("(%1 bytes)\n").arg(dataFiles[i].bytes.size()));
    if (dataFiles[i].size < BLOCK_SIZE)
    {
      for (int i = 0; i < blocks.size(); ++i)
      {
        header[i].start = i * BLOCK_SIZE + (BLOCK_SIZE - blocks[i].availableBytes);
        blocks[i].write(dataFiles[i].bytes);
        createBlocks = false;
        break;
      }
    }
    if (createBlocks)
    {
      while (!dataFiles[i].bytes.isEmpty())
      {
        DataBlock block;
        block.write(dataFiles[i].bytes.left(BLOCK_SIZE));
        dataFiles[i].bytes = dataFiles[i].bytes.mid(BLOCK_SIZE);
        blocks += block;
      }
    }
    header[i].end = header[i].start + dataFiles[i].size;
    for (int j = 0; j < i; ++j)
    {
      if (header[i].hash == header[j].hash)
      {
        outputText(Qt::red, QString("Error: \"%1\" and \"%2\" share the same hash (%3)\n").arg(dataFiles[i].location).arg(dataFiles[j].location).arg(header[i].hash, 8, 16, QChar('0')));
        return false;
      }
    }
  }
  headerBytes.append((const char *)header, dataFiles.count() * sizeof(FileHeader));
  for (int i = 0; i < blocks.size(); ++i)
    blocks[i].availableBytes = BLOCK_SIZE;
  while (!headerBytes.isEmpty())
  {
    blocks[headerBlock].write(headerBytes.left(BLOCK_SIZE));
    headerBytes = headerBytes.mid(BLOCK_SIZE);
    ++headerBlock;
  }
  if (!file.open(QFile::WriteOnly))
  {
    outputText(Qt::red, QString("Error: failed to open \"%1\"\nReason: %2\n").arg(file.fileName()).arg(file.errorString()));
    return false;
  }
  for (auto block : blocks)
    block.write(file);
  file.close();
  return true;
}

bool CompileData::generateResourcesHeader()
{
  outputText(Qt::green, "Generating resources.h\n");
  return true;
}

void CompileData::outputText(QColor color, QString text)
{
  QScrollBar *scrollBar = txtOutput->verticalScrollBar();
  bool autoScroll = scrollBar->value() == scrollBar->maximum();
  txtOutput->setTextColor(color);
  txtOutput->insertPlainText(text);
  if (autoScroll)
    scrollBar->setValue(scrollBar->maximum());
}
