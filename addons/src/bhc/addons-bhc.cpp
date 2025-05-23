/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
 *  Copyright (C) 2023 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This file is part of GCodeWorkShop.
 *
 *  GCodeWorkShop is free software: you can redistribute it and/or modify
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

#include <QString>  // for QString
#include <QWidget>  // for QWidget

#include "addons-bhc.h"
#include "bhcdialog.h"  // for BHCDialog


void Addons::doBhc(QWidget* parent, QSettings* settings)
{
	QString key = "BHCDialog";
	BHCDialog* dlg;
	dlg = parent->findChild<BHCDialog*>(key);

	if (!dlg) {
		dlg = new BHCDialog(parent, settings);
		dlg->setObjectName(key);
		dlg->loadSettings();
	}

	dlg->show();
}
