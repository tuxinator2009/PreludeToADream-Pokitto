/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2020 Justin (tuxinator2009) Davis                                *
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

#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>
#include "battleanimationseditor.h"
#include "globals.h"
#include "image.h"
#include "mainwindow.h"
#include "map.h"
#include "mapevent.h"
#include "mapeventeditor.h"
#include "progresstracker.h"
#include "resources.h"
#include "tileset.h"
#include "tileseteditor.h"
#include "xmlParser.h"

const int MainWindow::zoom[5] = {64, 128, 256, 512, 1024};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  ignoreEvents = true;
  setupUi(this);
  mapImage = nullptr;
  progressTracker = nullptr;
  QTimer::singleShot(10, this, SLOT(setup()));
  ignoreEvents = false;
}

MainWindow::~MainWindow()
{
  Globals::freeData();
}

void MainWindow::setup()
{
  XMLNode settingsNode, childNode;
  XMLResults results;
  Globals::datadir = QCoreApplication::applicationDirPath() + "/../resources";
  for (int row = 0; row < 2; ++row)
  {
    for (int col = 0; col < 8; ++col)
    {
      QLabel *label = new QLabel();
      label->setMinimumSize(QSize(32, 48));
      label->setMaximumSize(QSize(32, 48));
      label->setStyleSheet("border: 0px solid rgb(114, 159, 207);");
      tblSprites->setCellWidget(row, col, label);
    }
  }
  for (int row = 0; row < 52; ++row)
  {
    for (int col = 0; col < 5; ++col)
    {
      QLabel *label = new QLabel();
      label->setMinimumSize(QSize(48, 48));
      label->setMaximumSize(QSize(48, 48));
      label->setStyleSheet("border: 0px solid rgb(114, 159, 207);");
      tblEnemies->setCellWidget(row, col, label);
    }
  }
  flags = new MapFlags(this);
  flags->setVisible(false);
  connect(flags, SIGNAL(flagsChanged(uint8_t)), this, SLOT(flagsChanged(uint8_t)));
  settingsNode = XMLNode::parseFile(QString("%1/settings.xml").arg(Globals::datadir).toLocal8Bit().data(), "ptad-settings", &results);
  if (results.error != XMLError::eXMLErrorNone)
    QMessageBox::critical(this, "XML Parser Error", QString("An error occurred when parsing settings.xml on line %1 column %2.\nError: %3").arg(results.nLine).arg(results.nColumn).arg(XMLNode::getError(results.error)));
  Globals::setupData();
  Globals::watchdog->addPath(QString("%1/tilesets").arg(Globals::datadir));
  connect(Globals::watchdog, SIGNAL(directoryChanged(const QString)), this, SLOT(refreshTilesets()));
  connect(Globals::battlers, SIGNAL(imageChanged()), this, SLOT(redrawEnemies()));
  connect(Globals::sprites, SIGNAL(imageChanged()), this, SLOT(redrawSprites()));
  treeMaps->invisibleRootItem()->setText(2, "/maps");
  loadMapTree(treeMaps->invisibleRootItem(), settingsNode.getChildNode("maps"));
  childNode = settingsNode.getChildNode("messages");
  for (int i = 0; i < childNode.nChildNode(); ++i)
  {
    XMLNode messageNode = childNode.getChildNode(i);
    Globals::messages[QString(messageNode.getName())] = QString(messageNode.getAttribute("message"));
  }
  redrawEnemies();
  refreshTilesets();
  frmMapView->setEnabled(false);
  wMapTools->setEnabled(false);
  stkLayerTools->setCurrentWidget(pgBlank);
  optMusic->addItems(Globals::bgms);
  spritePicker = new SpritePicker(this);
  spritePicker->setVisible(false);
  connect(spritePicker, SIGNAL(spriteClicked(uint8_t)), this, SLOT(spritePicker_spriteClicked(uint8_t)));
}

void MainWindow::on_btnEditResources_clicked()
{
  Resources *resourceEditor = new Resources(this);
  ignoreEvents = true;
  if (btnPlayMusic->isChecked())
  {
    Globals::audio->stop();
    btnPlayMusic->setIcon(QIcon(":/icons/play.png"));
    btnPlayMusic->setChecked(false);
  }
  resourceEditor->exec();
  resourceEditor->deleteLater();
  optMusic->clear();
  optMusic->addItems(Globals::bgms);
  if (Globals::map != nullptr)
    optMusic->setCurrentText(Globals::map->getBGM());
  ignoreEvents = false;
}

void MainWindow::on_btnEditBattleAnimations_clicked()
{
  BattleAnimationsEditor *editor = new BattleAnimationsEditor(this);
  ignoreEvents = true;
  if (btnPlayMusic->isChecked())
  {
    Globals::audio->stop();
    btnPlayMusic->setIcon(QIcon(":/icons/play.png"));
    btnPlayMusic->setChecked(false);
  }
  editor->exec();
  editor->deleteLater();
  ignoreEvents = false;
}

void MainWindow::on_btnCompileData_clicked()
{
  QString fileLocation = QString("%1/music/%2.ogg").arg(Globals::datadir).arg(Globals::bgms[0]);
  audioData.clear();
  audioFormat.setChannelCount(1);
  audioFormat.setCodec("audio/x-raw");
  audioFormat.setSampleType(QAudioFormat::UnSignedInt);
  audioFormat.setSampleRate(8000);
  audioFormat.setSampleSize(8);
  audioDecoder = new QAudioDecoder(this);
  connect(audioDecoder, SIGNAL(bufferReady()), this, SLOT(audioBufferReady()));
  connect(audioDecoder, SIGNAL(error(QAudioDecoder::Error)), this, SLOT(audioError(QAudioDecoder::Error)));
  connect(audioDecoder, SIGNAL(finished()), this, SLOT(audioFinished()));
  audioDecoder->setAudioFormat(audioFormat);
  printf("decoding: \"%s\"\n", fileLocation.toLocal8Bit().data());
  audioDecoder->setSourceFilename(fileLocation);
  audioDecoder->start();
}

void MainWindow::on_btnNotes_clicked()
{
}

void MainWindow::on_btnProgressTracker_clicked()
{
  if (progressTracker == nullptr)
    progressTracker = new ProgressTracker(this);
  progressTracker->show();
  progressTracker->raise();
  progressTracker->activateWindow();
}

void MainWindow::audioBufferReady()
{
  QAudioBuffer buffer = audioDecoder->read();
  audioData.append(buffer.data<char>(), buffer.byteCount());
}

void MainWindow::audioError(QAudioDecoder::Error error)
{
  printf("Decoding error %d: %s\n", static_cast<int>(error), audioDecoder->errorString().toLocal8Bit().data());
  audioDecoder->deleteLater();
}

void MainWindow::audioFinished()
{
  QFile output("test.raw");
  output.open(QFile::WriteOnly);
  output.write(audioData);
  output.close();
  audioDecoder->deleteLater();
  printf("Decoding finished\n");
}

void MainWindow::on_btnNewMap_clicked()
{
}

void MainWindow::on_btnDeleteMap_clicked()
{
}

void MainWindow::on_treeMaps_itemClicked(QTreeWidgetItem *item)
{
  XMLNode mapNode = XMLNode::openFileHelper(QString("%1/maps/%2").arg(Globals::datadir).arg(item->text(1)).toLocal8Bit().data(), "ptad-map");
  QStringList flagsList;
  uint8_t flags;
  if (Globals::map != nullptr)
    delete Globals::map;
  Globals::map = new Map(mapNode);
  frmMapView->setEnabled(true);
  wMapTools->setEnabled(true);
  redrawSprites();
  if (mapImage != nullptr)
    delete mapImage;
  mapImage = new Image(QSize(Globals::map->getWidth() * 256, Globals::map->getHeight() * 256));
  imgMapView->setSize(Globals::map->getWidth() * zoom[optZoom->currentIndex()], Globals::map->getHeight() * zoom[optZoom->currentIndex()]);
  imgMapView->setImage(mapImage);
  leName->setText(item->text(0));
  numMinSteps->setValue(Globals::map->getMinSteps());
  numMaxSteps->setValue(Globals::map->getMaxSteps());
  optMusic->setCurrentText(Globals::map->getBGM());
  flags = Globals::map->getFlags();
  if (flags & Map::FLAG_CLIP_UP)
    flagsList << "Up";
  if (flags & Map::FLAG_CLIP_DOWN)
    flagsList << "Down";
  if (flags & Map::FLAG_CLIP_LEFT)
    flagsList << "Left";
  if (flags & Map::FLAG_CLIP_RIGHT)
    flagsList << "Right";
  if (flags & Map::FLAG_DARK)
    flagsList << "Dark";
  if (flags & Map::FLAG_DREAM)
    flagsList << "Dream";
  if (flags & Map::FLAG_SPIRIT)
    flagsList << "Spirit";
  leFlags->setText(flagsList.join(","));
  optTileset->setCurrentIndex(Globals::map->getTilesetID());
  updateMapView();
}

void MainWindow::on_tblLayers_itemSelectionChanged()
{
  int row = tblLayers->currentRow();
  if (row == 0) //Grid
    stkLayerTools->setCurrentWidget(pgBlank);
  else if (row == 1) //Passability
    stkLayerTools->setCurrentWidget(pgBlank);
  else if (row == 2) //Regions
    stkLayerTools->setCurrentWidget(pgEnemies);
  else if (row == 3) //Foreground
    stkLayerTools->setCurrentWidget(pgTiles);
  else if (row == 4) //Events
    stkLayerTools->setCurrentWidget(pgSprites);
  else if (row == 5) //Background
    stkLayerTools->setCurrentWidget(pgTiles);
  updateMapView();
}

void MainWindow::on_tblLayers_itemChanged()
{
  updateMapView();
}

void MainWindow::on_leName_textEdited(QString text)
{
  renameMap(treeMaps->currentItem(), text);
}

void MainWindow::on_numMinSteps_valueChanged(int value)
{
  if (Globals::map != nullptr)
    Globals::map->setMinSteps(value);
}

void MainWindow::on_numMaxSteps_valueChanged(int value)
{
  if (Globals::map != nullptr)
    Globals::map->setMaxSteps(value);
}

void MainWindow::on_optMusic_currentIndexChanged(QString text)
{
  if (ignoreEvents)
    return;
  if (Globals::map != nullptr)
    Globals::map->setBGM(text);
}

void MainWindow::on_btnPlayMusic_toggled(bool on)
{
  if (on)
  {
    Globals::audio->setMedia(QUrl::fromLocalFile(QString("%1/music/%2.ogg").arg(Globals::datadir).arg(optMusic->currentText())));
    Globals::audio->play();
    if (Globals::audio->error() != QMediaPlayer::NoError)
      QMessageBox::critical(this, "Playback Failed", QString("Failed to play %1.wav\nReason: %2").arg(optMusic->currentText()).arg(Globals::audio->errorString()));
    btnPlayMusic->setIcon(QIcon(":/icons/stop.png"));
  }
  else
  {
    Globals::audio->stop();
    btnPlayMusic->setIcon(QIcon(":/icons/play.png"));
  }
}

void MainWindow::on_btnFlags_clicked()
{
  flags->move(btnFlags->mapTo(this, QPoint(btnFlags->width() - flags->width(), 0)));
  flags->show();
}

void MainWindow::flagsChanged(uint8_t flags)
{
  QStringList flagsList;
  if (flags & Map::FLAG_CLIP_UP)
    flagsList << "Up";
  if (flags & Map::FLAG_CLIP_DOWN)
    flagsList << "Down";
  if (flags & Map::FLAG_CLIP_LEFT)
    flagsList << "Left";
  if (flags & Map::FLAG_CLIP_RIGHT)
    flagsList << "Right";
  if (flags & Map::FLAG_DARK)
    flagsList << "Dark";
  if (flags & Map::FLAG_DREAM)
    flagsList << "Dream";
  if (flags & Map::FLAG_SPIRIT)
    flagsList << "Spirit";
  leFlags->setText(flagsList.join(","));
  //TODO update map flags
}

void MainWindow::on_btnEditOnLoadEvent_clicked()
{
  if (Globals::map != nullptr)
  {
    MapEventEditor *editor = new MapEventEditor(Globals::map->getOnLoadEvent(), true, this);
    editor->exec();
    editor->deleteLater();
  }
}

void MainWindow::on_tblSprites_cellDoubleClicked()
{
  spritePicker->move(tblSprites->mapTo(this, QPoint(tblSprites->width() - spritePicker->width(), 0)));
  spritePicker->show();
}

void MainWindow::on_tblSprites_itemSelectionChanged()
{
  for (int row = 0; row < 2; ++row)
  {
    for (int col = 0; col < 8; ++col)
    {
      QLabel *label = static_cast<QLabel*>(tblSprites->cellWidget(row, col));
      if (row == tblSprites->currentRow() && col == tblSprites->currentColumn())
        label->setStyleSheet("border: 1px solid rgb(114, 159, 207);");
      else
        label->setStyleSheet("border: 0px solid rgb(114, 159, 207);");
    }
  }
}

void MainWindow::spritePicker_spriteClicked(uint8_t spriteID)
{
  QLabel *label = static_cast<QLabel*>(tblSprites->cellWidget(tblSprites->currentRow(), tblSprites->currentColumn()));
  label->setPixmap(QPixmap::fromImage(Globals::sprites->toQImage(QRect(spriteID % 16 * 16, spriteID / 16 * 24, 16, 24))).scaled(32, 48));
  Globals::map->setSpriteID(tblSprites->currentRow() * 8 + tblSprites->currentColumn(), spriteID);
  spritePicker->hide();
  updateMapView();
}

void MainWindow::on_tblEnemies_itemSelectionChanged()
{
  if (ignoreEvents)
    return;
  for (int row = 0; row < 52; ++row)
  {
    for (int col = 0; col < 5; ++col)
    {
      QLabel *label = static_cast<QLabel*>(tblEnemies->cellWidget(row, col));
      if (row == tblEnemies->currentRow() && col == tblEnemies->currentColumn())
        label->setStyleSheet("border: 1px solid rgb(114, 159, 207);");
      else
        label->setStyleSheet("border: 0px solid rgb(114, 159, 207);");
    }
  }
}

void MainWindow::on_optTileset_currentIndexChanged(int index)
{
  if (ignoreEvents)
    return;
  if (Globals::map != nullptr)
    Globals::map->setTilesetID(index);
  gvTileset->changeTileset(index);
}

void MainWindow::on_btnEditTileset_clicked()
{
  TilesetEditor *editor = new TilesetEditor(Globals::tilesets[optTileset->currentIndex()], this);
  editor->exec();
  editor->deleteLater();
}

void MainWindow::on_gvTileset_selectionChanged(QRect rect)
{
  tileSelection = rect;
}

void MainWindow::on_optZoom_currentIndexChanged(int index)
{
  imgMapView->setSize(Globals::map->getWidth() * zoom[index], Globals::map->getHeight() * zoom[index]);
  imgMapView->imageChanged();
}

void MainWindow::on_imgMapView_mousePressed(Qt::MouseButton button, QPoint pos)
{
  int currentZoom = zoom[optZoom->currentIndex()];
  if (button == Qt::LeftButton)
  {
    if (tblLayers->currentRow() == Map::LAYER_PASSABILITY)
    {
      Globals::map->togglePassability(pos.x() / (currentZoom / 16), pos.y() / (currentZoom / 16));
      action = Globals::map->getPassability(pos.x() / (currentZoom / 16), pos.y() / (currentZoom / 16));
    }
    else if (tblLayers->currentRow() == Map::LAYER_REGIONS)
    {
      int regionID = pos.y() / (currentZoom / 2) * Globals::map->getWidth() * 2 + pos.x() / (currentZoom / 2);
      int regionX = pos.x() % (currentZoom / 2);
      int regionY = pos.y() % (currentZoom / 2);
      int num = -1;
      if (regionX >= 8 * currentZoom / 256 && regionX < 56 * currentZoom / 256)
      {
        if (regionY >= 8 * currentZoom / 256 && regionY < 56 * currentZoom / 256)
          num = 0;
        else if (regionY >= 72 * currentZoom / 256 && regionY < 120 * currentZoom / 256)
          num = 2;
      }
      else if (regionX >= 72 * currentZoom / 256 && regionX < 120 * currentZoom / 256)
      {
        if (regionY >= 8 * currentZoom / 256 && regionY < 56 * currentZoom / 256)
          num = 1;
        else if (regionY >= 72 * currentZoom / 256 && regionY < 120 * currentZoom / 256)
          num = 3;
      }
      if (num != -1)
        Globals::map->setRegionMonster(regionID, num, tblEnemies->currentRow() * 5 + tblEnemies->currentColumn());
    }
    else if (tblLayers->currentRow() == Map::LAYER_FOREGROUND)
    {
      int tileX = pos.x() / (currentZoom / 32);
      int tileY = pos.y() / (currentZoom / 32);
      if (chkSnapToSubGrid->isChecked())
      {
        tileX = (tileX & ~1) | (tileSelection.x() % 2);
        tileY = (tileY & ~1) | (tileSelection.y() % 2);
      }
      if (btnToolPencil->isChecked())
      {
        rectStart = QPoint(tileX, tileY);
        penPoints += QPoint(tileX, tileY);
      }
      else if (btnToolRectangle->isChecked())
      {
        rectStart = QPoint(tileX, tileY);
        rectEnd = QPoint(tileX, tileY);
      }
      else if (btnToolCircle->isChecked())
      {
        rectStart = QPoint(tileX, tileY);
        rectEnd = QPoint(tileX, tileY);
      }
      action = 1;
    }
    else if (tblLayers->currentRow() == Map::LAYER_EVENTS)
    {
      action = -1;
      for (int i = 0; i < 29; ++i)
      {
        MapEvent *event = Globals::map->getMapEvent(i);
        if (event == nullptr)
          break;
        int eventX = event->getX() * currentZoom / 16;
        int eventY = event->getY() * currentZoom / 16 + (event->isFlagSet(MapEvent::FLAGS_OFFSET) ? 0:(currentZoom / 32) - (currentZoom / 32));
        if (pos.x() >= eventX && pos.x() < eventX + 16 * currentZoom / 256 && pos.y() >= eventY && pos.y() < eventY + 24 * currentZoom / 256)
        {
          action = i;
          break;
        }
      }
    }
    else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND)
    {
      int tileX = pos.x() / (currentZoom / 32);
      int tileY = pos.y() / (currentZoom / 32);
      if (chkSnapToSubGrid->isChecked())
      {
        tileX = (tileX & ~1) | (tileSelection.x() % 2);
        tileY = (tileY & ~1) | (tileSelection.y() % 2);
      }
      if (btnToolPencil->isChecked())
      {
        rectStart = QPoint(tileX, tileY);
        penPoints += QPoint(tileX, tileY);
      }
      else if (btnToolRectangle->isChecked())
      {
        rectStart = QPoint(tileX, tileY);
        rectEnd = QPoint(tileX, tileY);
      }
      else if (btnToolCircle->isChecked())
      {
        rectStart = QPoint(tileX, tileY);
        rectEnd = QPoint(tileX, tileY);
      }
      action = 1;
    }
    updateMapView();
  }
  else if (button == Qt::MiddleButton && tblLayers->currentRow() == Map::LAYER_EVENTS)
  {
    for (int i = 0; i < 29; ++i)
    {
      MapEvent *event = Globals::map->getMapEvent(i);
      if (event == nullptr)
        break;
      int eventX = event->getX() * currentZoom / 16;
      int eventY = event->getY() * currentZoom / 16 + (event->isFlagSet(MapEvent::FLAGS_OFFSET) ? 0:(currentZoom / 32) - (currentZoom / 32));
      if (pos.x() >= eventX && pos.x() < eventX + 16 * currentZoom / 256 && pos.y() >= eventY && pos.y() < eventY + 24 * currentZoom / 256)
      {
        event->setSpriteID(tblSprites->currentRow() * 8 + tblSprites->currentColumn());
        break;
      }
    }
  }
  else if (button == Qt::RightButton && action == 1 && (tblLayers->currentRow() == Map::LAYER_FOREGROUND || tblLayers->currentRow() == Map::LAYER_BACKGROUND))
  {
    penPoints.clear();
    action = 0;
    updateMapView();
  }
}

void MainWindow::on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta)
{
  int currentZoom = zoom[optZoom->currentIndex()];
  if (buttons & Qt::LeftButton)
  {
    if (tblLayers->currentRow() == Map::LAYER_PASSABILITY)
    {
      if (action)
        Globals::map->setPassability(pos.x() / (currentZoom / 16), pos.y() / (currentZoom / 16));
      else
        Globals::map->clearPassability(pos.x() / (currentZoom / 16), pos.y() / (currentZoom / 16));
    }
    else if (tblLayers->currentRow() == Map::LAYER_REGIONS)
    {
      int regionID = pos.y() / (currentZoom / 2) * Globals::map->getWidth() * 2 + pos.x() / (currentZoom / 2);
      int regionX = pos.x() % (currentZoom / 2);
      int regionY = pos.y() % (currentZoom / 2);
      int num = -1;
      if (regionX >= 8 * currentZoom / 256 && regionX < 56 * currentZoom / 256)
      {
        if (regionY >= 8 * currentZoom / 256 && regionY < 56 * currentZoom / 256)
          num = 0;
        else if (regionY >= 72 * currentZoom / 256 && regionY < 120 * currentZoom / 256)
          num = 2;
      }
      else if (regionX >= 72 * currentZoom / 256 && regionX < 120 * currentZoom / 256)
      {
        if (regionY >= 8 * currentZoom / 256 && regionY < 56 * currentZoom / 256)
          num = 1;
        else if (regionY >= 72 * currentZoom / 256 && regionY < 120 * currentZoom / 256)
          num = 3;
      }
      if (num != -1)
        Globals::map->setRegionMonster(regionID, num, tblEnemies->currentRow() * 5 + tblEnemies->currentColumn());
    }
    else if (tblLayers->currentRow() == Map::LAYER_FOREGROUND && action == 1)
    {
      int tileX = pos.x() / (currentZoom / 32);
      int tileY = pos.y() / (currentZoom / 32);
      if (btnToolPencil->isChecked() && !penPoints.contains(QPoint(tileX, tileY)))
      {
        if (chkSnapToSubGrid->isChecked())
        {
          tileX = (tileX & ~1) | (tileSelection.x() % 2);
          tileY = (tileY & ~1) | (tileSelection.y() % 2);
        }
        penPoints += QPoint(tileX, tileY);
      }
      else if (btnToolRectangle->isChecked())
        rectEnd = QPoint(tileX, tileY);
      else if (btnToolCircle->isChecked())
        rectEnd = QPoint(tileX, tileY);
    }
    else if (tblLayers->currentRow() == Map::LAYER_EVENTS)
    {
      if (action != -1)
      {
        MapEvent *event = Globals::map->getMapEvent(action);
        event->setFlag(MapEvent::FLAGS_OFFSET, pos.y() % (currentZoom / 16) < (currentZoom / 32));
        event->setX(pos.x() / (currentZoom / 16));
        event->setY(pos.y() / (currentZoom / 16));
      }
    }
    else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND && action == 1)
    {
      int tileX = pos.x() / (currentZoom / 32);
      int tileY = pos.y() / (currentZoom / 32);
      if (btnToolPencil->isChecked() && !penPoints.contains(QPoint(tileX, tileY)))
      {
        if (chkSnapToSubGrid->isChecked())
        {
          tileX = (tileX & ~1) | (tileSelection.x() % 2);
          tileY = (tileY & ~1) | (tileSelection.y() % 2);
        }
        penPoints += QPoint(tileX, tileY);
      }
      else if (btnToolRectangle->isChecked())
        rectEnd = QPoint(tileX, tileY);
      else if (btnToolCircle->isChecked())
        rectEnd = QPoint(tileX, tileY);
    }
    updateMapView();
  }
  else if (buttons & Qt::RightButton)
  {
    QScrollBar *scrollBar = panMapArea->horizontalScrollBar();
    scrollBar->setValue(scrollBar->value() - delta.x());
    scrollBar = panMapArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->value() - delta.y());
  }
}

void MainWindow::on_imgMapView_mouseReleased(Qt::MouseButton button)
{
  if (button == Qt::LeftButton && action == 1 && (tblLayers->currentRow() == Map::LAYER_FOREGROUND || tblLayers->currentRow() == Map::LAYER_BACKGROUND))
  {
    if (btnToolPencil->isChecked())
    {
      for (int i = 0; i < penPoints.count(); ++i)
      {
        int tileX = penPoints[i].x() - rectStart.x();
        int tileY = penPoints[i].y() - rectStart.y();
        while (tileX < 0)
          tileX += tileSelection.width();
        while (tileY < 0)
          tileY += tileSelection.height();
        for (int y = 0; y < tileSelection.height(); ++y)
        {
          for (int x = 0; x < tileSelection.width(); ++x)
          {
            uint8_t tileID = ((tileY + y) % tileSelection.height() + tileSelection.y()) * 16 + (tileX + x) % tileSelection.width() + tileSelection.x();
            if (tblLayers->currentRow() == Map::LAYER_FOREGROUND)
              Globals::map->setFGTile(penPoints[i].x() + x, penPoints[i].y() + y, tileID);
            else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND)
              Globals::map->setBGTile(penPoints[i].x() + x, penPoints[i].y() + y, tileID);
          }
        }
      }
    }
    else if (btnToolRectangle->isChecked())
    {
      int startX, startY;
      int endX, endY;
      if (rectStart.x() < rectEnd.x())
      {
        startX = rectStart.x();
        endX = rectEnd.x();
      }
      else
      {
        startX = rectEnd.x();
        endX = rectStart.x();
      }
      if (rectStart.y() < rectEnd.y())
      {
        startY = rectStart.y();
        endY = rectEnd.y();
      }
      else
      {
        startY = rectEnd.y();
        endY = rectStart.y();
      }
      for (int y = startY; y <= endY; ++y)
      {
        for (int x = startX; x <= endX; ++x)
        {
          uint8_t tileID = ((y - startY) % tileSelection.height() + tileSelection.y()) * 16 + (x - startX) % tileSelection.width() + tileSelection.x();
          if (tblLayers->currentRow() == Map::LAYER_FOREGROUND)
            Globals::map->setFGTile(x, y, tileID);
          else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND)
            Globals::map->setBGTile(x, y, tileID);
        }
      }
    }
    else if (btnToolCircle->isChecked())
    {
      int startX, startY;
      int endX, endY;
      if (rectStart.x() < rectEnd.x())
      {
        startX = rectStart.x();
        endX = rectEnd.x();
      }
      else
      {
        startX = rectEnd.x();
        endX = rectStart.x();
      }
      if (rectStart.y() < rectEnd.y())
      {
        startY = rectStart.y();
        endY = rectEnd.y();
      }
      else
      {
        startY = rectEnd.y();
        endY = rectStart.y();
      }
      float rx = (endX - startX + 1) / 2.0;
      float ry = (endY - startY + 1) / 2.0;
      float xc = startX + rx;
      float yc = startY + ry;
      float dx, dy, d1, d2, x, y;
      int tileX, tileY, tileID;
      x = 0;
      y = ry;
      d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
      dx = 2 * ry * ry * x;
      dy = 2 * rx * rx * y;
      while (dx < dy)
      {
        for (tileX = qMax((int)(-x + xc), startX); tileX <= qMin((int)(x + xc), endX); ++tileX)
        {
          tileY = y + yc;
          tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
          if (tblLayers->currentRow() == Map::LAYER_FOREGROUND)
            Globals::map->setFGTile(tileX, tileY, tileID);
          else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND)
            Globals::map->setBGTile(tileX, tileY, tileID);
          tileY = -y + yc;
          tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
          if (tblLayers->currentRow() == Map::LAYER_FOREGROUND)
            Globals::map->setFGTile(tileX, tileY, tileID);
          else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND)
            Globals::map->setBGTile(tileX, tileY, tileID);
        }
        if (d1 < 0)
        {
          ++x;
          dx = dx + (2 * ry * ry);
          d1 = d1 + dx + (ry * ry);
        }
        else
        {
          ++x;
          --y;
          dx = dx + (2 * ry * ry);
          dy = dy - (2 * rx * rx);
          d1 = d1 + dx - dy + (ry * ry);
        }
      }
      d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);
      while (y >= 0)
      {
        for (tileX = qMax((int)(-x + xc), startX); tileX <= qMin((int)(x + xc), endX); ++tileX)
        {
          tileY = y + yc;
          tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
          if (tblLayers->currentRow() == Map::LAYER_FOREGROUND)
            Globals::map->setFGTile(tileX, tileY, tileID);
          else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND)
            Globals::map->setBGTile(tileX, tileY, tileID);
          tileY = -y + yc;
          tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
          if (tblLayers->currentRow() == Map::LAYER_FOREGROUND)
            Globals::map->setFGTile(tileX, tileY, tileID);
          else if (tblLayers->currentRow() == Map::LAYER_BACKGROUND)
            Globals::map->setBGTile(tileX, tileY, tileID);
        }
        if (d2 > 0)
        {
          --y;
          dy = dy - (2 * rx * rx);
          d2 = d2 + (rx * rx) - dy;
        }
        else
        {
          --y;
          ++x;
          dx = dx + (2 * ry * ry);
          dy = dy - (2 * rx * rx);
          d2 = d2 + dx - dy + (rx * rx);
        }
      }
    }
    penPoints.clear();
    action = 0;
    updateMapView();
  }
}

void MainWindow::on_imgMapView_mouseDoubleClicked(Qt::MouseButton button, QPoint pos)
{
  int currentZoom = zoom[optZoom->currentIndex()];
  if (Globals::map == nullptr)
    return;
  if (button == Qt::LeftButton && tblLayers->currentRow() == Map::LAYER_EVENTS)
  {
    MapEvent *event = nullptr;
    for (int i = 0; i < 29; ++i)
    {
      event = Globals::map->getMapEvent(i);
      if (event == nullptr)
        break;
      int eventX = event->getX() * currentZoom / 16;
      int eventY = event->getY() * currentZoom / 16 + (event->isFlagSet(MapEvent::FLAGS_OFFSET) ? 0:(currentZoom / 32) - (currentZoom / 32));
      if (pos.x() >= eventX && pos.x() < eventX + 16 * currentZoom / 256 && pos.y() >= eventY && pos.y() < eventY + 24 * currentZoom / 256)
      {
        MapEventEditor *editor = new MapEventEditor(event, false, this);
        editor->exec();
        editor->deleteLater();
        break;
      }
    }
    if (event == nullptr && Globals::map->numMapEvents() < 29)
    {
      MapEventEditor *editor;
      event = new MapEvent(Globals::map);
      event->setFlag(MapEvent::FLAGS_OFFSET, pos.y() % (currentZoom / 16) < (currentZoom / 32));
      event->setX(pos.x() / (currentZoom / 16));
      event->setY(pos.y() / (currentZoom / 16));
      event->setSpriteID(tblSprites->currentRow() * 8 + tblSprites->currentColumn());
      Globals::map->setMapEvent(Globals::map->numMapEvents(), event);
      editor = new MapEventEditor(event, false, this);
      editor->exec();
      editor->deleteLater();
    }
  }
  updateMapView();
}

void MainWindow::on_imgMapView_zoomOut(QPoint pos)
{
  if (optZoom->currentIndex() == 0)
    return;
  optZoom->setCurrentIndex(optZoom->currentIndex() - 1);
  imgMapView->setSize(Globals::map->getWidth() * zoom[optZoom->currentIndex()], Globals::map->getHeight() * zoom[optZoom->currentIndex()]);
  imgMapView->imageChanged();
  zoomPos = (pos / 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void MainWindow::on_imgMapView_zoomIn(QPoint pos)
{
  if (optZoom->currentIndex() == 4)
    return;
  optZoom->setCurrentIndex(optZoom->currentIndex() + 1);
  imgMapView->setSize(Globals::map->getWidth() * zoom[optZoom->currentIndex()], Globals::map->getHeight() * zoom[optZoom->currentIndex()]);
  zoomPos = (pos * 2) - (pos - QPoint(panMapArea->horizontalScrollBar()->value(), panMapArea->verticalScrollBar()->value()));
  QTimer::singleShot(10, this, SLOT(updateZoom()));
}

void MainWindow::updateZoom()
{
  panMapArea->horizontalScrollBar()->setValue(zoomPos.x());
  panMapArea->verticalScrollBar()->setValue(zoomPos.y());
}

void MainWindow::redrawSprites()
{
  for (int sprite = 0; sprite < 16; ++sprite)
  {
    QLabel *label = static_cast<QLabel*>(tblSprites->cellWidget(sprite / 8, sprite % 8));
    uint8_t spriteID = Globals::map->getSpriteID(sprite);
    label->setPixmap(QPixmap::fromImage(Globals::sprites->toQImage(QRect(spriteID % 16 * 16, spriteID / 16 * 24, 16, 24))).scaled(32, 48));
  }
}

void MainWindow::redrawEnemies()
{
  for (int enemy = 0; enemy < 256; ++enemy)
  {
    QLabel *label = static_cast<QLabel*>(tblEnemies->cellWidget(enemy / 5, enemy % 5));
    label->setPixmap(QPixmap::fromImage(Globals::battlers->toQImage(QRect(enemy % 16 * 48, enemy / 16 * 48, 48, 48))));
  }
}

void MainWindow::refreshTilesets()
{
  optTileset->clear();
  for (int i = 0; i < Globals::tilesets.count(); ++i)
    optTileset->addItem(Globals::tilesets[i]->getName());
}

void MainWindow::loadMapTree(QTreeWidgetItem *parent, XMLNode mapNode)
{
  XMLNode childNode;
  for (int i = 0; i < mapNode.nChildNode(); ++i)
  {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    childNode = mapNode.getChildNode(i);
    item->setText(0, childNode.getAttribute("name"));
    item->setText(1, childNode.getAttribute("file"));
    item->setText(2, QString("%1/%2").arg(parent->text(2)).arg(item->text(0)));
    Globals::mapNames.insert(item->text(1).mid(3, 4).toInt(), item->text(2));
    loadMapTree(item, childNode);
  }
}

void MainWindow::saveMapTree(QTreeWidgetItem *parent, XMLNode mapNode)
{
  XMLNode childNode;
  for (int i = 0; i < parent->childCount(); ++i)
  {
    QTreeWidgetItem *childItem = parent->child(i);
    childNode = mapNode.addChild("map");
    childNode.addAttribute("name", childItem->text(0).toLocal8Bit().data());
    childNode.addAttribute("file", childItem->text(1).toLocal8Bit().data());
    saveMapTree(childItem, childNode);
  }
}

void MainWindow::renameMap(QTreeWidgetItem *item, QString name)
{
  QList<QTreeWidgetItem*> items;
  items += item;
  item->setText(0, name);
  while (items.count() > 0)
  {
    QTreeWidgetItem *current = items.takeFirst();
    if (current->parent() == nullptr)
      current->setText(2, QString("/maps/%2").arg(current->text(0)));
    else
      current->setText(2, QString("%1/%2").arg(current->parent()->text(2)).arg(current->text(0)));
    Globals::mapNames[current->text(1).mid(3, 4).toInt()] = current->text(2);
    for (int i = 0; i < current->childCount(); ++i)
      items += current->child(i);
  }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  XMLNode settingsNode = XMLNode::createXMLTopNode("ptad-settings");
  XMLNode childNode = settingsNode.addChild("maps");
  saveMapTree(treeMaps->invisibleRootItem(), childNode);
  childNode = settingsNode.addChild("messages");
  for (auto message : Globals::messages.keys())
    childNode.addChild(message.toLocal8Bit().data()).addAttribute("message", Globals::messages[message].toLocal8Bit().data());
  //printf("%s\n", settingsNode.createXMLString());
  //settingsNode.writeToFile(QString("%1/settings.xml").arg(Globals::datadir).toLocal8Bit().data());
  Globals::saveData();
  Globals::freeData();
  QMainWindow::closeEvent(event);
}

void MainWindow::updateMapView()
{
  Image *tilesetImage = Globals::tilesets[Globals::map->getTilesetID()]->getImage();
  Image passability(":/images/passability", nullptr, false);
  if (mapImage == nullptr)
    return;
  if (tblLayers->item(Map::LAYER_BACKGROUND, 0)->checkState() == Qt::Checked)
  {
    for (int y = 0; y < Globals::map->getHeight() * 32; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 32; ++x)
      {
        uint8_t tileID = Globals::map->getBGTile(x, y);
        if (tileID < 128)
          mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), false);
      }
    }
    if (tblLayers->currentRow() == Map::LAYER_BACKGROUND && action == 1)
    {
      if (btnToolPencil->isChecked())
      {
        for (int i = 0; i < penPoints.count(); ++i)
        {
          int tileX = penPoints[i].x() - rectStart.x();
          int tileY = penPoints[i].y() - rectStart.y();
          while (tileX < 0)
            tileX += tileSelection.width();
          while (tileY < 0)
            tileY += tileSelection.height();
          for (int y = 0; y < tileSelection.height(); ++y)
          {
            for (int x = 0; x < tileSelection.width(); ++x)
            {
              uint8_t tileID = ((tileY + y) % tileSelection.height() + tileSelection.y()) * 16 + (tileX + x) % tileSelection.width() + tileSelection.x();
              mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint((penPoints[i].x() + x) * 8, (penPoints[i].y() + y) * 8), false);
            }
          }
        }
      }
      else if (btnToolRectangle->isChecked())
      {
        int startX, startY;
        int endX, endY;
        if (rectStart.x() < rectEnd.x())
        {
          startX = rectStart.x();
          endX = rectEnd.x();
        }
        else
        {
          startX = rectEnd.x();
          endX = rectStart.x();
        }
        if (rectStart.y() < rectEnd.y())
        {
          startY = rectStart.y();
          endY = rectEnd.y();
        }
        else
        {
          startY = rectEnd.y();
          endY = rectStart.y();
        }
        for (int y = startY; y <= endY; ++y)
        {
          for (int x = startX; x <= endX; ++x)
          {
            uint8_t tileID = ((y - startY) % tileSelection.height() + tileSelection.y()) * 16 + (x - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), false);
          }
        }
      }
      else if (btnToolCircle->isChecked())
      {
        int startX, startY;
        int endX, endY;
        if (rectStart.x() < rectEnd.x())
        {
          startX = rectStart.x();
          endX = rectEnd.x();
        }
        else
        {
          startX = rectEnd.x();
          endX = rectStart.x();
        }
        if (rectStart.y() < rectEnd.y())
        {
          startY = rectStart.y();
          endY = rectEnd.y();
        }
        else
        {
          startY = rectEnd.y();
          endY = rectStart.y();
        }
        float rx = (endX - startX + 1) / 2.0;
        float ry = (endY - startY + 1) / 2.0;
        float xc = startX + rx;
        float yc = startY + ry;
        float dx, dy, d1, d2, x, y;
        int tileX, tileY, tileID;
        x = 0;
        y = ry;
        d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
        dx = 2 * ry * ry * x;
        dy = 2 * rx * rx * y;
        while (dx < dy)
        {
          for (tileX = qMax((int)(-x + xc), startX); tileX <= qMin((int)(x + xc), endX); ++tileX)
          {
            tileY = y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
            tileY = -y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
          }
          if (d1 < 0)
          {
            ++x;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
          }
          else
          {
            ++x;
            --y;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
          }
        }
        d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);
        while (y >= 0)
        {
          for (tileX = qMax((int)(-x + xc), startX); tileX <= qMin((int)(x + xc), endX); ++tileX)
          {
            tileY = y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
            tileY = -y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
          }
          if (d2 > 0)
          {
            --y;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
          }
          else
          {
            --y;
            ++x;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
          }
        }
      }
    }
  }
  if (tblLayers->item(Map::LAYER_EVENTS, 0)->checkState() == Qt::Checked)
  {
    for (int i = 0; i < 29; ++i)
    {
      MapEvent *event = Globals::map->getMapEvent(i);
      if (event == nullptr)
        break;
      uint8_t spriteID = Globals::map->getSpriteID(event->getSpriteID());
      mapImage->blitImage(*Globals::sprites, QRect(spriteID % 16 * 16, spriteID / 16 * 24, 16, 24), QPoint(event->getX() * 16, event->getY() * 16 + (event->isFlagSet(MapEvent::FLAGS_OFFSET) ? 0:8) - 8), true);
    }
  }
  if (tblLayers->item(Map::LAYER_FOREGROUND, 0)->checkState() == Qt::Checked)
  {
    for (int y = 0; y < Globals::map->getHeight() * 32; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 32; ++x)
      {
        uint8_t tileID = Globals::map->getFGTile(x, y);
        if (tileID < 128)
          mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), true);
      }
    }
    if (tblLayers->currentRow() == Map::LAYER_FOREGROUND && action == 1)
    {
      if (btnToolPencil->isChecked())
      {
        for (int i = 0; i < penPoints.count(); ++i)
        {
          int tileX = penPoints[i].x() - rectStart.x();
          int tileY = penPoints[i].y() - rectStart.y();
          while (tileX < 0)
            tileX += tileSelection.width();
          while (tileY < 0)
            tileY += tileSelection.height();
          for (int y = 0; y < tileSelection.height(); ++y)
          {
            for (int x = 0; x < tileSelection.width(); ++x)
            {
              uint8_t tileID = ((tileY + y) % tileSelection.height() + tileSelection.y()) * 16 + (tileX + x) % tileSelection.width() + tileSelection.x();
              mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint((penPoints[i].x() + x) * 8, (penPoints[i].y() + y) * 8), true);
            }
          }
        }
      }
      else if (btnToolRectangle->isChecked())
      {
        int startX, startY;
        int endX, endY;
        if (rectStart.x() < rectEnd.x())
        {
          startX = rectStart.x();
          endX = rectEnd.x();
        }
        else
        {
          startX = rectEnd.x();
          endX = rectStart.x();
        }
        if (rectStart.y() < rectEnd.y())
        {
          startY = rectStart.y();
          endY = rectEnd.y();
        }
        else
        {
          startY = rectEnd.y();
          endY = rectStart.y();
        }
        for (int y = startY; y <= endY; ++y)
        {
          for (int x = startX; x <= endX; ++x)
          {
            uint8_t tileID = ((y - startY) % tileSelection.height() + tileSelection.y()) * 16 + (x - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(x * 8, y * 8), true);
          }
        }
      }
      else if (btnToolCircle->isChecked())
      {
        int startX, startY;
        int endX, endY;
        if (rectStart.x() < rectEnd.x())
        {
          startX = rectStart.x();
          endX = rectEnd.x();
        }
        else
        {
          startX = rectEnd.x();
          endX = rectStart.x();
        }
        if (rectStart.y() < rectEnd.y())
        {
          startY = rectStart.y();
          endY = rectEnd.y();
        }
        else
        {
          startY = rectEnd.y();
          endY = rectStart.y();
        }
        float rx = (endX - startX + 1) / 2.0;
        float ry = (endY - startY + 1) / 2.0;
        float xc = startX + rx;
        float yc = startY + ry;
        float dx, dy, d1, d2, x, y;
        int tileX, tileY, tileID;
        x = 0;
        y = ry;
        d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
        dx = 2 * ry * ry * x;
        dy = 2 * rx * rx * y;
        while (dx < dy)
        {
          for (tileX = qMax((int)(-x + xc), startX); tileX <= qMin((int)(x + xc), endX); ++tileX)
          {
            tileY = y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
            tileY = -y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
          }
          if (d1 < 0)
          {
            ++x;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
          }
          else
          {
            ++x;
            --y;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
          }
        }
        d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);
        while (y >= 0)
        {
          for (tileX = qMax((int)(-x + xc), startX); tileX <= qMin((int)(x + xc), endX); ++tileX)
          {
            tileY = y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
            tileY = -y + yc;
            tileID = ((tileY - startY) % tileSelection.height() + tileSelection.y()) * 16 + (tileX - startX) % tileSelection.width() + tileSelection.x();
            mapImage->blitImage(*tilesetImage, QRect(tileID % 16 * 8, tileID / 16 * 8, 8, 8), QPoint(tileX * 8, tileY * 8), true);
          }
          if (d2 > 0)
          {
            --y;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
          }
          else
          {
            --y;
            ++x;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
          }
        }
      }
    }
  }
  if (tblLayers->item(Map::LAYER_REGIONS, 0)->checkState() == Qt::Checked)
  {
    for (int y = 0; y < Globals::map->getHeight() * 2; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 2; ++x)
      {
        mapImage->drawRect(QRect(x * 128, y * 128, 128, 128), 4);
        for (int i = 0; i < 4; ++i)
        {
          uint8_t monsterID = Globals::map->getRegionMonster(y * 8 + x, i);
          if (monsterID != 255)
            mapImage->blitImage(*Globals::battlers, QRect(monsterID % 16 * 48, monsterID / 16 * 48, 48, 48), QPoint(x * 128 + i % 2 * 64 + 8, y * 128 + i / 2 * 64 + 8), true);
          if (tblLayers->currentRow() == Map::LAYER_REGIONS)
            mapImage->drawRect(QRect(x * 128 + i % 2 * 64 + 8, y * 128 + i / 2 * 64 + 8, 48, 48), 247);
        }
      }
    }
  }
  if (tblLayers->item(Map::LAYER_PASSABILITY, 0)->checkState() == Qt::Checked)
  {
    for (int y = 0; y < Globals::map->getHeight() * 16; ++y)
    {
      for (int x = 0; x < Globals::map->getWidth() * 16; ++x)
      {
        if (Globals::map->getPassability(x, y))
          mapImage->blitImage(passability, QRect(0, 0, 32, 32), QPoint(x * 16, y * 16), true);
      }
    }
  }
  if (tblLayers->item(Map::LAYER_GRID, 0)->checkState() == Qt::Checked)
  {
    for (int x = 0; x < Globals::map->getWidth() * 16; ++x)
      mapImage->drawVLine(QPoint(x * 16 + 15, 0), mapImage->getHeight(), 231);
    for (int y = 0; y < Globals::map->getHeight() * 16; ++y)
      mapImage->drawHLine(QPoint(0, y * 16 + 15), mapImage->getWidth(), 231);
  }
  if (tblLayers->currentRow() == Map::LAYER_EVENTS)
  {
    for (int i = 0; i < 29; ++i)
    {
      MapEvent *event = Globals::map->getMapEvent(i);
      if (event == nullptr)
        break;
      mapImage->drawRect(QRect(event->getX() * 16, event->getY() * 16 + (event->isFlagSet(MapEvent::FLAGS_OFFSET) ? 0:8) - 8, 16, 24), 247);
    }
  }
  imgMapView->imageChanged();
}
