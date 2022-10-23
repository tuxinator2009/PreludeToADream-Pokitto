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

#include <QInputDialog>
#include <QMessageBox>
#include "globals.h"
#include "progresstracker.h"
#include "xmlParser.h"

ProgressTracker::ProgressTracker(QWidget *parent) : QWidget(parent, Qt::Window)
{
  XMLNode progressNode = XMLNode::parseFile(QString("%1/progress.xml").arg(Globals::datadir).toLocal8Bit().data(), "progress");
  setupUi(this);
  treeProgress->addAction(aAddTopLevelItem);
  treeProgress->addAction(aAddChildItem);
  treeProgress->addAction(aDeleteItem);
  connect(treeProgress->model(), SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(itemMoved()));
  if (!progressNode.isEmpty())
  {
    progressBar->setValue(QString(progressNode.getAttribute("progress")).toInt());
    buildProgressTree(treeProgress->invisibleRootItem(), progressNode);
  }
  treeProgress->setColumnWidth(0, 319);
  treeProgress->setColumnWidth(1, 91);
}

ProgressTracker::~ProgressTracker()
{
}

void ProgressTracker::itemMoved()
{
  updateProgressBar(treeProgress->invisibleRootItem());
}

void ProgressTracker::on_treeProgress_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
  if (item->childCount() == 0 && column == 1)
  {
    int progress = item->text(1).toInt();
    bool ok;
    progress = QInputDialog::getInt(this, "Progress", "Progress:", progress, 0, 100, 1, &ok);
    if (ok)
    {
      item->setText(1, QString::number(progress));
      updateProgressBar(treeProgress->invisibleRootItem());
    }
  }
}

void ProgressTracker::on_buttonBox_clicked(QAbstractButton *button)
{
  if (buttonBox->standardButton(button) == QDialogButtonBox::Save)
  {
    XMLNode progressNode = XMLNode::createXMLTopNode("progress");
    progressNode.addAttribute("value", QString::number(progressBar->value()).toLocal8Bit().data());
    for (int i = 0; i < treeProgress->topLevelItemCount(); ++i)
      progressNode.addChild(buildProgressNode(treeProgress->topLevelItem(i)));
    progressNode.writeToFile(QString("%1/progress.xml").arg(Globals::datadir).toLocal8Bit().data());
  }
}

void ProgressTracker::on_aAddTopLevelItem_triggered()
{
  QString name;
  bool ok;
  name = QInputDialog::getText(this, "Name", "Name:", QLineEdit::Normal, "", &ok);
  if (ok)
  {
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QProgressBar *bar = new QProgressBar();
    bar->setRange(0, 100);
    bar->setTextVisible(true);
    bar->setFormat("%p%");
    bar->setValue(0);
    item->setText(0, name);
    item->setText(1, "0");
    item->setText(2, "");
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
    treeProgress->addTopLevelItem(item);
    treeProgress->setItemWidget(item, 1, bar);
    updateProgressBar(treeProgress->invisibleRootItem());
  }
}

void ProgressTracker::on_aAddChildItem_triggered()
{
  QTreeWidgetItem *parent = treeProgress->currentItem();
  QString name;
  bool ok;
  name = QInputDialog::getText(this, "Name", "Name:", QLineEdit::Normal, "", &ok);
  if (ok)
  {
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QProgressBar *bar = new QProgressBar();
    bar->setRange(0, 100);
    bar->setTextVisible(true);
    bar->setFormat("%p%");
    bar->setValue(0);
    item->setText(0, name);
    item->setText(1, "0");
    item->setText(2, "");
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
    parent->addChild(item);
    parent->setExpanded(true);
    treeProgress->setItemWidget(item, 1, bar);
    updateProgressBar(treeProgress->invisibleRootItem());
  }
}

void ProgressTracker::on_aDeleteItem_triggered()
{
  if (QMessageBox::question(this, "Confirm Deletion", QString("Are you sure you want to delete \"%1\" and all it's children?").arg(treeProgress->currentItem()->text(0)), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
  {
    delete treeProgress->currentItem();
    updateProgressBar(treeProgress->invisibleRootItem());
  }
}

int ProgressTracker::updateProgressBar(QTreeWidgetItem *item)
{
  QProgressBar *bar;
  int progress = 0;
  if (item == treeProgress->invisibleRootItem())
    bar = progressBar;
  else
    bar = static_cast<QProgressBar*>(treeProgress->itemWidget(item, 1));
  if (bar == nullptr)
  {
    bar = new QProgressBar();
    bar->setRange(0, 100);
    bar->setTextVisible(true);
    bar->setFormat("%p%");
    bar->setValue(0);
    treeProgress->setItemWidget(item, 1, bar);
  }
  if (item->childCount() > 0)
  {
    for (int i = 0; i < item->childCount(); ++i)
      progress += updateProgressBar(item->child(i));
    progress /= item->childCount();
    item->setText(1, QString("%1").arg(progress));
  }
  else
    progress = item->text(1).toInt();
  bar->setValue(progress);
  return progress;
}

void ProgressTracker::buildProgressTree(QTreeWidgetItem *parent, XMLNode node)
{
  for (int i = 0; i < node.nChildNode(); ++i)
  {
    XMLNode child = node.getChildNode(i);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QProgressBar *bar = new QProgressBar();
    bar->setRange(0, 100);
    bar->setTextVisible(true);
    bar->setFormat("%p%");
    bar->setValue(QString(child.getAttribute("progress")).toInt());
    item->setText(0, child.getAttribute("name"));
    item->setText(1, child.getAttribute("progress"));
    item->setText(2, child.getAttribute("notes"));
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
    parent->addChild(item);
    treeProgress->setItemWidget(item, 1, bar);
    if (child.nChildNode() > 0)
      buildProgressTree(item, child);
  }
}

XMLNode ProgressTracker::buildProgressNode(QTreeWidgetItem *item)
{
  XMLNode node = XMLNode::createXMLTopNode("item");
  node.addAttribute("name", item->text(0).toLocal8Bit().data());
  node.addAttribute("progress", item->text(1).toLocal8Bit().data());
  node.addAttribute("notes", item->text(2).toLocal8Bit().data());
  for (int i = 0; i < item->childCount(); ++i)
    node.addChild(buildProgressNode(item->child(i)));
  return node;
}
