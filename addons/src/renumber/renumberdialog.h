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

#ifndef RENUMBERDIALOG_H
#define RENUMBERDIALOG_H

#include <QDialog>  // for QDialog
#include <QObject>  // for slots, Q_OBJECT
#include <QString>  // for QString
#include <Qt>       // for WindowFlags, Dialog

class QWidget;

#include "ui_renumberdialog.h"  // for Ui::RenumberDialog


/**
 * @brief The RenumberDialog class
 */
class RenumberDialog : public QDialog, private Ui::RenumberDialog
{
    Q_OBJECT

public:
    RenumberDialog(QWidget *parent = 0, Qt::WindowFlags f = Qt::Dialog);
    ~RenumberDialog();

public slots:
    void getState(int &mode, int &startAt, int &from, int &prec, int &inc, int &to,
                  bool &renumEmpty, bool &renumComm, bool &renumMarked);

private slots:
    void okButtonClicked();
    void atEndClicked();
    void divideClicked();
    void renumClicked();
    void allLinesClicked();
    void removeAllClicked();
    void mRenumHeClicked();
};

#endif // RENUMBERDIALOG_H
