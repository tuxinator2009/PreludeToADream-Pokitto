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
#include "messageseditor.h"

MessagesEditor::MessagesEditor(QWidget *parent) : QDialog(parent)
{
  int row = 0;
  setupUi(this);
  ignoreEvents = true;
  tblMessages->clearContents();
  tblMessages->setRowCount(Globals::messages.count());
  for (auto message : Globals::messages.keys())
  {
    QTableWidgetItem *item = new QTableWidgetItem(message);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    tblMessages->setItem(row, 0, item);
    item = new QTableWidgetItem(Globals::messages[message]);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    tblMessages->setItem(row, 1, item);
    ++row;
  }
  tblNullMessage->setColumnWidth(0, 220);
  tblNullMessage->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  tblMessages->setColumnWidth(0, 220);
  tblMessages->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ignoreEvents = false;
}

MessagesEditor::~MessagesEditor()
{
}

void MessagesEditor::on_tblMessages_itemClicked(QTableWidgetItem *item)
{
  if (ignoreEvents)
    return;
  if (item->column() == 0)
    currentMessageName = item->text();
  btnDeleteMessage->setEnabled(true);
}

void MessagesEditor::on_tblMessages_itemChanged(QTableWidgetItem *item)
{
  if (ignoreEvents)
    return;
  if (item->column() == 0)
    Globals::messages.insert(item->text(), Globals::messages.take(currentMessageName));
  else
    Globals::messages[tblMessages->item(item->row(), 0)->text()] = item->text();
}

void MessagesEditor::on_btnAddMessage_clicked()
{
  QTableWidgetItem *item;
  QString name = "";
  bool ok = false;
  while (!ok)
  {
    name = QInputDialog::getText(this, "Name", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok)
      return;
    if (!QRegExp("[a-zA-Z_][a-zA-Z0-9_]*").exactMatch(name))
    {
      QMessageBox::critical(this, "Invalid Name", "Message name must start with a letter or underscore and then contain only letters, numbers, and underscores.");
      ok = false;
    }
  }
  ignoreEvents = true;
  Globals::messages.insert(name, "");
  tblMessages->setRowCount(tblMessages->rowCount() + 1);
  item = new QTableWidgetItem(name);
  item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
  tblMessages->setItem(tblMessages->rowCount() - 1, 0, item);
  item = new QTableWidgetItem("");
  item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
  tblMessages->setItem(tblMessages->rowCount() - 1, 1, item);
  ignoreEvents = false;
}

void MessagesEditor::on_btnDeleteMessage_clicked()
{
  if (QMessageBox::question(this, "Delete Message", QString("Are you sure you want to delete %1?").arg(currentMessageName)) == QMessageBox::No)
    return;
  ignoreEvents = true;
  tblMessages->removeRow(tblMessages->currentRow());
  Globals::messages.take(currentMessageName);
  btnDeleteMessage->setEnabled(false);
  ignoreEvents = false;
}
