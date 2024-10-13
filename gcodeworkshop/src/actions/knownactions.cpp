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

#include <QString>  // for QString

#include <knownactions.h>   // IWYU pragma: associated


const QString& KnownActions::Addons::BHC{"Addons@Bhc"};
const QString& KnownActions::Addons::BLOCK_SKIP_DECREMENT{"Addons@BlockSkipDecrement"};
const QString& KnownActions::Addons::BLOCK_SKIP_INCREMENT{"Addons@BlockSkipIncrement"};
const QString& KnownActions::Addons::BLOCK_SKIP_REMOVE{"Addons@BlockSkipRemove"};
const QString& KnownActions::Addons::CHAMFER{"Addons@Chamfer"};
const QString& KnownActions::Addons::CLEANUP{"Addons@Cleanup"};
const QString& KnownActions::Addons::COMMENT_PARA{"Addons@CommentPara"};
const QString& KnownActions::Addons::COMMENT_SEMI{"Addons@CommentSemi"};
const QString& KnownActions::Addons::COMPILE_MACRO{"Addons@CompileMacro"};
const QString& KnownActions::Addons::DOT{"Addons@Dot"};
const QString& KnownActions::Addons::EMPTY_LINE_INSERT{"Addons@EmptyLineInsert"};
const QString& KnownActions::Addons::EMPTY_LINE_REMOVE{"Addons@EmptyLineRemove"};
const QString& KnownActions::Addons::FEEDS{"Addons@Feeds"};
const QString& KnownActions::Addons::I2M{"Addons@I2m"};
const QString& KnownActions::Addons::I2MPROG{"Addons@I2mProg"};
const QString& KnownActions::Addons::RENUMBER{"Addons@Renumber"};
const QString& KnownActions::Addons::SPACES_INSERT{"Addons@SpaceInsert"};
const QString& KnownActions::Addons::SPACES_REMOVE{"Addons@SpaceRemove"};
const QString& KnownActions::Addons::SPLIT_PROGRAMMS{"Addons@SplitProgramms"};
const QString& KnownActions::Addons::SWAP_AXES{"Addons@SwapAxes"};
const QString& KnownActions::Addons::TRANGLE{"Addons@Triangle"};

const QString& KnownActions::Edit::CONFIG{"Edit@Config"};
const QString& KnownActions::Edit::COPY{"Edit@Copy"};
const QString& KnownActions::Edit::CUT{"Edit@Cut"};
const QString& KnownActions::Edit::DELETE{"Edit@Delete"};
const QString& KnownActions::Edit::FIND{"Edit@Find"};
const QString& KnownActions::Edit::PASTE{"Edit@Paste"};
const QString& KnownActions::Edit::READ_ONLY{"Edit@ReadOnly"};
const QString& KnownActions::Edit::REDO{"Edit@Redo"};
const QString& KnownActions::Edit::REPLACE{"Edit@Replace"};
const QString& KnownActions::Edit::SELECT_ALL{"Edit@SelectAll"};
const QString& KnownActions::Edit::UNDO{"Edit@Undo"};

const QString& KnownActions::File::CLOSE{"File@Close"};
const QString& KnownActions::File::CLOSE_ALL{"File@CloseAll"};
const QString& KnownActions::File::EXIT{"File@Exit"};
const QString& KnownActions::File::FIND_IN_FILES{"File@FindInFiles"};
const QString& KnownActions::File::NEW{"File@New"};
const QString& KnownActions::File::OPEN{"File@Open"};
const QString& KnownActions::File::OPEN_EXAMPLE{"File@OpenExample"};
const QString& KnownActions::File::PRINT{"File@Print"};
const QString& KnownActions::File::PRINT_PREVIEW{"File@PrintPreview"};
const QString& KnownActions::File::SAVE{"File@Save"};
const QString& KnownActions::File::SAVE_ALL{"File@SaveAll"};
const QString& KnownActions::File::SAVE_AS{"File@SaveAs"};
const QString& KnownActions::File::SESSION_MANAGER{"File@SessionManager"};

const QString& KnownActions::FindToolBar::FIND_NEXT{"FindToolBar@FindNext"};
const QString& KnownActions::FindToolBar::FIND_PREVIOUS{"FindToolBar@FindPrevious"};
const QString& KnownActions::FindToolBar::REPLACE_NEXT{"FindToolBar@ReplaceNext"};
const QString& KnownActions::FindToolBar::REPLACE_PREVIOUS{"FindToolBar@ReplacePrevious"};
const QString& KnownActions::FindToolBar::REPLACE_ALL{"FindToolBar@ReplaceAll"};
const QString& KnownActions::FindToolBar::CLOSE{"FindToolBar@Close"};

const QString& KnownActions::Help::ABOUT{"Help@About"};
const QString& KnownActions::Help::ABOUT_QT{"Help@AboutQt"};
const QString& KnownActions::Help::CREATE_GLOBAL_TOOL_TIP{"Help@CreateGlobalToolTip"};
const QString& KnownActions::Help::CREATE_USER_TOOL_TIP{"Help@CreateUserToolTip"};

const QString& KnownActions::SerialToolBar::ATTACH_TO_DIR{"SerialToolBar@AttachToDir"};
const QString& KnownActions::SerialToolBar::CLOSE{"SerialToolBar@Close"};
const QString& KnownActions::SerialToolBar::COMM_APP{"SerialToolBar@CommApp"};
const QString& KnownActions::SerialToolBar::CONFIG_DIALOG{"SerialToolBar@ConfigDialog"};
const QString& KnownActions::SerialToolBar::DE_ATTACH_TO_DIR{"SerialToolBar@DeAttachToDir"};
const QString& KnownActions::SerialToolBar::RECEIVE{"SerialToolBar@Receve"};
const QString& KnownActions::SerialToolBar::SEND{"SerialToolBar@Send"};
const QString& KnownActions::SerialToolBar::TEST_DIALOG{"SerialToolBar@TestDialog"};

const QString& KnownActions::StatusBar::ATTACH_HIGHLIGHT_TO_DIR{"StatusBar@AttachHightToDir"};
const QString& KnownActions::StatusBar::DE_ATTACH_HIGHLIGHT_TO_DIR{"StatusBar@DeAttachHightToDir"};

const QString& KnownActions::Tool::CALC{"Tool@Calc"};
const QString& KnownActions::Tool::DIFF{"Tool@Diff"};
const QString& KnownActions::Tool::DIFF_EDITOR{"Tool@DiffEditor"};
const QString& KnownActions::Tool::DIFF_LEFT{"Tool@DiffLeft"};
const QString& KnownActions::Tool::DIFF_RIGHT{"Tool@DiffRight"};
const QString& KnownActions::Tool::INLINE_CALC{"Tool@InlineCalc"};
const QString& KnownActions::Tool::SHOW_SERIAL_TOOLBAR{"Tool@ShowSerialToolBar"};

const QString& KnownActions::Window::CASCADE{"Window@Cascade"};
const QString& KnownActions::Window::NEXT{"Window@Next"};
const QString& KnownActions::Window::PREVIOUS{"Window@Previous"};
const QString& KnownActions::Window::SEPARATOR{"Window@Separator"};
const QString& KnownActions::Window::TILE_HORIZ{"Window@TileHoriz"};
const QString& KnownActions::Window::TILE_VERT{"Window@TileVert"};
