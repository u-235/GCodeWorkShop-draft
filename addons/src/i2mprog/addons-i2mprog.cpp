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

#include <QDialog>  // for QDialog, QDialog::Accepted
#include <QString>  // for QString
#include <QWidget>  // for QWidget

#include "addons-i2mprog.h"
#include "i2mprogdialog.h"  // for I2MProgDialog
#include "i2mprogoptions.h" // for I2MProgOptions
#include "utils-i2mprog.h"  // for i2mprog


int Addons::doI2MProg(QWidget* parent, QSettings* settings,  QString& tx)
{
	int result = 0;
	QString key = "I2MProgDialog";
	I2MProgDialog* dlg;
	dlg = parent->findChild<I2MProgDialog*>(key);

	if (!dlg) {
		dlg = new I2MProgDialog(parent, settings);
		dlg->setObjectName(key);
		dlg->loadSettings(I2MProgOptions());
	}

	if (dlg->exec() == QDialog::Accepted) {
		I2MProgOptions opt = dlg->options();
		result = Utils::i2mprog(tx, opt.axes, opt.toInch, opt.prec);
	}

	dlg->deleteLater();
	return result;
}
