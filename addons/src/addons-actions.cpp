/*
 *  Copyright (C) 2023-2025 Nick Egorrov, nicegorov@yandex.ru
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

#include <functional>   // for function

#include <QAction>          // for QAction
#include <QApplication>     // for QApplication
#include <QIcon>            // for QIcon
#include <QList>            // for QList<>::iterator
#include <QMainWindow>      // for QMainWindow
#include <QMessageBox>      // for QMessageBox
#include <QStringList>      // for QStringList
#include <Qt>               // for BusyCursor

#include <addons-actions.h>     // IWYU pragma: no_include "abstractactions.h"
#include <document.h>           // for Document
#include <gcoderdocument.h>     // for GCoderDocument
#include <gcodeworkshop.h>      // for GCodeWorkShop
#include <knownactions.h>       // for KnownActions
#include <ui/longjobhelper.h>   // for LongJobHelper, LongJobHelper::CANCEL
#include <utils/medium.h>       // for Medium
#include <utils/splitfile.h>    // for splitFile

#include "addons-context.h" // for Context, Context::ALL, Context::SELECTED, Context::SELECTED_BLOCKS

#include "bhc/addons-bhc.h"
#include "blockskip/utils-blockskip.h"
#include "chamfer/addons-chamfer.h"
#include "cleanup/addons-cleanup.h"
#include "comment/utils-comment.h"
#include "compilemacro/utils-compilemacro.h"
#include "dot/addons-dot.h"
#include "emptylines/utils-emptylines.h"
#include "feeds/addons-feeds.h"
#include "i2m/addons-i2m.h"
#include "i2mprog/addons-i2mprog.h"
#include "renumber/addons-renumber.h"
#include "spaces/utils-spaces.h"
#include "swapaxes/addons-swapaxes.h"
#include "triangle/addons-triangle.h"


Addons::Actions::Actions(GCodeWorkShop* parent) : AbstractActions(parent),
	m_app(parent)
{
	connect(makeAction(KnownActions::Addons::BHC), SIGNAL(triggered()), this, SLOT(doBhc()));
	connect(makeAction(KnownActions::Addons::BLOCK_SKIP_DECREMENT), SIGNAL(triggered()), this,
	        SLOT(doBlockSkipDecrement()));
	connect(makeAction(KnownActions::Addons::BLOCK_SKIP_INCREMENT), SIGNAL(triggered()), this,
	        SLOT(doBlockSkipIncrement()));
	connect(makeAction(KnownActions::Addons::BLOCK_SKIP_REMOVE), SIGNAL(triggered()), this, SLOT(doBlockSkipRemove()));
	connect(makeAction(KnownActions::Addons::CHAMFER), SIGNAL(triggered()), this, SLOT(doChamfer()));
	connect(makeAction(KnownActions::Addons::CLEANUP), SIGNAL(triggered()), this, SLOT(doCleanUp()));
	connect(makeAction(KnownActions::Addons::COMMENT_PARA), SIGNAL(triggered()), this, SLOT(doParaComment()));
	connect(makeAction(KnownActions::Addons::COMMENT_SEMI), SIGNAL(triggered()), this, SLOT(doSemiComment()));
	connect(makeAction(KnownActions::Addons::COMPILE_MACRO), SIGNAL(triggered()), this, SLOT(doCompileMacro()));
	connect(makeAction(KnownActions::Addons::DOT), SIGNAL(triggered()), this, SLOT(doDot()));
	connect(makeAction(KnownActions::Addons::EMPTY_LINE_INSERT), SIGNAL(triggered()), this, SLOT(doInsertEmptyLines()));
	connect(makeAction(KnownActions::Addons::EMPTY_LINE_REMOVE), SIGNAL(triggered()), this, SLOT(doRemoveEmptyLines()));
	connect(makeAction(KnownActions::Addons::FEEDS), SIGNAL(triggered()), this, SLOT(doFeeds()));
	connect(makeAction(KnownActions::Addons::I2M), SIGNAL(triggered()), this, SLOT(doI2M()));
	connect(makeAction(KnownActions::Addons::I2MPROG), SIGNAL(triggered()), this, SLOT(doI2MProg()));
	connect(makeAction(KnownActions::Addons::RENUMBER), SIGNAL(triggered()), this, SLOT(doRenumber()));
	connect(makeAction(KnownActions::Addons::SPACES_INSERT), SIGNAL(triggered()), this, SLOT(doInsertSpaces()));
	connect(makeAction(KnownActions::Addons::SPACES_REMOVE), SIGNAL(triggered()), this, SLOT(doRemoveSpaces()));
	connect(makeAction(KnownActions::Addons::SPLIT_PROGRAMMS), SIGNAL(triggered()), this, SLOT(doSplitProgramms()));
	connect(makeAction(KnownActions::Addons::SWAP_AXES), SIGNAL(triggered()), this, SLOT(doSwapAxes()));
	connect(makeAction(KnownActions::Addons::TRANGLE), SIGNAL(triggered()), this, SLOT(doTriangle()));

	Actions::loadIcons();
	Actions::loadTranslations();
}

Addons::Actions::~Actions()
{
}

void Addons::Actions::loadTranslations()
{
	setActionText(KnownActions::Addons::BHC, tr("&Bolt hole circle"));
	setActionExToolTip(KnownActions::Addons::BHC, tr("Calculate bolt hole's positions"));
	setActionText(KnownActions::Addons::BLOCK_SKIP_DECREMENT, tr("Block Skip -"));
	setActionExToolTip(KnownActions::Addons::BLOCK_SKIP_DECREMENT, tr("Insert/decrease Block Skip /"));
	setActionText(KnownActions::Addons::BLOCK_SKIP_INCREMENT, tr("Block Skip +"));
	setActionExToolTip(KnownActions::Addons::BLOCK_SKIP_INCREMENT, tr("Insert/increase Block Skip /"));
	setActionText(KnownActions::Addons::BLOCK_SKIP_REMOVE, tr("Block Skip remove"));
	setActionExToolTip(KnownActions::Addons::BLOCK_SKIP_REMOVE, tr("Remove Block Skip /"));
	setActionText(KnownActions::Addons::CHAMFER, tr("Chamfer"));
	setActionExToolTip(KnownActions::Addons::CHAMFER, tr("Calculate chamfer"));
	setActionText(KnownActions::Addons::CLEANUP, tr("Clean &up"));
	setActionExToolTip(KnownActions::Addons::CLEANUP, tr("Remove text using regular expressions"));
	setActionText(KnownActions::Addons::COMMENT_PARA, tr("Comment ()"));
	setActionExToolTip(KnownActions::Addons::COMMENT_PARA, tr("Comment/uncomment selected text using parentheses"));
	setActionText(KnownActions::Addons::COMMENT_SEMI, tr("Comment ;"));
	setActionExToolTip(KnownActions::Addons::COMMENT_SEMI, tr("Comment/uncomment selected text using semicolon"));
	setActionText(KnownActions::Addons::COMPILE_MACRO, tr("Compile macro - experimental"));
	setActionExToolTip(KnownActions::Addons::COMPILE_MACRO, tr("Translate EdytorNC macro into CNC program"));
	setActionText(KnownActions::Addons::DOT, tr("Insert dots"));
	setActionExToolTip(KnownActions::Addons::DOT, tr("Inserts decimal dot"));
	setActionText(KnownActions::Addons::EMPTY_LINE_INSERT, tr("Insert empty lines"));
	setActionExToolTip(KnownActions::Addons::EMPTY_LINE_INSERT, tr("Inserts empty lines"));
	setActionText(KnownActions::Addons::EMPTY_LINE_REMOVE, tr("Remove empty lines"));
	setActionExToolTip(KnownActions::Addons::EMPTY_LINE_REMOVE, tr("Removes empty lines"));
	setActionText(KnownActions::Addons::FEEDS, tr("Feed's speed's"));
	setActionExToolTip(KnownActions::Addons::FEEDS, tr("Calculate speed, feed, cutting speed"));
	setActionText(KnownActions::Addons::I2M, tr("Convert inch <-> mm"));
	setActionExToolTip(KnownActions::Addons::I2M, tr("Convert inch <-> mm"));
	setActionText(KnownActions::Addons::I2MPROG, tr("Convert program inch <-> mm"));
	setActionExToolTip(KnownActions::Addons::I2MPROG, tr("Convert program inch <-> mm"));
	setActionText(KnownActions::Addons::RENUMBER, tr("Renumber"));
	setActionExToolTip(KnownActions::Addons::RENUMBER, tr("Renumber program blocks"));
	setActionText(KnownActions::Addons::SPACES_INSERT, tr("&Insert spaces"));
	setActionExToolTip(KnownActions::Addons::SPACES_INSERT, tr("Inserts spaces"));
	setActionText(KnownActions::Addons::SPACES_REMOVE, tr("Remove spaces"));
	setActionExToolTip(KnownActions::Addons::SPACES_REMOVE, tr("Removes spaces"));
	setActionText(KnownActions::Addons::SPLIT_PROGRAMMS, tr("Split file"));
	setActionExToolTip(KnownActions::Addons::SPLIT_PROGRAMMS, tr("Split file"));
	setActionText(KnownActions::Addons::SWAP_AXES, tr("Swap axes"));
	setActionExToolTip(KnownActions::Addons::SWAP_AXES, tr("Swap/modify axes, selected text or entire program"));
	setActionText(KnownActions::Addons::TRANGLE, tr("Solution of triangles"));
	setActionExToolTip(KnownActions::Addons::TRANGLE, tr("Solution of triangles"));
}

void Addons::Actions::loadIcons()
{
	setActionIcon(KnownActions::Addons::BHC, QIcon(":/images/bhc.png"));
	setActionIcon(KnownActions::Addons::BLOCK_SKIP_DECREMENT, QIcon(":/images/blockskip-.png"));
	setActionIcon(KnownActions::Addons::BLOCK_SKIP_INCREMENT, QIcon(":/images/blockskip+.png"));
	setActionIcon(KnownActions::Addons::BLOCK_SKIP_REMOVE, QIcon(":/images/blockskipr.png"));
	setActionIcon(KnownActions::Addons::CHAMFER, QIcon(":/images/chamfer.png"));
	setActionIcon(KnownActions::Addons::CLEANUP, QIcon(":/images/cleanup.png"));
	setActionIcon(KnownActions::Addons::COMMENT_PARA, QIcon(":/images/paracomment.pn"));
	setActionIcon(KnownActions::Addons::COMMENT_SEMI, QIcon(":/images/semicomment.png"));
	setActionIcon(KnownActions::Addons::COMPILE_MACRO, QIcon(":/images/compfile.png"));
	setActionIcon(KnownActions::Addons::DOT, QIcon(":/images/dots.png"));
	setActionIcon(KnownActions::Addons::EMPTY_LINE_INSERT, QIcon(":/images/insertemptylines.png"));
	setActionIcon(KnownActions::Addons::EMPTY_LINE_REMOVE, QIcon(":/images/removeemptylines.png"));
	setActionIcon(KnownActions::Addons::FEEDS, QIcon(":/images/vcf.png"));
	setActionIcon(KnownActions::Addons::I2M, QIcon(":/images/i2m.png"));
	setActionIcon(KnownActions::Addons::I2MPROG, QIcon(":/images/i2mprog.png"));
	setActionIcon(KnownActions::Addons::RENUMBER, QIcon(":/images/renumber.png"));
	setActionIcon(KnownActions::Addons::SPACES_INSERT, QIcon(":/images/insertspc.png"));
	setActionIcon(KnownActions::Addons::SPACES_REMOVE, QIcon(":/images/removespc.png"));
	setActionIcon(KnownActions::Addons::SPLIT_PROGRAMMS, QIcon(":/images/split_prog.png"));
	setActionIcon(KnownActions::Addons::SWAP_AXES, QIcon(":/images/swapaxes.png"));
	setActionIcon(KnownActions::Addons::TRANGLE, QIcon(":/images/triangles.png"));
}

QAction* Addons::Actions::bhc()
{
	return action(KnownActions::Addons::BHC);
}

QAction* Addons::Actions::blockSkipDecrement()
{
	return action(KnownActions::Addons::BLOCK_SKIP_DECREMENT);
}

QAction* Addons::Actions::blockSkipIncrement()
{
	return action(KnownActions::Addons::BLOCK_SKIP_INCREMENT);
}

QAction* Addons::Actions::blockSkipRemove()
{
	return action(KnownActions::Addons::BLOCK_SKIP_REMOVE);
}

QAction* Addons::Actions::chamfer()
{
	return action(KnownActions::Addons::CHAMFER);
}

QAction* Addons::Actions::cleanUp()
{
	return action(KnownActions::Addons::CLEANUP);
}

QAction* Addons::Actions::paraComment()
{
	return action(KnownActions::Addons::COMMENT_PARA);
}

QAction* Addons::Actions::semiComment()
{
	return action(KnownActions::Addons::COMMENT_SEMI);
}

QAction* Addons::Actions::compileMacro()
{
	return action(KnownActions::Addons::COMPILE_MACRO);
}

QAction* Addons::Actions::dot()
{
	return action(KnownActions::Addons::DOT);
}

QAction* Addons::Actions::insertEmptyLines()
{
	return action(KnownActions::Addons::EMPTY_LINE_INSERT);
}

QAction* Addons::Actions::removeEmptyLines()
{
	return action(KnownActions::Addons::EMPTY_LINE_REMOVE);
}

QAction* Addons::Actions::feeds()
{
	return action(KnownActions::Addons::FEEDS);
}

QAction* Addons::Actions::i2m()
{
	return action(KnownActions::Addons::I2M);
}

QAction* Addons::Actions::i2mProg()
{
	return action(KnownActions::Addons::I2MPROG);
}

QAction* Addons::Actions::renumber()
{
	return action(KnownActions::Addons::RENUMBER);
}

QAction* Addons::Actions::insertSpaces()
{
	return action(KnownActions::Addons::SPACES_INSERT);
}

QAction* Addons::Actions::removeSpaces()
{
	return action(KnownActions::Addons::SPACES_REMOVE);
}

QAction* Addons::Actions::splitProgramms()
{
	return action(KnownActions::Addons::SPLIT_PROGRAMMS);
}

QAction* Addons::Actions::swapAxes()
{
	return action(KnownActions::Addons::SWAP_AXES);
}

QAction* Addons::Actions::triangle()
{
	return action(KnownActions::Addons::TRANGLE);
}

void Addons::Actions::doBhc()
{
	Addons::doBhc(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doBlockSkip(bool remove, bool inc)
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED_BLOCKS)) {
		return;
	}

	Utils::blockSkip(ctx.text(), remove, inc);
	ctx.setSelectionStart(ctx.fragmentStart());
	ctx.setSelectionEnd(ctx.fragmentStart() + ctx.text().length());
	ctx.push();
}

void Addons::Actions::doBlockSkipDecrement()
{
	doBlockSkip(false, false);
}

void Addons::Actions::doBlockSkipIncrement()
{
	doBlockSkip(false, true);
}

void Addons::Actions::doBlockSkipRemove()
{
	doBlockSkip(true, true);
}

void Addons::Actions::doChamfer()
{
	Addons::doChamfer(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doCleanUp()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	if (Addons::doCleanUp(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doParaComment()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Comments/uncomments"));

	bool changed = Utils::autoComments(ctx.text(), Utils::ParenthesisComments, [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doSemiComment()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Comments/uncomments"));

	bool changed = Utils::autoComments(ctx.text(), Utils::SemicolonComments, [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doCompileMacro()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	Utils::CompileMacro compiler;

	if (compiler.compile(ctx.text()) == -1) {
		QMessageBox::warning(m_app->mainWindow(), tr("GCodeWorkShop - compile macro"), compiler.status());
		//return;
	}

	GCoderDocument* gdoc = dynamic_cast<GCoderDocument*>(m_app->newFile());

	if (gdoc) {
		gdoc->insertText(compiler.result());
	}
}

void Addons::Actions::doDot()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	if (Addons::doDot(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doInsertEmptyLines()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Inserting empty lines"));

	bool changed = Utils::insertEmptyLines(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doRemoveEmptyLines()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{GCodeWorkShop::instance()};
	helper.begin(ctx.text().length(), tr("Removing empty lines"));

	bool changed = Utils::removeEmptyLines(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doFeeds()
{
	Addons::doFeeds(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doI2M()
{
	Addons::doI2M(m_app->mainWindow(), Medium::instance().settings());
}

void Addons::Actions::doI2MProg()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	if (Addons::doI2MProg(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doRenumber()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED_OR_ALL)) {
		return;
	}

	if (Addons::doRenumber(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doInsertSpaces()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{m_app->mainWindow()};
	helper.begin(ctx.text().length(), tr("Inserting space"));

	int changed = Utils::insertSpaces(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doRemoveSpaces()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	LongJobHelper helper{m_app->mainWindow()};
	helper.begin(ctx.text().length(), tr("Removing space"));

	int changed = Utils::removeSpaces(ctx.text(), [&helper](int pos) -> bool{
		return helper.check(pos) == LongJobHelper::CANCEL;
	});

	helper.end();

	if (changed) {
		ctx.push();
	}
}

void Addons::Actions::doSplitProgramms()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::ALL)) {
		return;
	}

	QApplication::setOverrideCursor(Qt::BusyCursor);

	QStringList list = Utils::splitFile(ctx.text());

	if (list.size() <= 1) {
		QApplication::restoreOverrideCursor();
		return;
	}

	for (QString it : list) {
		GCoderDocument* gdoc = dynamic_cast<GCoderDocument*>(m_app->newFile());

		if (gdoc == nullptr) {
			continue;
		}

		gdoc->setText(it);
		gdoc->clearUndoRedoStacks();
	}

	QApplication::restoreOverrideCursor();
}

void Addons::Actions::doSwapAxes()
{
	Addons::Context ctx;

	if (!ctx.pull(Addons::Context::SELECTED_OR_ALL)) {
		return;
	}

	if (Addons::doSwapAxes(m_app->mainWindow(), Medium::instance().settings(), ctx.text())) {
		ctx.push();
	}
}

void Addons::Actions::doTriangle()
{
	Addons::doTriangle(m_app->mainWindow(), Medium::instance().settings());
}
