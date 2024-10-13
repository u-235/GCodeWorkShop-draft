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
#include <QIcon>        // for QIcon

#include <gcodeworkshop.h>  // for GCodeWorkShop
#include <knownactions.h>   // for KnownActions

#include "windowactions.h"  // IWYU pragma: no_include "abstractactions.h"


WindowActions::WindowActions(GCodeWorkShop* app) : AbstractActions(app)
{
	connect(makeAction(KnownActions::Window::CASCADE), SIGNAL(triggered()), app, SLOT(cascadeSubWindows()));
	connect(makeAction(KnownActions::Window::NEXT), SIGNAL(triggered()), app, SLOT(activateNextSubWindow()));
	connect(makeAction(KnownActions::Window::PREVIOUS), SIGNAL(triggered()), app, SLOT(activatePreviousSubWindow()));
	makeAction(KnownActions::Window::SEPARATOR)->setSeparator(true);
	connect(makeAction(KnownActions::Window::TILE_HORIZ), SIGNAL(triggered()), app, SLOT(tileSubWindowsHorizontally()));
	connect(makeAction(KnownActions::Window::TILE_VERT), SIGNAL(triggered()), app, SLOT(tileSubWindowsVertycally()));

	WindowActions::loadIcons();
	WindowActions::loadTranslations();
}

WindowActions::~WindowActions()
{
}

void WindowActions::loadTranslations()
{
	setActionText(KnownActions::Window::CASCADE, tr("&Cascade"));
	setActionExToolTip(KnownActions::Window::CASCADE, tr("Cascade the windows"));
	setActionText(KnownActions::Window::NEXT, tr("Ne&xt"));
	setActionExToolTip(KnownActions::Window::NEXT, tr("Move the focus to the next window"));
	setActionText(KnownActions::Window::PREVIOUS, tr("Pre&vious"));
	setActionExToolTip(KnownActions::Window::PREVIOUS, tr("Move the focus to the previous window"));
	setActionText(KnownActions::Window::TILE_HORIZ, tr("Tile &horyzontally"));
	setActionExToolTip(KnownActions::Window::TILE_HORIZ, tr("Tile the windows horyzontallyTile the windows horyzontally"));
	setActionText(KnownActions::Window::TILE_VERT, tr("Tile &vertycally"));
	setActionExToolTip(KnownActions::Window::TILE_VERT, tr("Tile the windows vertycally"));
}

void WindowActions::loadIcons()
{
	setActionIcon(KnownActions::Window::CASCADE, QIcon(":/images/cascade.png"));
	setActionIcon(KnownActions::Window::NEXT, QIcon(":/images/go-next.png"));
	setActionIcon(KnownActions::Window::PREVIOUS, QIcon(":/images/go-previous.png"));
	setActionIcon(KnownActions::Window::TILE_HORIZ, QIcon(":/images/tile_h.png"));
	setActionIcon(KnownActions::Window::TILE_VERT, QIcon(":/images/tile_v.png"));
}

QAction* WindowActions::cascade()
{
	return action(KnownActions::Window::CASCADE);
}

QAction* WindowActions::next()
{
	return action(KnownActions::Window::NEXT);
}

QAction* WindowActions::previous()
{
	return action(KnownActions::Window::PREVIOUS);
}

QAction* WindowActions::separator()
{
	return action(KnownActions::Window::SEPARATOR);
}

QAction* WindowActions::tileHoriz()
{
	return action(KnownActions::Window::TILE_HORIZ);
}

QAction* WindowActions::tileVert()

{
	return action(KnownActions::Window::TILE_VERT);
}
