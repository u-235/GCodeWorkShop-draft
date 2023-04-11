/*
 *  Copyright (C) 2023 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This file is part of EdytorNC.
 *
 *  EdytorNC is free software: you can redistribute it and/or modify
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

#ifndef ADDONS_ACTIONS_H
#define ADDONS_ACTIONS_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // for QString

class QAction;


namespace Addons {
class Actions : public QObject
{
    Q_OBJECT

public:
    explicit Actions(QObject *parent = nullptr);

public slots:
    void loadTranslations();
    void loadIcons();

public:
    // *INDENT-OFF*
    QAction *bhc() {return m_bhc;}
    // *INDENT-ON*

protected:
    QAction *m_bhc;

protected slots:
    void doBhc();
};
} // namespace Addons

#endif // ADDONS_ACTIONS_H
