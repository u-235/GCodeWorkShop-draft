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
#include <QApplication> // for QApplication, qApp
#include <QIcon>        // for QIcon

#include <gcodeworkshop.h>  // for GCodeWorkShop
#include <knownactions.h>   // for KnownActions

#include "helpactions.h"    // IWYU pragma: no_include "abstractactions.h"


HelpActions::HelpActions(GCodeWorkShop* app) : AbstractActions(app)
{
	connect(makeAction(KnownActions::Help::ABOUT), SIGNAL(triggered()), app, SLOT(about()));
	connect(makeAction(KnownActions::Help::ABOUT_QT), SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(makeAction(KnownActions::Help::CREATE_GLOBAL_TOOL_TIP), SIGNAL(triggered()), app,
	        SLOT(createGlobalToolTipsFile()));
	connect(makeAction(KnownActions::Help::CREATE_USER_TOOL_TIP), SIGNAL(triggered()), app, SLOT(createUserToolTipsFile()));

	HelpActions::loadIcons();
	HelpActions::loadTranslations();
}

HelpActions::~HelpActions()
{
}

void HelpActions::loadTranslations()
{
	setActionText(KnownActions::Help::ABOUT, tr("&About"));
	setActionExToolTip(KnownActions::Help::ABOUT, tr("Show the application's About box"));
	setActionText(KnownActions::Help::ABOUT_QT, tr("About &Qt"));
	setActionExToolTip(KnownActions::Help::ABOUT_QT, tr("Show the Qt library's About box"));
	setActionText(KnownActions::Help::CREATE_GLOBAL_TOOL_TIP, tr("&Create global cnc tooltips"));
	setActionExToolTip(KnownActions::Help::CREATE_GLOBAL_TOOL_TIP, tr("Create default global cnc tooltips file"));
	setActionText(KnownActions::Help::CREATE_USER_TOOL_TIP, tr("&Create user cnc tooltips"));
	setActionExToolTip(KnownActions::Help::CREATE_USER_TOOL_TIP, tr("Create/edit user cnc tooltips file"));
}

void HelpActions::loadIcons()
{
	setActionIcon(KnownActions::Help::ABOUT, QIcon(":/images/about.png"));
	setActionIcon(KnownActions::Help::ABOUT_QT, QIcon(":/images/about_qt.png"));
	setActionIcon(KnownActions::Help::CREATE_GLOBAL_TOOL_TIP, QIcon(":/images/tooltips_glob.png"));
	setActionIcon(KnownActions::Help::CREATE_USER_TOOL_TIP, QIcon(":/images/tooltips_user.png"));
}

QAction* HelpActions::about()
{
	return action(KnownActions::Help::ABOUT);
}

QAction* HelpActions::aboutQt()
{
	return action(KnownActions::Help::ABOUT_QT);
}

QAction* HelpActions::createGlobalToolTips()
{
	return action(KnownActions::Help::CREATE_GLOBAL_TOOL_TIP);
}

QAction* HelpActions::createUserToolTips()
{
	return action(KnownActions::Help::CREATE_USER_TOOL_TIP);
}
