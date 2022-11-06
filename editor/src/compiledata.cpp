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
#include <QFileDialog>
#include <QScrollBar>
#include <algorithm>
#include "battleanimation.h"
#include "compiledata.h"
#include "globals.h"
#include "image.h"
#include "map.h"
#include "monster.h"
#include "tileset.h"

const uint8_t CompileData::newGameEvent[] =
{
  0x00,0x01,0x02,
  0x04,
  0x01,0x01,0x02,
  0x31
};

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
  leFemtoIDELocation->setText(Globals::femtoIDE);
}

CompileData::~CompileData()
{
}

void CompileData::on_btnBrowseFemtoIDE_clicked()
{
  QString location = QFileDialog::getOpenFileName(this, "FemtoIDE Executable", Globals::datadir);
  if (!location.isEmpty())
  {
    Globals::femtoIDE = location;
    leFemtoIDELocation->setText(location);
  }
}

void CompileData::on_btnCompileData_clicked()
{
  QElapsedTimer timer;
  qint64 timeTaken;
  audioData.clear();
  dataFiles.clear();
  blocks.clear();
  txtOutput->clear();
  timer.start();
  dataFiles += DataFile("/newGameEvent.dat", QByteArray((const char*)newGameEvent, sizeof(newGameEvent)));
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
  if (!generateResourcesSource())
    return;
  timeTaken = timer.elapsed();
  outputText(Qt::green, QString("Compiled in %1.%2 seconds\n").arg(timeTaken / 1000).arg(timeTaken % 1000, 3, 10, QChar('0')));
}

void CompileData::on_btnCompileHW_clicked()
{
  txtOutput->clear();
  this->setEnabled(false);
  outputText(Qt::green, "Building HW binary\n");
  process = new QProcess();
  connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
  connect(process, SIGNAL(readyReadStandardError()), this, SLOT(processReadyReadStandardError()));
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadStandardOutput()));
  process->start(Globals::femtoIDE, QStringList() << "PreludeToADream" << "compile" << "-" << "true" << "0" << "Pokitto");
}

void CompileData::on_btnCompileSIM_clicked()
{
  txtOutput->clear();
  this->setEnabled(false);
  outputText(Qt::green, "Building HW binary\n");
  process = new QProcess();
  connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
  connect(process, SIGNAL(readyReadStandardError()), this, SLOT(processReadyReadStandardError()));
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadStandardOutput()));
  process->start(Globals::femtoIDE, QStringList() << "PreludeToADream" << "compile" << "-" << "false" << "0" << "linux");
}

void CompileData::on_btnLaunchSIM_clicked()
{
  txtOutput->clear();
  this->setEnabled(false);
  outputText(Qt::green, QString("Launching \"%1/../PreludeToADream\"\n").arg(Globals::datadir));
  process = new QProcess();
  connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
  connect(process, SIGNAL(readyReadStandardError()), this, SLOT(processReadyReadStandardError()));
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadStandardOutput()));
  process->start(QString("%1/../PreludeToADream").arg(Globals::datadir));
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

void CompileData::processError(QProcess::ProcessError error)
{
  outputText(Qt::red, QString("\nProcess error (%1): %2\n").arg(static_cast<int>(error)).arg(process->errorString()));
  this->setEnabled(true);
}

void CompileData::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit)
    outputText(Qt::green, QString("Finished with exit code %1\n").arg(exitCode));
  else
    outputText(Qt::red, "\nProcess crashed\n");
  process->deleteLater();
  this->setEnabled(true);
}

void CompileData::processReadyReadStandardError()
{
  outputText(Qt::red, process->readAllStandardError());
}

void CompileData::processReadyReadStandardOutput()
{
  outputText(Qt::lightGray, process->readAllStandardOutput());
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
  dataFiles += DataFile("/animations/animations.png", Globals::animations->toByteArray(32, 32));
  return true;
}

bool CompileData::compileBackdrops()
{
  outputText(Qt::green, "Compiling backdrops\n");
  for (auto backdrop : Globals::backdrops)
  {
    Image image(QString("%1/backdrops/%2.png").arg(Globals::datadir).arg(backdrop), nullptr, false);
    dataFiles += DataFile(QString("/backdrops/%1.png").arg(backdrop), image.toByteArray(8, 8));
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
    Monster monster(XMLNode::parseFile(monsterFile.toLocal8Bit().data(), "monster"));
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
    Map map(XMLNode::parseFile(QString("%1/maps/%2").arg(Globals::datadir).arg(item->text(1)).toLocal8Bit().data(), "ptad-map"));
    for (int i = item->childCount() - 1; i >= 0; --i)
      items.push_front(item->child(i));
    while (parent != nullptr)
    {
      outputText(Qt::lightGray, "  ");
      parent = parent->parent();
    }
    outputText(Qt::lightGray, QString("%1 (%2) ").arg(item->text(0)).arg(item->text(1)));
    dataFiles += DataFile(QString("/maps/%1").arg(item->text(1)), map.compile());
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
    dataFiles += DataFile(QString("/music/%1.ogg").arg(bgm), audioData);
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
    dataFiles += DataFile(QString("/sfx/%1.wav").arg(sfx), audioData);
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
  QFile file(QString("%1/../data/preludetoadream.pak").arg(Globals::datadir));
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
    header[i].hash = Globals::hash(dataFiles[i].location.toLocal8Bit().data(), dataFiles[i].location.size() + 1);
    header[i].start = blocks.size() * BLOCK_SIZE;
    if (dataFiles[i].size < BLOCK_SIZE)
    {
      for (int j = 0; j < blocks.size(); ++j)
      {
        if (blocks[j].availableBytes >= dataFiles[i].size)
        {
          outputText(Qt::green, QString("  Found existing block (%1) with %2 bytes available.\n").arg(j).arg(blocks[j].availableBytes));
          header[i].start = j * BLOCK_SIZE + (BLOCK_SIZE - blocks[j].availableBytes);
          blocks[j].write(dataFiles[i].bytes);
          createBlocks = false;
          break;
        }
      }
    }
    if (createBlocks)
    {
      int size = dataFiles[i].bytes.size();
      while (!dataFiles[i].bytes.isEmpty())
      {
        DataBlock block;
        block.write(dataFiles[i].bytes.left(BLOCK_SIZE));
        dataFiles[i].bytes = dataFiles[i].bytes.mid(BLOCK_SIZE);
        blocks += block;
      }
      if (size % 512 != 0)
        outputText(Qt::green, QString("  Creating %1 new blocks (%2 available bytes)\n").arg((size / 512) + 1).arg(blocks[blocks.size() - 1].availableBytes));
      else
        outputText(Qt::green, QString("  Creating %1 new blocks (%2 available bytes)\n").arg(size / 512).arg(blocks[blocks.size() - 1].availableBytes));
    }
    header[i].end = header[i].start + dataFiles[i].size;
    outputText(Qt::lightGray, QString("  %1 (%2) ").arg(dataFiles[i].location).arg(header[i].hash, 8, 16, QChar('0')));
    outputText(Qt::red, QString("(%1 bytes from %2 to %3)\n").arg(dataFiles[i].bytes.size()).arg(header[i].start).arg(header[i].end));
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
  QFile file(QString("%1/../src/resources.h").arg(Globals::datadir));
  QTextStream stream(&file);
  int counter;
  outputText(Qt::green, "Generating resources.h\n");
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    outputText(Qt::red, QString("Failed to open \"%1\"\nReason: %2\n").arg(file.fileName()).arg(file.errorString()));
    return false;
  }
  stream << "//Automatically generated by editor\n";
  stream << "#pragma once\n\n";
  stream << "namespace PTAD\n";
  stream << "{\n";
  stream << "  namespace Resources\n";
  stream << "  {\n";
  stream << "    //Battle resources\n";
  stream << "    extern const uint32_t backgrounds[];\n";
  stream << "    extern const uint32_t enemies[];\n";
  stream << "    extern const uint8_t enemyNames[];\n";
  stream << "    extern const uint8_t numEnemies;\n";
  stream << "    //Dialog resources\n";
  stream << "    extern const uint8_t messages[];\n";
  stream << "    extern const uint8_t messageSizes[];\n";
  counter = 0;
  for (auto message : Globals::messages.keys())
    stream << "    constexpr int " << message << " = " << counter++ << ";\n";
  stream << "    //Game resources\n";
  stream << "    struct StatGrowth\n";
  stream << "    {\n";
  stream << "      uint32_t start;\n";
  stream << "      float base;\n";
  stream << "      float exponent;\n";
  stream << "    };\n";
  stream << "    extern const StatGrowth hpGrowth;\n";
  stream << "    extern const StatGrowth mpGrowth;\n";
  stream << "    extern const StatGrowth attackGrowth;\n";
  stream << "    extern const StatGrowth defenseGrowth;\n";
  stream << "    extern const StatGrowth agilityGrowth;\n";
  stream << "    extern const StatGrowth magicGrowth;\n";
  stream << "    extern const StatGrowth experienceGrowth;\n";
  stream << "    extern const uint8_t skillLearned[99];\n";
  stream << "    constexpr uint32_t getStatForLevel(uint8_t level, const StatGrowth &growth)\n";
  stream << "    {\n";
  stream << "      return growth.start + (uint32_t)(growth.base * pow(level, growth.exponent));\n";
  stream << "    }\n";
  stream << "    //Global resources\n";
  stream << "    struct Item\n";
  stream << "    {\n";
  stream << "      uint8_t name[8];\n";
  stream << "      uint8_t description[24];\n";
  stream << "      uint16_t price;\n";
  stream << "    };\n";
  stream << "    struct EquipmentStats\n";
  stream << "    {\n";
  stream << "      int8_t stats[4];\n";
  stream << "      uint16_t spellResistance;\n";
  stream << "      int8_t statusResistance[2];\n";
  stream << "    };\n";
  stream << "    extern const uint32_t tilesets[];\n";
  stream << "    extern const uint32_t tiledataFiles[];\n";
  stream << "    extern const uint8_t playerSprite[];\n";
  stream << "    extern const uint8_t cursor[];\n";
  stream << "    extern const uint8_t statusIcons[];\n";
  stream << "    extern const Item items[8][16];\n";
  stream << "    extern const Item skills[16];\n";
  stream << "    extern const Item spells[16];\n";
  stream << "    extern const EquipmentStats equipmentStats[6][16];\n";
  stream << "    //Audio resources\n";
  stream << "    extern const uint32_t music[];\n";
  stream << "    extern const uint32_t sfx[];\n";
  counter = 0;
  for (auto bgm : Globals::bgms)
    stream << "    constexpr int music_" << bgm << " = " << counter++ << ";\n";
  counter = 0;
  for (auto sfx : Globals::sfx)
    stream << "    constexpr int sfx_" << sfx << " = " << counter++ << ";\n";
  stream << "  };\n";
  stream << "};\n";
  file.close();
  return true;
}

bool CompileData::generateResourcesSource()
{
  QFile file(QString("%1/../src/resources.cpp").arg(Globals::datadir));
  QTextStream stream(&file);
  QByteArray bytes;
  int skillLearned[99];
  int counter;
  outputText(Qt::green, "Generating resources.cpp\n");
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    outputText(Qt::red, QString("Failed to open \"%1\"\nReason: %2\n").arg(file.fileName()).arg(file.errorString()));
    return false;
  }
  for (int i = 0; i < 99; ++i)
    skillLearned[i] = 255;
  for (int i = 0; i < 16; ++i)
  {
    if (Globals::skillLearned[i] != 0)
      skillLearned[Globals::skillLearned[i] - 1] = i;
  }
  stream << "//Automatically generated by editor\n";
  stream << "#include <cstdint>\n";
  stream << "#include \"DataPack/DataPack.h\"\n";
  stream << "#include \"resources.h\"\n\n";
  stream << "//BEGIN: Battle resources\n";
  stream << "const uint32_t PTAD::Resources::backgrounds[] =\n";
  stream << "{\n";
  for (auto backdrop : Globals::backdrops)
    stream << "  DataPack::hash(\"/backdrops/" << backdrop << ".png\"),\n";
  stream << "};\n\n";
  stream << "const uint32_t PTAD::Resources::enemies[] =\n";
  stream << "{\n";
  counter = 0;
  while (QFile::exists(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(counter, 3, 10, QChar('0'))))
  {
    Monster monster(XMLNode::parseFile(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(counter, 3, 10, QChar('0')).toLocal8Bit().data(), "monster"));
    stream << "  DataPack::hash(\"/battlers/" << monster.getName() << "\"),\n";
    ++counter;
  }
  stream << "};\n\n";
  stream << "const uint8_t PTAD::Resources::enemyNames[]\n";
  stream << "{\n";
  counter = 0;
  while (QFile::exists(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(counter, 3, 10, QChar('0'))))
  {
    Monster monster(XMLNode::parseFile(QString("%1/battlers/monster%2.xml").arg(Globals::datadir).arg(counter, 3, 10, QChar('0')).toLocal8Bit().data(), "monster"));
    QString name = monster.getName();
    name.resize(8, QChar('_'));
    stream << "  //" << name << "\n  ";
    for (int i = 0; i < 8; ++i)
      stream << "0x" << QString("%1").arg(Globals::charToCode(name[i].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
    stream << "\n";
    ++counter;
  }
  stream << "};\n\n";
  stream << "const uint8_t PTAD::Resources::numEnemies = " << counter << ";\n";
  stream << "//END\n\n";
  stream << "//BEGIN: Dialog resources\n";
  stream << "const uint8_t PTAD::Resources::messages[] =\n";
  stream << "{\n";
  for (auto message : Globals::messages.keys())
  {
    QString text = Globals::messages[message];
    text.resize(24, QChar('_'));
    stream << "  //" << text << "\n";
    stream << "  ";
    for (int i = 0; i < 24; ++i)
      stream << "0x" << QString("%1").arg(Globals::charToCode(text[i].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
    stream << "\n";
  }
  stream << "};\n\n";
  stream << "const uint8_t PTAD::Resources::messageSizes[] = {";
  counter = 0;
  for (auto message : Globals::messages.keys())
  {
    stream << Globals::messages[message].length();
    if (counter++ < Globals::messages.count() - 1)
      stream << ",";
  }
  stream << "};\n";
  stream << "//END\n\n";
  stream << "//BEGIN: Game resources\n";
  for (int i = 0; i < 7; ++i)
    stream << "const PTAD::Resources::StatGrowth PTAD::Resources::" << Globals::statNames[i] << "Growth = {" << Globals::statsGrowth[i].start << "," << Globals::statsGrowth[i].base << "," << Globals::statsGrowth[i].exponent << "};\n";
  stream << "\n";
  stream << "const uint8_t PTAD::Resources::skillLearned[99] =\n";
  stream << "{\n";
  for (int i = 0; i < 99; ++i)
  {
    if (i % 9 == 0)
      stream << "  ";
    stream << skillLearned[i];
    if (i < 98)
      stream << ",";
    if (i % 9 == 8)
      stream << "\n";
  }
  stream << "};\n";
  stream << "//END\n\n";
  stream << "//BEGIN: Global resources\n";
  stream << "const uint32_t PTAD::Resources::tilesets[]\n";
  stream << "{\n";
  for (auto tileset : Globals::tilesets)
    stream << "  DataPack::hash(\"/tilesets/" << tileset->getLocation() << "\"),\n";
  stream << "};\n\n";
  stream << "const uint32_t PTAD::Resources::tiledataFiles[] =\n";
  stream << "{\n";
  for (auto tileset : Globals::tilesets)
    stream << "  DataPack::hash(\"/tilesets/" << tileset->getLocation().replace(".png", ".dat") << "\"),\n";
  stream << "};\n\n";
  stream << "const uint8_t PTAD::Resources::playerSprite[] =\n";
  stream << "{\n";
  bytes = Image(QString("%1/sprites/player.png").arg(Globals::datadir), nullptr, false).toByteArray(16, 24);
  for (int i = 0; i < bytes.count(); ++i)
  {
    if (i % 64 == 0)
      stream << "  ";
    stream << "0x" << QString("%1").arg((uint8_t)bytes[i], 2, 16, QChar('0')).toUpper();
    if (i != bytes.count() - 1)
      stream << ",";
    if (i % 64 == 63)
      stream << "\n";
  }
  stream << "};\n\n";
  stream << "const uint8_t PTAD::Resources::cursor[] =\n";
  stream << "{\n";
  bytes = Image(QString("%1/cursor.png").arg(Globals::datadir), nullptr, false).toByteArray(16, 16);
  for (int i = 0; i < bytes.count(); ++i)
  {
    if (i % 16 == 0)
      stream << "  ";
    stream << "0x" << QString("%1").arg((uint8_t)bytes[i], 2, 16, QChar('0')).toUpper();
    if (i != bytes.count() - 1)
      stream << ",";
    if (i % 16 == 15)
      stream << "\n";
  }
  stream << "};\n\n";
  stream << "const uint8_t PTAD::Resources::statusIcons[] =\n";
  stream << "{\n";
  bytes = Image(QString("%1/statusIcons.png").arg(Globals::datadir), nullptr, false).toByteArray(16, 16);
  for (int i = 0; i < bytes.count(); ++i)
  {
    if (i % 32 == 0)
      stream << "  ";
    stream << "0x" << QString("%1").arg((uint8_t)bytes[i], 2, 16, QChar('0')).toUpper();
    if (i != bytes.count() - 1)
      stream << ",";
    if (i % 32 == 31)
      stream << "\n";
  }
  stream << "};\n\n";
  stream << "const PTAD::Resources::Item PTAD::Resources::items[8][16] =\n";
  stream << "{\n";
  for (int i = 0; i < 8; ++i)
  {
    stream << "  {//" << Globals::itemTypes[i] << "\n";
    for (int j = 0; j < 16; ++j)
    {
      QString name = Globals::items[i][j].name;
      QString description = Globals::items[i][j].description;
      name.resize(8, QChar('_'));
      description.resize(24, QChar('_'));
      stream << "    //" << name << "\n";
      stream << "    ";
      for (int k = 0; k < 8; ++k)
        stream << "0x" << QString("%1").arg(Globals::charToCode(name[k].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
      stream << "\n";
      stream << "    //" << description << "\n";
      stream << "    ";
      for (int k = 0; k < 24; ++k)
        stream << "0x" << QString("%1").arg(Globals::charToCode(description[k].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
      stream << "\n";
      stream << "    " << Globals::items[i][j].price << ",\n";
    }
    stream << "  }";
    if (i != 7)
      stream << ",";
    stream << "\n";
  }
  stream << "};\n\n";
  stream << "const PTAD::Resources::Item PTAD::Resources::skills[16] =\n";
  stream << "{\n";
  for (int i = 0; i < 16; ++i)
  {
    QString name = Globals::skills[i].name;
    QString description = Globals::skills[i].description;
    name.resize(8, QChar('_'));
    description.resize(24, QChar('_'));
    stream << "  //" << name << "\n";
    stream << "  ";
    for (int j = 0; j < 8; ++j)
      stream << "0x" << QString("%1").arg(Globals::charToCode(name[j].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
    stream << "\n";
    stream << "  //" << description << "\n";
    stream << "  ";
    for (int j = 0; j < 24; ++j)
      stream << "0x" << QString("%1").arg(Globals::charToCode(description[j].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
    stream << "\n";
    stream << "  " << Globals::skills[i].price << ",\n";
  }
  stream << "};\n\n";
  stream << "const PTAD::Resources::Item PTAD::Resources::spells[16] =\n";
  stream << "{\n";
  for (int i = 0; i < 16; ++i)
  {
    QString name = Globals::spells[i].name;
    QString description = Globals::spells[i].description;
    name.resize(8, QChar('_'));
    description.resize(24, QChar('_'));
    stream << "  //" << name << "\n";
    stream << "  ";
    for (int j = 0; j < 8; ++j)
      stream << "0x" << QString("%1").arg(Globals::charToCode(name[j].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
    stream << "\n";
    stream << "  //" << description << "\n";
    stream << "  ";
    for (int j = 0; j < 24; ++j)
      stream << "0x" << QString("%1").arg(Globals::charToCode(description[j].toLatin1()), 2, 16, QChar('0')).toUpper() << ",";
    stream << "\n";
    stream << "  " << Globals::spells[i].price << ",\n";
  }
  stream << "};\n\n";
  stream << "const PTAD::Resources::EquipmentStats PTAD::Resources::equipmentStats[6][16] =\n";
  stream << "{\n";
  for (int i = 0; i < 6; ++i)
  {
    stream << "  {//" << Globals::itemTypes[i + 2] << "\n";
    for (int j = 0; j < 16; ++j)
    {
      QString name = Globals::items[i + 2][j].name;
      name.resize(8, QChar('_'));
      stream << "    //" << name << "\n";
      stream << "    {{";
      for (int k = 0; k < 4; ++k)
      {
        stream << Globals::equipmentStats[i][j].stats[k];
        if (k != 3)
          stream << ", ";
      }
      stream << "}, 0x" << QString("%1").arg(Globals::equipmentStats[i][j].spellResistance, 4, 16, QChar('0')).toUpper() << ", {";
      stream << Globals::equipmentStats[i][j].statusResistance[0] << ", " << Globals::equipmentStats[i][j].statusResistance[1] << "}}";
      if (j != 15)
        stream << ",";
      stream << "\n";
    }
    stream << "  }";
    if (i != 5)
      stream << ",";
    stream << "\n";
  }
  stream << "};\n";
  stream << "//END\n\n";
  stream << "//BEGIN: Audio resources\n";
  stream << "const uint32_t PTAD::Resources::music[] =\n";
  stream << "{\n";
  for (auto bgm : Globals::bgms)
    stream << "  DataPack::hash(\"/music/" << bgm << ".ogg\"),\n";
  stream << "};\n\n";
  stream << "const uint32_t PTAD::Resources::sfx[] =\n";
  stream << "{\n";
  for (auto sfx : Globals::sfx)
    stream << " DataPack::hash(\"/sfx/" << sfx << ".wav\"),\n";
  stream << "};\n";
  stream << "//END\n";
  file.close();
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
