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

#ifndef UTILS_GUESSFILENAME_H
#define UTILS_GUESSFILENAME_H

#include <QString>  // for QString


namespace Utils {
struct FileExt {
	QString name;
	QString ext;
};

FileExt guessFileNameByProgNum(const QString& text);
QString guessFileNameByComments(const QString& text);
FileExt guessFileNameByRegExp(const QString& text, const QString& expression);
}

#endif // UTILS_GUESSFILENAME_H
