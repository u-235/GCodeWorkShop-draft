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

#ifndef FINDTOOLBAR_H
#define FINDTOOLBAR_H

#include <QObject>  // for QObject, Q_OBJECT, slots
#include <QString>  // for QString
#include <QToolBar>

class QCheckBox;
class QCloseEvent;
class QHideEvent;
class QLabel;
class QLineEdit;
class QSettings;

class AbstractActions;
class CapsLockEventFilter;
class FindToolBarActions;
class GCoderDocument;
class GCodeWorkShop;


class FindToolBar : public QToolBar
{
	Q_OBJECT

public:
	explicit FindToolBar(GCodeWorkShop* parent);
	~FindToolBar();

public slots:
	bool findNext();
	bool findPrevious();
	void replaceNext();
	void replacePrevious();
	void replaceAll();

	void highlight();

public:
	void activate(bool replace);
	void deActivate();

	enum ReturnReaction {
		NO, REVERT_FOCUS, FIND_NEXT
	};

	void setReturnReaction(ReturnReaction reaction);
	ReturnReaction returnReaction() const;

protected:
	bool findInDocument(GCoderDocument* gdoc, bool backward);
	bool replaceInDocument(GCoderDocument* gdoc, bool backward, bool all);

	void hideEvent(QHideEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

protected slots:
	void findReturnPressed();
	void findTextChanged();

public slots:
	void setCapsLockEnable(bool enable);

	void loadTranslations();
	void loadIcons();

	void loadSettings(QSettings* cfg);
	void saveSettings(QSettings* cfg) const;

public:
	AbstractActions* actions() const;

	bool isWholeWords() const;
	bool isIgnoreCase() const;
	bool isIgnoreComments() const;

	QString textForFind() const;
	void setTextForFind(const QString& text);

	QString textForReplace() const;

protected:
	GCodeWorkShop* m_app;
	FindToolBarActions* m_actions;
	ReturnReaction m_returnReaction;
	QLabel* m_findLabel;
	QLabel* m_replaceLabel;
	QLineEdit* m_findEdit;
	QLineEdit* m_replaceEdit;
	QCheckBox* m_wholeWords;
	QCheckBox* m_ignoreCase;
	QCheckBox* m_ignoreComments;
	CapsLockEventFilter* m_findEditEventFilter;
	CapsLockEventFilter* m_replaceEditEventFilter;
};

#endif // FINDTOOLBAR_H
