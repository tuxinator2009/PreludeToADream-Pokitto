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

#ifndef COMPILEDATA_H
#define COMPILEDATA_H

#include <QAudioFormat>
#include <QAudioDecoder>
#include <QByteArray>
#include <QFile>
#include <QProcess>
#include <QTreeWidgetItem>
#include <QVector>
#include "ui_compiledata.h"

class CompileData : public QDialog, public Ui::CompileData
{
  Q_OBJECT
  public:
    CompileData(QTreeWidgetItem *root, QWidget *parent=nullptr);
    ~CompileData();
  protected slots:
    void on_btnBrowseFemtoIDE_clicked();
    void on_btnCompileData_clicked();
    void on_btnCompileHW_clicked();
    void on_btnCompileSIM_clicked();
    void on_btnLaunchSIM_clicked();
    void audioBufferReady();
    void audioError(QAudioDecoder::Error error);
    void audioFinished();
    void processError(QProcess::ProcessError error);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processReadyReadStandardError();
    void processReadyReadStandardOutput();
  private:
    static const size_t BLOCK_SIZE = 512;
    struct FileHeader
    {
      uint32_t hash;
      uint32_t start;
      uint32_t end;
    };
    struct DataFile
    {
      DataFile() {}
      DataFile(QString l, QByteArray b)
      {
        location = l;
        bytes = b;
        size = b.size();
      }
      bool operator<(const DataFile &other) const {return bytes.size() < other.bytes.size();}
      QString location;
      QByteArray bytes;
      size_t size;
    };
    struct DataBlock
    {
      DataBlock()
      {
        availableBytes = BLOCK_SIZE;
        for (size_t i = 0; i < BLOCK_SIZE; ++i)
          bytes[i] = 0;
      }
      void write(QByteArray data)
      {
        char *pByte = bytes + (BLOCK_SIZE - availableBytes);
        for (int i = 0; i < data.size(); ++i)
          *pByte++ = data[i];
        availableBytes -= data.size();
      }
      void write(QFile &file) {file.write(bytes, BLOCK_SIZE);}
      size_t availableBytes;
      char bytes[BLOCK_SIZE];
    };
    bool compileAnimations();
    bool compileBackdrops();
    bool compileBattlers();
    bool compileMaps();
    bool compileMusic();
    bool compileScreens();
    bool compileSFX();
    bool compileSprites();
    bool compileTilesets();
    bool packData();
    bool generateResourcesHeader();
    bool generateResourcesSource();
    void outputText(QColor color, QString text);
    QTreeWidgetItem *rootMapItem;
    QProcess *process;
    QAudioFormat audioFormat;
    QAudioDecoder *audioDecoder;
    QByteArray audioData;
    QVector<DataFile> dataFiles;
    QVector<DataBlock> blocks;
    bool decoding;
    bool decodeError;
};

#endif //COMPILEDATA_H
