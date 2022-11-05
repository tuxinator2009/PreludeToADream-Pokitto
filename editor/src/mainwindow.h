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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioFormat>
#include <QAudioDecoder>
#include <QByteArray>
#include <type_traits>
#include "ui_mainwindow.h"
#include "image.h"
#include "map.h"
#include "mapflags.h"
#include "spritepicker.h"
#include "xmlParser.h"

#define DEBUG_MESSAGE "%s: %d\n", __FILE__, __LINE__

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget *parent=0);
    ~MainWindow();
	protected slots:
    void setup();
    void on_btnBrowseResources_clicked();
    void on_btnEditBattleAnimations_clicked();
    void on_btnEditItems_clicked();
    void on_btnEditMessages_clicked();
    void on_btnEditMonsters_clicked();
    void on_btnEditSkills_clicked();
    void on_btnEditSpells_clicked();
    void on_btnEditStats_clicked();
    void on_btnCompileData_clicked();
    void audioBufferReady();
    void audioError(QAudioDecoder::Error error);
    void audioFinished();
    void on_treeMaps_itemClicked(QTreeWidgetItem *item);
    void on_aAddTopLevelMap_triggered();
    void on_aAddChildMap_triggered();
    void on_aDeleteMap_triggered();
    void on_btnPreviewMap_clicked();
    void on_tblLayers_itemSelectionChanged();
    void on_tblLayers_itemChanged();
    void on_leName_textEdited(QString text);
    void on_numMinSteps_valueChanged(int value);
    void on_numMaxSteps_valueChanged(int value);
    void on_optMusic_currentIndexChanged(QString text);
    void on_btnPlayMusic_toggled(bool on);
    void on_btnFlags_clicked();
    void flagsChanged(uint8_t flags);
    void on_btnEditOnLoadEvent_clicked();
    void on_btnResizeMap_clicked();
    void on_btnShiftMap_clicked();
    void on_tblSprites_cellDoubleClicked();
    void on_tblSprites_itemSelectionChanged();
    void spritePicker_spriteClicked(uint8_t spriteID);
    void on_tblEnemies_itemSelectionChanged();
    void on_optTileset_currentIndexChanged(int index);
    void on_btnEditTileset_clicked();
    void on_btnAddTileset_clicked();
    void on_gvTileset_selectionChanged(QRect rect);
    void on_optZoom_currentIndexChanged(int index);
    void on_imgMapView_mousePressed(Qt::MouseButton button, QPoint pos);
    void on_imgMapView_mouseMoved(Qt::MouseButtons buttons, QPoint pos, QPoint delta);
    void on_imgMapView_mouseReleased(Qt::MouseButton button);
    void on_imgMapView_mouseDoubleClicked(Qt::MouseButton button, QPoint pos);
    void on_imgMapView_zoomOut(QPoint pos);
    void on_imgMapView_zoomIn(QPoint pos);
    void updateZoom();
    void redrawSprites();
    void redrawEnemies();
    void refreshTilesets();
	private:
    void loadMapTree(QTreeWidgetItem *parent, XMLNode mapNode);
    void saveMapTree(QTreeWidgetItem *parent, XMLNode mapNode);
    void renameMap(QTreeWidgetItem *item, QString name);
    void addMap(QTreeWidgetItem *parent);
    void deleteMap(QTreeWidgetItem *item);
    void closeEvent(QCloseEvent *event);
    void updateMapView();
    MapFlags *flags;
    SpritePicker *spritePicker;
    Image *mapImage;
    QPoint zoomPos;
    QAudioFormat audioFormat;
    QAudioDecoder *audioDecoder;
    QByteArray audioData;
    QList<QPoint> penPoints;
    QPoint rectStart;
    QPoint rectEnd;
    QRect tileSelection;
    QString currentMapFile;
    int action;
    static const int zoom[5];
    bool ignoreEvents;
};

#endif // MAINWINDOW_H
