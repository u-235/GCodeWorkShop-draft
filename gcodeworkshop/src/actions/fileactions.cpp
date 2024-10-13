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

#include <QAction>      // for QAction
#include <QApplication> // for qApp
#include <QIcon>        // for QIcon

#include <gcodeworkshop.h>  // for GCodeWorkShop
#include <knownactions.h>   // for KnownActions

#include "fileactions.h" // IWYU pragma: no_include "abstractactions.h"


FileActions::FileActions(GCodeWorkShop* app) : AbstractActions(app)
{
	connect(makeAction(KnownActions::File::CLOSE), SIGNAL(triggered()), app, SLOT(closeCurrentWindow()));
	connect(makeAction(KnownActions::File::CLOSE_ALL), SIGNAL(triggered()), app, SLOT(closeAllMdiWindows()));
	connect(makeAction(KnownActions::File::EXIT), SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
	connect(makeAction(KnownActions::File::FIND_IN_FILES), SIGNAL(triggered()), app, SLOT(findInFl()));
	connect(makeAction(KnownActions::File::NEW), SIGNAL(triggered()), app, SLOT(newFileFromTemplate()));
	connect(makeAction(KnownActions::File::OPEN), SIGNAL(triggered()), app, SLOT(open()));
	connect(makeAction(KnownActions::File::OPEN_EXAMPLE), SIGNAL(triggered()), app, SLOT(openExample()));
	connect(makeAction(KnownActions::File::PRINT), SIGNAL(triggered()), app, SLOT(printFile()));
	connect(makeAction(KnownActions::File::PRINT_PREVIEW), SIGNAL(triggered()), app, SLOT(filePrintPreview()));
	connect(makeAction(KnownActions::File::SAVE), SIGNAL(triggered()), app, SLOT(save()));
	connect(makeAction(KnownActions::File::SAVE_ALL), SIGNAL(triggered()), app, SLOT(saveAll()));
	connect(makeAction(KnownActions::File::SAVE_AS), SIGNAL(triggered()), app, SLOT(saveAs()));
	connect(makeAction(KnownActions::File::SESSION_MANAGER), SIGNAL(triggered()), app, SLOT(showSessionDialog()));

	FileActions::loadIcons();
	FileActions::loadTranslations();
}

FileActions::~FileActions()
{
}

void FileActions::loadTranslations()
{
	setActionText(KnownActions::File::CLOSE, tr("Cl&ose \"%1\"").arg(""));
	setActionExToolTip(KnownActions::File::CLOSE, tr("Close the active window"));
	setActionText(KnownActions::File::CLOSE_ALL, tr("Close &All"));
	setActionExToolTip(KnownActions::File::CLOSE_ALL, tr("Close all the windows"));
	setActionText(KnownActions::File::EXIT, tr("E&xit"));
	setActionExToolTip(KnownActions::File::EXIT, tr("Exit the application"));
	setActionText(KnownActions::File::FIND_IN_FILES, tr("&Find files..."));
	setActionExToolTip(KnownActions::File::FIND_IN_FILES, tr("Find files"));
	findFiles()->setCheckable(true);
	setActionText(KnownActions::File::NEW, tr("&New"));
	setActionExToolTip(KnownActions::File::NEW, tr("Create a new file"));
	setActionText(KnownActions::File::OPEN, tr("&Open..."));
	setActionExToolTip(KnownActions::File::OPEN, tr("Open an existing file"));
	setActionText(KnownActions::File::OPEN_EXAMPLE, tr("&Open example..."));
	setActionExToolTip(KnownActions::File::OPEN_EXAMPLE, tr("Open an example file"));
	setActionText(KnownActions::File::PRINT, tr("&Print"));
	setActionExToolTip(KnownActions::File::PRINT, tr("Print file"));
	setActionText(KnownActions::File::PRINT_PREVIEW, tr("Pr&int preview"));
	setActionExToolTip(KnownActions::File::PRINT_PREVIEW, tr("Preview printing"));
	setActionText(KnownActions::File::SAVE, tr("&Save"));
	setActionExToolTip(KnownActions::File::SAVE, tr("Save the document to disk"));
	setActionText(KnownActions::File::SAVE_ALL, tr("Save A&ll"));
	setActionExToolTip(KnownActions::File::SAVE_ALL, tr("Save all modified documents to disk"));
	setActionText(KnownActions::File::SAVE_AS, tr("Save &As..."));
	setActionExToolTip(KnownActions::File::SAVE_AS, tr("Save the document under a new name"));
	setActionText(KnownActions::File::SESSION_MANAGER, tr("Session manager..."));
	setActionExToolTip(KnownActions::File::SESSION_MANAGER, tr("Sessions manager"));
}

void FileActions::loadIcons()
{
	setActionIcon(KnownActions::File::CLOSE, QIcon(":/images/fileclose.png"));
	setActionIcon(KnownActions::File::CLOSE_ALL, QIcon(":/images/window-close.png"));
	setActionIcon(KnownActions::File::EXIT, QIcon(":/images/exit.png"));
	setActionIcon(KnownActions::File::FIND_IN_FILES, QIcon(":/images/filefind.png"));
	setActionIcon(KnownActions::File::NEW, QIcon(":/images/filenew.png"));
	setActionIcon(KnownActions::File::OPEN, QIcon(":/images/fileopen.png"));
	setActionIcon(KnownActions::File::OPEN_EXAMPLE, QIcon(":/images/fileopen.png"));
	setActionIcon(KnownActions::File::PRINT, QIcon(":/images/document-print.png"));
	setActionIcon(KnownActions::File::PRINT_PREVIEW, QIcon(":/images/document-print-preview.png"));
	setActionIcon(KnownActions::File::SAVE, QIcon(":/images/filesave.png"));
	setActionIcon(KnownActions::File::SAVE_ALL, QIcon(":/images/filesaveall.png"));
	setActionIcon(KnownActions::File::SAVE_AS, QIcon(":/images/filesaveas.png"));
}

QAction* FileActions::close()
{
	return action(KnownActions::File::CLOSE);
}

QAction* FileActions::closeAll()
{
	return action(KnownActions::File::CLOSE_ALL);
}

QAction* FileActions::exit()
{
	return action(KnownActions::File::EXIT);
}

QAction* FileActions::findFiles()
{
	return action(KnownActions::File::FIND_IN_FILES);
}

QAction* FileActions::makeNew()
{
	return action(KnownActions::File::NEW);
}

QAction* FileActions::open()
{
	return action(KnownActions::File::OPEN);
}

QAction* FileActions::openExample()
{
	return action(KnownActions::File::OPEN_EXAMPLE);
}

QAction* FileActions::print()
{
	return action(KnownActions::File::PRINT);
}

QAction* FileActions::printPreview()
{
	return action(KnownActions::File::PRINT_PREVIEW);
}

QAction* FileActions::save()
{
	return action(KnownActions::File::SAVE);
}

QAction* FileActions::saveAll()
{
	return action(KnownActions::File::SAVE_ALL);
}

QAction* FileActions::saveAs()
{
	return action(KnownActions::File::SAVE_AS);
}

QAction* FileActions::sessionManager()
{
	return action(KnownActions::File::SESSION_MANAGER);
}
