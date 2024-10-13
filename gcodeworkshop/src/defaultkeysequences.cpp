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

#include <QCoreApplication>
#include <QKeySequence>  // for QKeySequence
#include <QString>       // for QString, operator<

#include "defaultkeysequences.h"
#include "knownactions.h"

#define tr(s) QCoreApplication::translate("QShortcut", s)


QMap<QString, QKeySequence> DefaultKeySequences::sequence()
{
	return QMap<QString, QKeySequence> {
		{KnownActions::Addons::BHC, QKeySequence(tr("F8"))},
		{KnownActions::Addons::BLOCK_SKIP_DECREMENT, QKeySequence(tr("Ctrl+1"))},
		{KnownActions::Addons::BLOCK_SKIP_INCREMENT, QKeySequence(tr("Ctrl+2"))},
		{KnownActions::Addons::BLOCK_SKIP_REMOVE, QKeySequence(tr("Ctrl+3"))},
		{KnownActions::Addons::CHAMFER, QKeySequence()},
		{KnownActions::Addons::CLEANUP, QKeySequence()},
		{KnownActions::Addons::COMMENT_PARA, QKeySequence(tr("Ctrl+9"))},
		{KnownActions::Addons::COMMENT_SEMI, QKeySequence(tr("Ctrl+;"))},
		{KnownActions::Addons::COMPILE_MACRO, QKeySequence()},
		{KnownActions::Addons::DOT, QKeySequence(tr("F6"))},
		{KnownActions::Addons::EMPTY_LINE_INSERT, QKeySequence()},
		{KnownActions::Addons::EMPTY_LINE_REMOVE, QKeySequence()},
		{KnownActions::Addons::FEEDS, QKeySequence(tr("F9"))},
		{KnownActions::Addons::I2M, QKeySequence()},
		{KnownActions::Addons::I2MPROG, QKeySequence()},
		{KnownActions::Addons::RENUMBER, QKeySequence(tr("F7"))},
		{KnownActions::Addons::SPACES_INSERT, QKeySequence(tr("F4"))},
		{KnownActions::Addons::SPACES_REMOVE, QKeySequence(tr("F5"))},
		{KnownActions::Addons::SWAP_AXES, QKeySequence()},
		{KnownActions::Addons::TRANGLE, QKeySequence()},

		{KnownActions::Edit::CONFIG, QKeySequence(QKeySequence::Preferences)},
		{KnownActions::Edit::COPY, QKeySequence(QKeySequence::Copy)},
		{KnownActions::Edit::CUT, QKeySequence(QKeySequence::Cut)},
		{KnownActions::Edit::DELETE, QKeySequence(QKeySequence::Delete)},
		{KnownActions::Edit::FIND, QKeySequence(QKeySequence::Find)},
		{KnownActions::Edit::PASTE, QKeySequence(QKeySequence::Paste)},
		{KnownActions::Edit::READ_ONLY, QKeySequence(tr("F12"))},
		{KnownActions::Edit::REDO, QKeySequence(QKeySequence::Redo)},
		{KnownActions::Edit::REPLACE, QKeySequence(tr("Ctrl+R"))},
		{KnownActions::Edit::SELECT_ALL, QKeySequence(QKeySequence::SelectAll)},
		{KnownActions::Edit::UNDO, QKeySequence(QKeySequence::Undo)},

		{KnownActions::File::CLOSE, QKeySequence()},
		{KnownActions::File::CLOSE_ALL, QKeySequence()}, // tr("Ctrl+")
		{KnownActions::File::EXIT, QKeySequence(tr("Ctrl+Q"))},
		{KnownActions::File::FIND_IN_FILES, QKeySequence()},
		{KnownActions::File::NEW, QKeySequence(QKeySequence::New)},
		{KnownActions::File::OPEN, QKeySequence(QKeySequence::Open)},
		{KnownActions::File::OPEN_EXAMPLE, QKeySequence()},
		{KnownActions::File::PRINT, QKeySequence(QKeySequence::Print)},
		{KnownActions::File::PRINT_PREVIEW, QKeySequence()},
		{KnownActions::File::SAVE, QKeySequence(QKeySequence::Save)},
		{KnownActions::File::SAVE_ALL, tr("Ctrl+Shift+S")},
		{KnownActions::File::SAVE_AS, QKeySequence(QKeySequence::SaveAs)}, // TODO replace with Ctrl+Alt+S ?
		{KnownActions::File::SESSION_MANAGER, QKeySequence()},

		{KnownActions::FindToolBar::FIND_NEXT, QKeySequence(QKeySequence::FindNext)},
		{KnownActions::FindToolBar::FIND_PREVIOUS, QKeySequence(QKeySequence::FindPrevious)},
		{KnownActions::FindToolBar::REPLACE_NEXT, QKeySequence()},
		{KnownActions::FindToolBar::REPLACE_PREVIOUS, QKeySequence()},
		{KnownActions::FindToolBar::REPLACE_ALL, QKeySequence()},
		{KnownActions::FindToolBar::CLOSE, QKeySequence()},

		{KnownActions::Help::ABOUT, QKeySequence()},
		{KnownActions::Help::ABOUT_QT, QKeySequence()},
		{KnownActions::Help::CREATE_GLOBAL_TOOL_TIP, QKeySequence()},
		{KnownActions::Help::CREATE_USER_TOOL_TIP, QKeySequence()},

		{KnownActions::SerialToolBar::ATTACH_TO_DIR, QKeySequence()},
		{KnownActions::SerialToolBar::CLOSE, QKeySequence()},
		{KnownActions::SerialToolBar::COMM_APP, QKeySequence()},
		{KnownActions::SerialToolBar::CONFIG_DIALOG, QKeySequence()},
		{KnownActions::SerialToolBar::DE_ATTACH_TO_DIR, QKeySequence()},
		{KnownActions::SerialToolBar::RECEIVE, QKeySequence()},
		{KnownActions::SerialToolBar::SEND, QKeySequence()},
		{KnownActions::SerialToolBar::TEST_DIALOG, QKeySequence()},

		{KnownActions::StatusBar::ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},
		{KnownActions::StatusBar::DE_ATTACH_HIGHLIGHT_TO_DIR, QKeySequence()},

		{KnownActions::Tool::CALC, QKeySequence()},
		{KnownActions::Tool::DIFF, QKeySequence()},
		{KnownActions::Tool::DIFF_EDITOR, QKeySequence()},
		{KnownActions::Tool::DIFF_LEFT, QKeySequence()},
		{KnownActions::Tool::DIFF_RIGHT, QKeySequence()},
		{KnownActions::Tool::INLINE_CALC, QKeySequence(tr("Ctrl+0"))},
		{KnownActions::Tool::SHOW_SERIAL_TOOLBAR, QKeySequence()},

		{KnownActions::Window::CASCADE, QKeySequence()},
		{KnownActions::Window::NEXT, QKeySequence(QKeySequence::Forward)},
		{KnownActions::Window::PREVIOUS, QKeySequence(QKeySequence::Back)},
		{KnownActions::Window::SEPARATOR, QKeySequence()},
		{KnownActions::Window::TILE_HORIZ, QKeySequence()},
		{KnownActions::Window::TILE_VERT, QKeySequence()}
	};
}
