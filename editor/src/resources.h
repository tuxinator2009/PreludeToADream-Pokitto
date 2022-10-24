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

#ifndef RESOURCES_H
#define RESOURCES_H

#include <type_traits>
#include "battleanimation.h"
#include "colorpicker.h"
#include "image.h"
#include "ui_resources.h"

#define DEBUG_MESSAGE "%s: %d\n", __FILE__, __LINE__

class QMediaPlayer;

class Resources : public QDialog, public Ui::Resources
{
	Q_OBJECT
	public:
		Resources(QWidget *parent=0);
		~Resources();
	protected slots:
    void on_tabWidget_currentChanged();
    void on_lstMusic_itemChanged(QListWidgetItem *item);
    void on_sldMusicVolume_sliderMoved(int value);
    void on_lstSFX_itemChanged(QListWidgetItem *item);
    void on_sldSFXVolume_sliderMoved(int value);
	private:
    void refreshMusic();
    void refreshSFX();
    void refreshBackdrops();
    QListWidgetItem *playing;
};

#endif // RESOURCES_H
