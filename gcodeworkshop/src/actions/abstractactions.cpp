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

#include <QAction>      // for QAction
#include <QIcon>        // for QIcon
#include <QKeySequence> // for QKeySequence

#include <abstractactions.h>    // IWYU pragma: associated


AbstractActions::AbstractActions(QObject* parent) : QObject{parent}
{
}

AbstractActions::~AbstractActions()
{
}

void AbstractActions::loadShortcuts(const QMap<QString, QKeySequence>& keymap)
{
	QMap<QString, ActionEx>::iterator end = m_actions.end();

	for (QMap<QString, ActionEx>::iterator i = m_actions.begin(); i != end; ++i) {
		setActionShortcut(i.key(), keymap.value(i.key()));
	}
}

QAction* AbstractActions::makeAction(const QString& id)
{
	ActionEx ae{new QAction(this), QString()};
	m_actions.insert(id, ae);
	return ae.action;
}

QAction* AbstractActions::action(const QString& id) const
{
	return m_actions.value(id).action;
}

void AbstractActions::setActionIcon(const QString& id, const QIcon& icon)
{
	m_actions.value(id).action->setIcon(icon);
}

void AbstractActions::setActionShortcut(const QString& id, const QKeySequence& shortcut)
{
	m_actions.value(id).action->setShortcut(shortcut);
	updateActionToolTip(id);
}

void AbstractActions::setActionText(const QString& id, const QString& text)
{
	m_actions.value(id).action->setText(text);
}

void AbstractActions::setActionExToolTip(const QString& id, const QString& tooltip)
{
	m_actions[id].tooltip = tooltip;
	updateActionToolTip(id);
}

QString AbstractActions::actionExToolTip(const QString& id) const
{
	return m_actions.value(id).tooltip;
}

void AbstractActions::updateActionToolTip(const QString& id)
{
	QAction* act = m_actions[id].action;
	QString tooltip = actionExToolTip(id);

	if (!act->shortcut().isEmpty()) {
		tooltip = QString(tr("%1<p>Shortcut: <b>%2</b>")).arg(tooltip).arg(act->shortcut().toString());
	}

	act->setToolTip(tooltip);
}
