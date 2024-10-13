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

#ifndef KNOWNACTIONS_H
#define KNOWNACTIONS_H

class QString;


struct KnownActions {
	struct Addons {
		static const QString& BHC;
		static const QString& BLOCK_SKIP_DECREMENT;
		static const QString& BLOCK_SKIP_INCREMENT;
		static const QString& BLOCK_SKIP_REMOVE;
		static const QString& CHAMFER;
		static const QString& CLEANUP;
		static const QString& COMMENT_PARA;
		static const QString& COMMENT_SEMI;
		static const QString& COMPILE_MACRO;
		static const QString& DOT;
		static const QString& EMPTY_LINE_INSERT;
		static const QString& EMPTY_LINE_REMOVE;
		static const QString& FEEDS;
		static const QString& I2M;
		static const QString& I2MPROG;
		static const QString& RENUMBER;
		static const QString& SPACES_INSERT;
		static const QString& SPACES_REMOVE;
		static const QString& SPLIT_PROGRAMMS;
		static const QString& SWAP_AXES;
		static const QString& TRANGLE;
	};

	struct Edit {
		static const QString& CONFIG;
		static const QString& COPY;
		static const QString& CUT;
		static const QString& DELETE;
		static const QString& FIND;
		static const QString& PASTE;
		static const QString& READ_ONLY;
		static const QString& REDO;
		static const QString& REPLACE;
		static const QString& SELECT_ALL;
		static const QString& UNDO;
	};

	struct File {
		static const QString& CLOSE;
		static const QString& CLOSE_ALL;
		static const QString& EXIT;
		static const QString& FIND_IN_FILES;
		static const QString& NEW;
		static const QString& OPEN;
		static const QString& OPEN_EXAMPLE;
		static const QString& PRINT;
		static const QString& PRINT_PREVIEW;
		static const QString& SAVE;
		static const QString& SAVE_ALL;
		static const QString& SAVE_AS;
		static const QString& SESSION_MANAGER;
	};

	struct FindToolBar {
		static const QString& FIND_NEXT;
		static const QString& FIND_PREVIOUS;
		static const QString& REPLACE_NEXT;
		static const QString& REPLACE_PREVIOUS;
		static const QString& REPLACE_ALL;
		static const QString& CLOSE;
	};

	struct Help {
		static const QString& ABOUT;
		static const QString& ABOUT_QT;
		static const QString& CREATE_GLOBAL_TOOL_TIP;
		static const QString& CREATE_USER_TOOL_TIP;
	};

	struct SerialToolBar {
		static const QString& ATTACH_TO_DIR;
		static const QString& CLOSE;
		static const QString& COMM_APP;
		static const QString& CONFIG_DIALOG;
		static const QString& DE_ATTACH_TO_DIR;
		static const QString& RECEIVE;
		static const QString& SEND;
		static const QString& TEST_DIALOG;
	};

	struct StatusBar {
		static const QString& ATTACH_HIGHLIGHT_TO_DIR;
		static const QString& DE_ATTACH_HIGHLIGHT_TO_DIR;
	};

	struct Tool {
		static const QString& CALC;
		static const QString& DIFF;
		static const QString& DIFF_EDITOR;
		static const QString& DIFF_LEFT;
		static const QString& DIFF_RIGHT;
		static const QString& INLINE_CALC;
		static const QString& SHOW_SERIAL_TOOLBAR;
	};

	struct Window {
		static const QString& TILE_HORIZ;
		static const QString& TILE_VERT;
		static const QString& CASCADE;
		static const QString& NEXT;
		static const QString& PREVIOUS;
		static const QString& SEPARATOR;
	};
};

#endif // KNOWNACTIONS_H
