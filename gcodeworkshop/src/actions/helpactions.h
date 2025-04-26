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

#ifndef HELPACTIONS_H
#define HELPACTIONS_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // for QString

class QAction;

#include <abstractactions.h> // for AbstractActions

class GCodeWorkShop;


class HelpActions : public AbstractActions
{
	Q_OBJECT

public:
	explicit HelpActions(GCodeWorkShop* app);
	~HelpActions();

public slots:
	void loadTranslations() override;
	void loadIcons() override;

public:
	QAction* about();
	QAction* aboutQt();
	QAction* createGlobalToolTips();
	QAction* createUserToolTips();
};

#endif // HELPACTIONS_H
