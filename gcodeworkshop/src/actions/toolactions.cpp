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

#include "toolactions.h"    // IWYU pragma: no_include "abstractactions.h"


ToolActions::ToolActions(GCodeWorkShop* app) : AbstractActions(app)
{
	connect(makeAction(KnownActions::Tool::CALC), SIGNAL(triggered()), app, SLOT(doCalc()));
	connect(makeAction(KnownActions::Tool::DIFF), SIGNAL(triggered()), app, SLOT(doDiff()));
	connect(makeAction(KnownActions::Tool::DIFF_EDITOR), SIGNAL(triggered()), app, SLOT(diffEditorFile()));
	connect(makeAction(KnownActions::Tool::DIFF_LEFT), SIGNAL(triggered()), app, SLOT(doDiffL()));
	connect(makeAction(KnownActions::Tool::DIFF_RIGHT), SIGNAL(triggered()), app, SLOT(doDiffR()));
	connect(makeAction(KnownActions::Tool::INLINE_CALC), SIGNAL(triggered()), app, SLOT(doShowInLineCalc()));
	connect(makeAction(KnownActions::Tool::SHOW_SERIAL_TOOLBAR), SIGNAL(triggered()), app, SLOT(createSerialToolBar()));

	diff()->setCheckable(true);
	showSerialToolBar()->setCheckable(true);

	ToolActions::loadIcons();
	ToolActions::loadTranslations();
}

ToolActions::~ToolActions()
{
}

void ToolActions::loadTranslations()
{
	setActionText(KnownActions::Tool::CALC, tr("Calculator"));
	setActionExToolTip(KnownActions::Tool::CALC, tr("Run calculator"));
	setActionText(KnownActions::Tool::DIFF, tr("On/off diff window"));
	setActionExToolTip(KnownActions::Tool::DIFF, tr("Show diff window"));
	setActionText(KnownActions::Tool::DIFF_EDITOR, tr("Show unsaved changes"));
	setActionExToolTip(KnownActions::Tool::DIFF_EDITOR, tr("Show diff of currently edited file and file on disk"));
	setActionText(KnownActions::Tool::DIFF_LEFT, tr("Show diff - open current file in left diff window"));
	setActionExToolTip(KnownActions::Tool::DIFF_LEFT, tr("Show diff - open current file in left diff window"));
	setActionText(KnownActions::Tool::DIFF_RIGHT, tr("Show diff - open current file in right diff window"));
	setActionExToolTip(KnownActions::Tool::DIFF_RIGHT, tr("Show diff - open current file in right diff window"));
	setActionText(KnownActions::Tool::INLINE_CALC, tr("Inline calculator"));
	setActionExToolTip(KnownActions::Tool::INLINE_CALC, tr("Inline calculator"));
	setActionText(KnownActions::Tool::SHOW_SERIAL_TOOLBAR, tr("Serial port send/receive"));
	setActionExToolTip(KnownActions::Tool::SHOW_SERIAL_TOOLBAR, tr("Serial port send/receive"));
}

void ToolActions::loadIcons()
{
	setActionIcon(KnownActions::Tool::CALC, QIcon(":/images/calc.png"));
	setActionIcon(KnownActions::Tool::DIFF, QIcon(":/images/diff.png"));
	setActionIcon(KnownActions::Tool::DIFF_EDITOR, QIcon(":/images/diff_editor.png"));
	setActionIcon(KnownActions::Tool::DIFF_LEFT, QIcon(":/images/diffl.png"));
	setActionIcon(KnownActions::Tool::DIFF_RIGHT, QIcon(":/images/diffr.png"));
	setActionIcon(KnownActions::Tool::INLINE_CALC, QIcon(":/images/inlinecalc.png"));
	setActionIcon(KnownActions::Tool::SHOW_SERIAL_TOOLBAR, QIcon(":/images/serial.png"));
}

QAction* ToolActions::calc()
{
	return action(KnownActions::Tool::CALC);
}

QAction* ToolActions::diff()
{
	return action(KnownActions::Tool::DIFF);
}

QAction* ToolActions::diffEditor()
{
	return action(KnownActions::Tool::DIFF_EDITOR);
}

QAction* ToolActions::diffLeft()
{
	return action(KnownActions::Tool::DIFF_LEFT);
}

QAction* ToolActions::diffRight()
{
	return action(KnownActions::Tool::DIFF_RIGHT);
}

QAction* ToolActions::inLineCalc()
{
	return action(KnownActions::Tool::INLINE_CALC);
}

QAction* ToolActions::showSerialToolBar()
{
	return action(KnownActions::Tool::SHOW_SERIAL_TOOLBAR);
}
