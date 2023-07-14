/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
 *  Copyright (C) 2023 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This file is part of EdytorNC.
 *
 *  EdytorNC is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QAbstractItemView>    // for QAbstractItemView, QAbstractItemView::NoSelection
#include <QAction>              // for QAction
#include <QApplication>         // for QApplication
#include <QColor>               // for QColor
#include <QCursor>              // for QCursor
#include <QIcon>                // for QIcon
#include <QKeySequence>         // for QKeySequence, QKeySequence::Delete
#include <QList>                // for QList
#include <QMenu>                // for QMenu
#include <QPlainTextEdit>       // for QPlainTextEdit
#include <QPushButton>          // for QPushButton
#include <QRegularExpression>   // for QRegularExpression
#include <QString>              // for QString, operator==
#include <QTableWidget>         // for QTableWidget
#include <QTableWidgetItem>     // for QTableWidgetItem
#include <QTextCharFormat>      // for QTextCharFormat
#include <QTextCursor>          // for QTextCursor, QTextCursor::StartOfBlock
#include <QTextDocument>        // for QTextDocument
#include <QTextEdit>            // for QTextEdit, QTextEdit::ExtraSelection
#include <Qt>                   // for operator|, AlignLeft, AlignVCenter, Checked, ItemIsEnabled, ItemIsUserCheckable
#include <QtGlobal>             // for QFlags, Q_UNUSED

#include "cleanupdialog.h"


CleanUpDialog::CleanUpDialog(QWidget *parent) :
    QDialog(parent),
    Ui::CleanUpDialog()
{
    setupUi(this);

    QAction *deleteRowAct = new QAction(QIcon(":/images/removeemptylines.png"), tr("Delete &row"),
                                        this);
    deleteRowAct->setShortcut(QKeySequence::Delete);
    deleteRowAct->setStatusTip(tr("Delete current row"));
    connect(deleteRowAct, SIGNAL(triggered()), this, SLOT(removeRow()));
    tableWidget->addAction(deleteRowAct);

    contextMenu = new QMenu(this);
    contextMenu->addAction(deleteRowAct);

    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    newRow();

    connect(tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(highlightText(int, int)));
    connect(tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(cellChangedSlot(int, int)));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(contextMenuReq(const QPoint &)));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
}

CleanUpDialog::~CleanUpDialog()
{
}

void CleanUpDialog::setText(QString text)
{
    textEdit->setPlainText(text);
}

void CleanUpDialog::cellChangedSlot(int row, int col)
{
    if (col == 1) {
        highlightText(row, col);
    }
}

void CleanUpDialog::newRow()
{
    int row = tableWidget->rowCount();

    if (row > 0) {
        row--;

        if (tableWidget->item(row, 0) != nullptr)
            if (tableWidget->item(row, 0)->text().isNull()
                    || tableWidget->item(row, 0)->text().isEmpty()) {
                return;
            }

        if (tableWidget->item(row, 1) != nullptr)
            if (tableWidget->item(row, 1)->text().isNull()
                    || tableWidget->item(row, 1)->text().isEmpty()) {
                return;
            }
    }

    row++;

    QTableWidgetItem *valueItem = new QTableWidgetItem("");
    valueItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *commentItem = new QTableWidgetItem("");
    commentItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *enableItem = new QTableWidgetItem("");
    enableItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    enableItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    enableItem->setCheckState(Qt::Unchecked);

    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, commentItem);
    tableWidget->setItem(row, 1, valueItem);
    tableWidget->setItem(row, 2, enableItem);
}

void CleanUpDialog::highlightText(int row, int col)
{
    Q_UNUSED(col);
    QTableWidgetItem *item = tableWidget->item(row, 1);

    if (item != nullptr) {
        if (!item->text().isEmpty()) {
            if (item->text() == "$") { //causes endless loop
                return;
            }

            highlightFindText(QRegularExpression(item->text()));
        }
    }

    newRow();
}

void CleanUpDialog::highlightFindText(QRegularExpression regex)
{
    QList<QTextEdit::ExtraSelection> findTextExtraSelections;
    QTextEdit::ExtraSelection selection;
    QTextCursor firstFoundCursor;

    QColor lineColor = QColor(Qt::red).lighter(155);
    selection.format.setBackground(lineColor);

    QTextDocument *doc = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();

    cursor.setPosition(0);

    firstFoundCursor = cursor;

    do {
        cursor = doc->find(regex, cursor);

        if (!cursor.isNull()) {
            if (cursor.selectedText().length() == 0) { //cursor not null but nothing found
                break;
            }

            if (firstFoundCursor.position() == 0) {
                firstFoundCursor = cursor;
            }

            selection.cursor = cursor;
            findTextExtraSelections.append(selection);
        }

        QApplication::processEvents();

    } while (!cursor.isNull() && !cursor.atEnd());

    firstFoundCursor.movePosition(QTextCursor::StartOfBlock);
    textEdit->setExtraSelections(findTextExtraSelections);
    textEdit->setTextCursor(firstFoundCursor);
    textEdit->centerCursor();
}

void CleanUpDialog::contextMenuReq(const QPoint &pos)
{
    Q_UNUSED(pos);
    contextMenu->popup(QCursor::pos());
}

void CleanUpDialog::removeRow()
{
    int row = tableWidget->currentRow();
    tableWidget->removeRow(row);
}
