/*
 *  Copyright (C) 2020 Nick Egorrov, nicegorov@yandex.ru
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

#include <QPushButton>      // for QPushButton
#include <QSharedPointer>   // for QSharedPointer, QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, QTypeInfo<>::isCo...
#include <QStackedLayout>   // for QStackedLayout
#include <QTreeWidget>      // for QTreeWidget, QTreeWidgetItem
#include <QWidget>          // for QWidget
#include <QtGlobal>         // for QForeachContainer, QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, qMakeForeachCo...

#include <utils/configdialog.h> // IWYU pragma: associated
#include <utils/configpage.h>   // for ConfigPage

#include "ui_configdialog.h"    // for Ui::ConfigDialog


class TreeItem: public QTreeWidgetItem
{
public:
	explicit TreeItem(ConfigPage* page):
		QTreeWidgetItem()
	{
		setText(0, page->text());
		setToolTip(0, page->toolTip());
	}
};

ConfigDialog::ConfigDialog(QWidget* parent) :
	QDialog(parent),
	mForm(new Ui::ConfigDialog)
{
	mForm->setupUi(this);
	mStackedLayout = new QStackedLayout;
	mForm->stackWidget->setLayout(mStackedLayout);

	mForm->treeView->setHeaderHidden(true);
	connect(mForm->treeView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
	        SLOT(selectedItemChange(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(mForm->okButton, SIGNAL(clicked()), SLOT(accept()));
	connect(mForm->cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(mForm->defaultButton, SIGNAL(clicked()), SLOT(reset()));
}

ConfigDialog::ConfigDialog(QSharedPointer<ConfigPage> page, QWidget* parent) :
	ConfigDialog(parent)
{
	addPage(page);
}

ConfigDialog::ConfigDialog(QList<QSharedPointer<ConfigPage>> pages, QWidget* parent) :
	ConfigDialog(parent)
{
	addPages(pages);
}

ConfigDialog::~ConfigDialog()
{
	delete mStackedLayout;
	delete mForm;
}

void ConfigDialog::addPage(QSharedPointer<ConfigPage> page)
{
	mPages.append(page);
	mForm->treeView->addTopLevelItem(doItem(page));
}

void ConfigDialog::addPages(QList<QSharedPointer<ConfigPage>> pages)
{
	foreach (QSharedPointer<ConfigPage> page, pages) {
		addPage(page);
	}
}

void ConfigDialog::open()
{
	emit onStart();
	QDialog::open();
}

int ConfigDialog::exec()
{
	emit onStart();
	return QDialog::exec();
}

QTreeWidgetItem* ConfigDialog::doItem(QSharedPointer<ConfigPage> pagePtr)
{
	TreeItem* item = new TreeItem(pagePtr.data());
	mItemPageMap.insert(item, pagePtr);
	mStackedLayout->addWidget(pagePtr->widget());
	connect(this, SIGNAL(onStart()), pagePtr.data(), SLOT(start()));
	connect(this, SIGNAL(accepted()), pagePtr.data(), SLOT(accept()));
	connect(this, SIGNAL(rejected()), pagePtr.data(), SLOT(reject()));
	connect(this, SIGNAL(onRetranslateUI()), pagePtr.data(), SLOT(retranslateUI()));

	QList<QTreeWidgetItem*> subItems;

	foreach (QSharedPointer<ConfigPage> subPage, pagePtr->pages()) {
		subItems.append(doItem(subPage));
	}

	item->addChildren(subItems);

	return item;
}

void ConfigDialog::reset()
{
	emit onReset();
}

void ConfigDialog::selectedItemChange(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	QSharedPointer<ConfigPage> pagePtr;

	if (mItemPageMap.contains(previous)) {
		pagePtr = mItemPageMap.value(previous);

		if (!pagePtr.isNull()) {
			disconnect(this, SIGNAL(onReset()), pagePtr.data(), SLOT(reset()));
		}
	}

	if (mItemPageMap.contains(current)) {
		pagePtr = mItemPageMap.value(current);

		if (!pagePtr.isNull()) {
			connect(this, SIGNAL(onReset()), pagePtr.data(), SLOT(reset()));
			mStackedLayout->setCurrentWidget(pagePtr->widget());
		}
	}
}

void ConfigDialog::retranslateUI()
{
	mForm->retranslateUi(this);

	foreach (QSharedPointer<ConfigPage> page, mItemPageMap) {
		QTreeWidgetItem* item = mItemPageMap.key(page);
		item->setText(0, page->text());
		item->setToolTip(0, page->toolTip());
	}

	emit onRetranslateUI();
}
