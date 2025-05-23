/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
 *  Copyright (C) 2023 Nick Egorrov, nicegorov@yandex.ru
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

#include <QCheckBox>    // for QCheckBox
#include <QLineEdit>    // for QLineEdit
#include <QPoint>       // for QPoint
#include <QPushButton>  // for QPushButton
#include <QRect>        // for QRect
#include <QSettings>    // for QSettings
#include <QSize>        // for QSize
#include <QSpinBox>     // for QSpinBox
#include <QString>      // for QString
#include <QVariant>     // for QVariant
#include <QtGlobal>     // for Q_UNUSED

class QWidget;

#include "dotdialog.h"
#include "dotoptions.h" // for DotOptions


#define CFG_SECTION  "DotDialog"
#define CFG_KEY_POS  "Position"
#define CFG_KEY_SIZE "Size"


DotDialog::DotDialog(QWidget* parent, QSettings* settings) :
	QDialog(parent)
{
	setupUi(this);

	mSettings = settings;

	setWindowTitle(tr("Insert dots"));

	connect(mInput, SIGNAL(textChanged(const QString&)), this,
	        SLOT(inputChanged(const QString&)));
	connect(mCheckAtEnd, SIGNAL(clicked()), this, SLOT(atEndClicked()));
	connect(mCheckDivide, SIGNAL(clicked()), this, SLOT(divideClicked()));
	connect(mSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxVal(int)));
	connect(okButton, SIGNAL(clicked()), SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(cancelButton, SIGNAL(clicked()), SLOT(close()));
	connect(this, SIGNAL(finished(int)), SLOT(onFinished(int)));

	setMaximumSize(width(), height());
	okButton->setEnabled(mInput->hasAcceptableInput());
}

DotDialog::~DotDialog()
{
}

void DotDialog::inputChanged(const QString& text)
{
	Q_UNUSED(text);
	okButton->setEnabled(mInput->hasAcceptableInput());
}

void DotDialog::atEndClicked()
{
	mCheckDivide->setChecked(!mCheckAtEnd->isChecked());
	mSpinBox->setEnabled(mCheckDivide->isChecked());
}

void DotDialog::divideClicked()
{
	mCheckAtEnd->setChecked(!mCheckDivide->isChecked());
	mSpinBox->setEnabled(mCheckDivide->isChecked());
}

void DotDialog::spinBoxVal(int val)
{
	if (val == 99) {
		mSpinBox->setValue(10);
	} else if (val == 999) {
		mSpinBox->setValue(10);
	} else if (val == 9999) {
		mSpinBox->setValue(100);
	}

	if (val == 11) {
		mSpinBox->setValue(100);
	} else if (val == 101) {
		mSpinBox->setValue(1000);
	} else if (val == 1001) {
		mSpinBox->setValue(10000);
	}
}

void DotDialog::setOptions(const DotOptions& options)
{
	mInput->setText(options.axes);
	mCheckAtEnd->setChecked(!options.convert);
	mCheckDivide->setChecked(options.convert);
	mSpinBox->setValue(options.divider);

	okButton->setEnabled(mInput->hasAcceptableInput());
	divideClicked();
}

DotOptions DotDialog::options()
{
	DotOptions options;

	options.axes = mInput->text();
	options.convert = mCheckDivide->isChecked();
	options.divider = mSpinBox->value();

	return options;
}

void DotDialog::loadSettings()
{
	if (mSettings.isNull()) {
		return;
	}

	mSettings->beginGroup(CFG_SECTION);

	if (mSettings->contains(CFG_KEY_POS)) {
		QPoint pos = mSettings->value(CFG_KEY_POS, geometry().topLeft()).toPoint();
		QSize size = mSettings->value(CFG_KEY_SIZE, geometry().size()).toSize();
		setGeometry(QRect(pos, size));
	}

	DotOptions opt;
	opt.load(mSettings);

	mSettings->endGroup();

	setOptions(opt);
}

void DotDialog::saveSettings(bool saveOptions)
{
	if (mSettings.isNull()) {
		return;
	}

	mSettings->beginGroup(CFG_SECTION);

	mSettings->setValue(CFG_KEY_POS, geometry().topLeft());
	mSettings->setValue(CFG_KEY_SIZE, geometry().size());

	if (saveOptions) {
		options().save(mSettings);
	}

	mSettings->endGroup();
}

void DotDialog::onFinished(int result)
{
	saveSettings(result == QDialog::Accepted);
}
