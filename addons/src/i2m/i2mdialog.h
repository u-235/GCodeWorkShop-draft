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

#ifndef I2MDIALOG_H
#define I2MDIALOG_H

#include <QDialog>  // for QDialog
#include <QObject>  // for Q_OBJECT, slots
#include <QString>  // for QString
#include <Qt>       // for WindowFlags, Dialog

class QWidget;

#include "ui_i2mdialog.h"   // for Ui::I2MDialog


/**
 * @brief The I2MDialog class
 */
class I2MDialog : public QDialog, private Ui::I2MDialog
{
    Q_OBJECT

public:
    I2MDialog(QWidget *parent = 0, Qt::WindowFlags f = Qt::Dialog);
    ~I2MDialog();

protected slots:
    void inputChanged();
    void checkBoxToggled();
};

#endif // I2MDIALOG_H
