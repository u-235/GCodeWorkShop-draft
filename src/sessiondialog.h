/***************************************************************************
 *   Copyright (C) 2006-2018 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of EdytorNC.                                        *
 *                                                                         *
 *   EdytorNC is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SESSIONDIALOG_H
#define SESSIONDIALOG_H

#include <QDialog>
#include <QObject>  // Q_OBJECT
#include <QString>
#include <Qt>       // Qt::WindowFlags
#include <QtGlobal> // QT_VERSION QT_VERSION_CHECK
#include <QWidget>

#include "ui_sessiondialog.h"

class QListWidgetItem;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    class QStringList;
#else
    using QStringList = QList<QString>;
#endif


class SessionDialog: public QDialog, private Ui::SessionDialog
{
    Q_OBJECT

public:
    SessionDialog(QWidget *parent = 0, Qt::WindowFlags f = Qt::Dialog);
    ~SessionDialog();

    void setSelectedSession(QString name);
    QString selectedSession();
    QStringList sessionList();
    void setSessionList(QStringList list);

private slots:
    void newButtonClicked();
    void renameButtonClicked();
    void deleteButtonClicked();
    void cloneButtonClicked();
    void switchButtonClicked();
    void sessionListItemitemActivated(QListWidgetItem *item);

private:
    void clearChecked();
    void copySession(QString oldName, QString newName, bool deleteOld = false);
    void deleteSession(QString name);
};

#endif // SESSIONDIALOG_H
