/*
 *  Copyright (C) 2024 Nick Egorrov, nicegorov@yandex.ru
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

#ifndef WINDOWACTIONS_H
#define WINDOWACTIONS_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // for QString

class QAction;

#include <abstractactions.h> // for AbstractActions

class GCodeWorkShop;


class WindowActions : public AbstractActions
{
	Q_OBJECT

public:
	explicit WindowActions(GCodeWorkShop* app);
	~WindowActions();

public slots:
	void loadTranslations() override;
	void loadIcons() override;

public:
	QAction* cascade();
	QAction* next();
	QAction* previous();
	QAction* separator();
	QAction* tileHoriz();
	QAction* tileVert();
};

#endif // WINDOWACTIONS_H
