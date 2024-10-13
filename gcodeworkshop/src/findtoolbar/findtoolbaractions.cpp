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
 *  but WITHOUT ANY WARRANTY(new QAction(this)), without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QIcon>           // for QIcon

#include <abstractactions.h>    // for AbstractActions
#include <knownactions.h>       // for KnownActions

#include "findtoolbaractions.h"


FindToolBarActions::FindToolBarActions(QObject* parent) : AbstractActions(parent)
{
	makeAction(KnownActions::FindToolBar::CLOSE);
	makeAction(KnownActions::FindToolBar::FIND_NEXT);
	makeAction(KnownActions::FindToolBar::FIND_PREVIOUS);
	makeAction(KnownActions::FindToolBar::REPLACE_ALL);
	makeAction(KnownActions::FindToolBar::REPLACE_NEXT);
	makeAction(KnownActions::FindToolBar::REPLACE_PREVIOUS);

	FindToolBarActions::loadIcons();
	FindToolBarActions::loadTranslations();
}

FindToolBarActions::~FindToolBarActions()
{
}

void FindToolBarActions::loadTranslations()
{
	setActionText(KnownActions::FindToolBar::CLOSE, tr("Close find toolbar"));
	setActionExToolTip(KnownActions::FindToolBar::CLOSE, tr("Close find toolbar"));
	setActionText(KnownActions::FindToolBar::FIND_NEXT, tr("Find next"));
	setActionExToolTip(KnownActions::FindToolBar::FIND_NEXT, tr("Find next"));
	setActionText(KnownActions::FindToolBar::FIND_PREVIOUS, tr("Find previous"));
	setActionExToolTip(KnownActions::FindToolBar::FIND_PREVIOUS, tr("Find previous"));
	setActionText(KnownActions::FindToolBar::REPLACE_ALL, tr("Replace all"));
	setActionExToolTip(KnownActions::FindToolBar::REPLACE_ALL, tr("Replace all"));
	setActionText(KnownActions::FindToolBar::REPLACE_NEXT, tr("Replace && find next"));
	setActionExToolTip(KnownActions::FindToolBar::REPLACE_NEXT, tr("Replace && find next"));
	setActionText(KnownActions::FindToolBar::REPLACE_PREVIOUS, tr("Replace && find previous"));
	setActionExToolTip(KnownActions::FindToolBar::REPLACE_PREVIOUS, tr("Replace && find previous"));
}

void FindToolBarActions::loadIcons()
{
	setActionIcon(KnownActions::FindToolBar::CLOSE, QIcon(":/images/close_small.png"));
	setActionIcon(KnownActions::FindToolBar::FIND_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(KnownActions::FindToolBar::FIND_PREVIOUS, QIcon(":/images/arrow-left.png"));
	setActionIcon(KnownActions::FindToolBar::REPLACE_ALL, QIcon(":/images/arrow-right-double.png"));
	setActionIcon(KnownActions::FindToolBar::REPLACE_NEXT, QIcon(":/images/arrow-right.png"));
	setActionIcon(KnownActions::FindToolBar::REPLACE_PREVIOUS, QIcon(":/images/arrow-left.png"));
}

QAction* FindToolBarActions::findClose()
{
	return action(KnownActions::FindToolBar::CLOSE);
}

QAction* FindToolBarActions::findNext()
{
	return action(KnownActions::FindToolBar::FIND_NEXT);
}

QAction* FindToolBarActions::findPrevious()
{
	return action(KnownActions::FindToolBar::FIND_PREVIOUS);
}

QAction* FindToolBarActions::replaceAll()
{
	return action(KnownActions::FindToolBar::REPLACE_ALL);
}

QAction* FindToolBarActions::replaceNext()
{
	return action(KnownActions::FindToolBar::REPLACE_NEXT);
}

QAction* FindToolBarActions::replacePrevious()
{
	return action(KnownActions::FindToolBar::REPLACE_PREVIOUS);
}
