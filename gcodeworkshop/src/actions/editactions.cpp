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

#include <QAction>  // for QAction
#include <QIcon>    // for QIcon

#include <gcodeworkshop.h>  // for GCodeWorkShop
#include <knownactions.h>   // for KnownActions

#include "editactions.h"    // IWYU pragma: no_include "abstractactions.h"


EditActions::EditActions(GCodeWorkShop* app) : AbstractActions(app)
{
	connect(makeAction(KnownActions::Edit::CONFIG), SIGNAL(triggered()), app, SLOT(config()));
	connect(makeAction(KnownActions::Edit::COPY), SIGNAL(triggered()), app, SLOT(copy()));
	connect(makeAction(KnownActions::Edit::CUT), SIGNAL(triggered()), app, SLOT(cut()));
	connect(makeAction(KnownActions::Edit::DELETE), SIGNAL(triggered()), app, SLOT(deleteText()));
	connect(makeAction(KnownActions::Edit::FIND), SIGNAL(triggered()), app, SLOT(showFindToolBar()));
	connect(makeAction(KnownActions::Edit::PASTE), SIGNAL(triggered()), app, SLOT(paste()));
	connect(makeAction(KnownActions::Edit::READ_ONLY), SIGNAL(triggered()), app, SLOT(readOnly()));
	connect(makeAction(KnownActions::Edit::REDO), SIGNAL(triggered()), app, SLOT(redo()));
	connect(makeAction(KnownActions::Edit::REPLACE), SIGNAL(triggered()), app, SLOT(showReplaceToolBar()));
	connect(makeAction(KnownActions::Edit::SELECT_ALL), SIGNAL(triggered()), app, SLOT(selAll()));
	connect(makeAction(KnownActions::Edit::UNDO), SIGNAL(triggered()), app, SLOT(undo()));

	readOnly()->setCheckable(true);
	redo()->setEnabled(false);
	undo()->setEnabled(false);

	EditActions::loadIcons();
	EditActions::loadTranslations();
}

EditActions::~EditActions()
{
}

void EditActions::loadTranslations()
{
	setActionText(KnownActions::Edit::CONFIG, tr("Configuration"));
	setActionExToolTip(KnownActions::Edit::CONFIG, tr("Open configuration dialog"));
	setActionText(KnownActions::Edit::COPY, tr("&Copy"));
	setActionExToolTip(KnownActions::Edit::COPY, tr("Copy the current selection's contents to the clipboard"));
	setActionText(KnownActions::Edit::CUT, tr("Cu&t"));
	setActionExToolTip(KnownActions::Edit::CUT, tr("Cut the current selection's contents to the clipboard"));
	setActionText(KnownActions::Edit::DELETE, tr("&Delete"));
	setActionExToolTip(KnownActions::Edit::DELETE, tr("Removes selected text"));
	setActionText(KnownActions::Edit::FIND, tr("&Find"));
	setActionExToolTip(KnownActions::Edit::FIND, tr("Find text"));
	setActionText(KnownActions::Edit::PASTE, tr("&Paste"));
	setActionExToolTip(KnownActions::Edit::PASTE, tr("Paste the clipboard's contents into the current selection"));
	setActionText(KnownActions::Edit::READ_ONLY, tr("Read &only"));
	setActionExToolTip(KnownActions::Edit::READ_ONLY, tr("Makes text read only"));
	setActionText(KnownActions::Edit::REDO, tr("&Redo"));
	setActionExToolTip(KnownActions::Edit::REDO, tr("Redo last operation"));
	setActionText(KnownActions::Edit::REPLACE, tr("&Replace"));
	setActionExToolTip(KnownActions::Edit::REPLACE, tr("Find and replace text"));
	setActionText(KnownActions::Edit::SELECT_ALL, tr("&Select all"));
	setActionExToolTip(KnownActions::Edit::SELECT_ALL, tr("Select all text"));
	setActionText(KnownActions::Edit::UNDO, tr("&Undo"));
	setActionExToolTip(KnownActions::Edit::UNDO, tr("Undo last operation"));
}

void EditActions::loadIcons()
{
	setActionIcon(KnownActions::Edit::CONFIG, QIcon(":/images/configure.png"));
	setActionIcon(KnownActions::Edit::COPY, QIcon(":/images/editcopy.png"));
	setActionIcon(KnownActions::Edit::CUT, QIcon(":/images/editcut.png"));
	setActionIcon(KnownActions::Edit::DELETE, QIcon(":/images/editdelete.png"));
	setActionIcon(KnownActions::Edit::FIND, QIcon(":/images/find.png"));
	setActionIcon(KnownActions::Edit::PASTE, QIcon(":/images/editpaste.png"));
	setActionIcon(KnownActions::Edit::READ_ONLY, QIcon(":/images/unlock.png"));
	setActionIcon(KnownActions::Edit::REDO, QIcon(":/images/redo.png"));
	setActionIcon(KnownActions::Edit::REPLACE, QIcon(":/images/replace.png"));
	setActionIcon(KnownActions::Edit::SELECT_ALL, QIcon(":/images/edit-select-all.png"));
	setActionIcon(KnownActions::Edit::UNDO, QIcon(":/images/undo.png"));
}

QAction* EditActions::config()
{
	return action(KnownActions::Edit::CONFIG);
}

QAction* EditActions::copy()
{
	return action(KnownActions::Edit::COPY);
}

QAction* EditActions::cut()
{
	return action(KnownActions::Edit::CUT);
}

QAction* EditActions::del()
{
	return action(KnownActions::Edit::DELETE);
}

QAction* EditActions::find()
{
	return action(KnownActions::Edit::FIND);
}

QAction* EditActions::paste()
{
	return action(KnownActions::Edit::PASTE);
}

QAction* EditActions::readOnly()
{
	return action(KnownActions::Edit::READ_ONLY);
}

QAction* EditActions::redo()
{
	return action(KnownActions::Edit::REDO);
}

QAction* EditActions::replace()
{
	return action(KnownActions::Edit::REPLACE);
}

QAction* EditActions::selectAll()
{
	return action(KnownActions::Edit::SELECT_ALL);
}

QAction* EditActions::undo()
{
	return action(KnownActions::Edit::UNDO);
}
