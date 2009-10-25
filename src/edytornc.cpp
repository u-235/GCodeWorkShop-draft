/***************************************************************************
 *   Copyright (C) 2006-2009 by Artur Kozioł                               *
 *   artkoz@poczta.onet.pl                                                 *
 *                                                                         *
 *   This file is part of EdytorNC.                                        *
 *                                                                         *
 *   EdytorNC is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>

#include "edytornc.h"
#include "mdichild.h"




//**************************************************************************************************
//
//**************************************************************************************************

edytornc::edytornc()
{
    setAttribute(Qt::WA_DeleteOnClose);

    findToolBar = NULL;
    serialToolBar = NULL;
    clipboard = QApplication::clipboard();
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(updateMenus()));

    mdiArea = new QMdiArea;
    setCentralWidget(mdiArea);


    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(updateMenus()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget *)), this, SLOT(setActiveSubWindow(QWidget *)));
    


    createActions();
    createMenus();
    createToolBars();

    updateMenus();

    readSettings();

    if(tabbedView)
      mdiArea->setViewMode(QMdiArea::TabbedView);
    else
      mdiArea->setViewMode(QMdiArea::SubWindowView);

    setWindowTitle(tr("EdytorNC"));
    setWindowIcon(QIcon(":/images/edytornc.png"));
    createStatusBar();
}

//**************************************************************************************************
//
//**************************************************************************************************

edytornc::~edytornc()
{
   proc = findChild<QProcess *>();
   if(proc)
   {
      proc->close();
      delete(proc);
   };
}    

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::closeEvent(QCloseEvent *event)
{
    setUpdatesEnabled(FALSE);
    writeSettings();

    foreach(QMdiSubWindow *window, mdiArea->subWindowList(QMdiArea::StackingOrder))
    {
       MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
       if(mdiChild->textEdit->document()->isModified())
       {
          setUpdatesEnabled(TRUE);
          mdiChild->activateWindow();
          mdiChild->raise();
          if(!mdiChild->parentWidget()->close())
          {
              event->ignore();
              return;
          };
          setUpdatesEnabled(FALSE);
       };
    };

    mdiArea->closeAllSubWindows();

    if(activeMdiChild())
    {
       event->ignore();
    } 
    else 
    {  
       event->accept();
    }
    setUpdatesEnabled(TRUE);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::newFile()
{
    MdiChild *child = createMdiChild();

    child->newFile();
    defaultMdiWindowProperites.cursorPos = 0;
    defaultMdiWindowProperites.readOnly = FALSE;
    //defaultMdiWindowProperites.maximized = FALSE;
    defaultMdiWindowProperites.geometry = QByteArray();
    child->setMdiWindowProperites(defaultMdiWindowProperites);

    if(defaultMdiWindowProperites.maximized)
      child->showMaximized();
    else
      child->showNormal();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::open()
{
    QFileInfo file;
    QMdiSubWindow *existing;

    existing = 0;

#ifdef Q_OS_LINUX
    QString filters = tr("CNC programs files *.nc (*.nc);;"
                         "CNC programs files *.nc *.min *.anc *.cnc (*.nc *.min *.anc *.cnc);;"
                         "Text files *.txt (*.txt);; All files (*.* *)");
#endif

#ifdef Q_OS_WIN32
  QString filters = tr("CNC programs files (*.nc);;"
                       "CNC programs files (*.nc *.min *.anc *.cnc);;"
                       "Text files (*.txt);; All files (*.* *)");
#endif



    QStringList files = QFileDialog::getOpenFileNames(
                         this,
                         tr("Select one or more files to open"),
                         lastDir.absolutePath(),
                         filters, &openFileFilter);

    //qDebug()<< openFileFilter;


    QStringList::Iterator it = files.begin();
    while(it != files.end())
    {
       file.setFile(*it);
       existing = findMdiChild(*it);

       if((file.exists()) && (file.isReadable()) && !existing)
       {
          lastDir = file.absoluteDir();
          MdiChild *child = createMdiChild();
          if(child->loadFile(*it))
          {
             defaultMdiWindowProperites.cursorPos = 0;
             defaultMdiWindowProperites.readOnly = FALSE;
             defaultMdiWindowProperites.geometry = QByteArray();
             child->setMdiWindowProperites(defaultMdiWindowProperites);
             if(defaultMdiWindowProperites.maximized)
               child->showMaximized();
             else
               child->showNormal();
             updateRecentFiles(*it);
          }
          else
          {
             child->parentWidget()->close();
          };
       };
       ++it;
    };
    if(existing)
      mdiArea->setActiveSubWindow(existing);

    statusBar()->showMessage(tr("File loaded"), 2000);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::openFile(const QString fileName)
{
   QFileInfo file;

   file.setFile(fileName);

   QMdiSubWindow *existing = findMdiChild(fileName);

   if((file.exists()) && (file.isReadable()) && !existing)
   {
      lastDir = file.absoluteDir();
      MdiChild *child = createMdiChild();
      if(child->loadFile(fileName))
      {
         defaultMdiWindowProperites.cursorPos = 0;
         defaultMdiWindowProperites.readOnly = FALSE;
         //defaultMdiWindowProperites.maximized = FALSE;
         defaultMdiWindowProperites.geometry = QByteArray();
         child->setMdiWindowProperites(defaultMdiWindowProperites);
         if(defaultMdiWindowProperites.maximized)
           child->showMaximized();
         else
           child->showNormal();
      }
      else
      {
         child->parentWidget()->close();
      };
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::openWithPreview()
{

    CustomFDialog *fileDialog;
    QStringList filters;
    QFileInfo file;



    fileDialog = new CustomFDialog(this, fdShowPreview + fdExistingFiles);


    filters << tr("CNC programs files (*.nc)")
            << tr("CNC programs files (*.nc *.min *.anc *.cnc)")
            << tr("Text files (*.txt)")
            << tr("All files (*.* *)");

    fileDialog->setNameFilters(filters);

    if(defaultMdiWindowProperites.syntaxH)
      fileDialog->setHighlightColors(defaultMdiWindowProperites.hColors);

    fileDialog->restoreState(fileDialogState);
    fileDialog->selectNameFilter(openFileFilter);


    if (fileDialog->exec() == QDialog::Accepted)
    {
       fileDialogState = fileDialog->saveState();
       openFileFilter = fileDialog->selectedNameFilter();


       QStringList list = fileDialog->selectedFiles();
       QStringList::Iterator it = list.begin();
       while( it != list.end() )
       {
          file.setFile (*it);
          QMdiSubWindow *existing = findMdiChild(*it);

          if((file.exists ()) && (file.isReadable ()) && !existing)
          {
             MdiChild *child = createMdiChild();
             if(child->loadFile(*it))
             {
                defaultMdiWindowProperites.cursorPos = 0;
                defaultMdiWindowProperites.readOnly = FALSE;
                //defaultMdiWindowProperites.maximized = FALSE;
                defaultMdiWindowProperites.geometry = QByteArray();
                child->setMdiWindowProperites(defaultMdiWindowProperites);
                if(defaultMdiWindowProperites.maximized)
                  child->showMaximized();
                else
                  child->showNormal();
                updateRecentFiles(*it);
             }
             else
             {
                child->parentWidget()->close();
             };
          };
          ++it;
       };
       statusBar()->showMessage(tr("File loaded"), 2000);
    };

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::save()
{
    if(activeMdiChild() && activeMdiChild()->save())
      statusBar()->showMessage(tr("File saved"), 2000);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::saveAs()
{
    if(activeMdiChild() && activeMdiChild()->saveAs())
      statusBar()->showMessage(tr("File saved"), 2000);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::printFile()
{
   if(activeMdiChild())
   {
      QTextDocument *document = activeMdiChild()->textEdit->document();
      QPrinter printer;

      QPrintDialog *dlg = new QPrintDialog(&printer, this);
      if(dlg->exec() != QDialog::Accepted)
        return;

      document->print(&printer);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::cut()
{
    if(activeMdiChild())
      activeMdiChild()->textEdit->cut();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::copy()
{
    if(activeMdiChild())
      activeMdiChild()->textEdit->copy();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::findInFl()
{

   if((findFiles = findChild<FindInFiles *>()) != 0)
   { 
      return;
   };
   findFiles = new FindInFiles(this);

   if(defaultMdiWindowProperites.syntaxH)
     findFiles->setHighlightColors(defaultMdiWindowProperites.hColors);

   if(activeMdiChild())
     findFiles->setDir(QFileInfo(activeMdiChild()->currentFile()).canonicalPath());

   connect(findFiles, SIGNAL(fileClicket(QString)), this, SLOT(loadFoundedFile(QString)));
   findFiles->show();


}

//**************************************************************************************************
//
//**************************************************************************************************

bool edytornc::findNext()
{
   bool hasMdiChild = (activeMdiChild() != 0);
   bool found = false;
   QTextCursor cursor, cursorOld;
   QPalette palette;

   if(!findEdit->text().isEmpty() && hasMdiChild)
   {
      activeMdiChild()->highlightFindText(findEdit->text(),
                                          ((mCheckFindWholeWords->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags(0)) |
                                          (!mCheckIgnoreCase->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags(0))));
      found = activeMdiChild()->textEdit->find(findEdit->text(),
                                ((mCheckFindWholeWords->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags(0)) |
                                (!mCheckIgnoreCase->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags(0))));
      if(!found)
      {
         cursor = activeMdiChild()->textEdit->textCursor();
         cursorOld = cursor;
         cursor.movePosition(QTextCursor::Start);
         activeMdiChild()->textEdit->setTextCursor(cursor);
         found = activeMdiChild()->textEdit->find(findEdit->text(),
                                   ((mCheckFindWholeWords->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags(0)) |
                                   (!mCheckIgnoreCase->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags(0))));
         if(!found)
           activeMdiChild()->textEdit->setTextCursor(cursorOld);
      };


      palette.setColor(QPalette::Base, QColor(Qt::red).lighter(160));

      if(found)
        findEdit->setPalette(QPalette());
      else
        findEdit->setPalette(palette);
      
      return found;
   };
   return false;
}

//**************************************************************************************************
//
//**************************************************************************************************

bool edytornc::findPrevious()
{
   bool hasMdiChild = (activeMdiChild() != 0);
   bool found = false;
   QTextCursor cursor, cursorOld;
   QPalette palette;

   if(!findEdit->text().isEmpty() && hasMdiChild)
   {
      activeMdiChild()->highlightFindText(findEdit->text(),
                                          ((mCheckFindWholeWords->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags(0)) |
                                          (!mCheckIgnoreCase->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags(0))));
      found = activeMdiChild()->textEdit->find(findEdit->text(), QTextDocument::FindBackward |
                               ((mCheckFindWholeWords->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags(0)) |
                               (!mCheckIgnoreCase->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags(0))));
      if(!found)
      {
         cursor = activeMdiChild()->textEdit->textCursor();
         cursorOld = cursor;
         cursor.movePosition(QTextCursor::End);
         activeMdiChild()->textEdit->setTextCursor(cursor);
         found = activeMdiChild()->textEdit->find(findEdit->text(), QTextDocument::FindBackward |
                                   ((mCheckFindWholeWords->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags(0)) |
                                   (!mCheckIgnoreCase->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags(0))));
         if(!found)
           activeMdiChild()->textEdit->setTextCursor(cursorOld);
      };

      palette.setColor(QPalette::Base, QColor(Qt::red).lighter(160));

      if(found)
        findEdit->setPalette(QPalette());
      else
        findEdit->setPalette(palette);

      return found;
   };
   return false;
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::replaceNext()
{
   bool hasMdiChildNotReadOnly = ((activeMdiChild() != 0) && !activeMdiChild()->textEdit->isReadOnly());
   bool found = false;

   if(hasMdiChildNotReadOnly) //!replaceEdit->text().isEmpty() &&
   {
      if(activeMdiChild()->textEdit->textCursor().selectedText() == findEdit->text())
        found = true;
      else
        found = findNext();

      if(found)
      {
         QTextCursor cr = activeMdiChild()->textEdit->textCursor();
         cr.beginEditBlock();
         if(defaultMdiWindowProperites.underlineChanges)
         {
            QTextCharFormat format = cr.charFormat();
            format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            format.setUnderlineColor(QColor(defaultMdiWindowProperites.underlineColor));
            cr.setCharFormat(format);
         };
         cr.insertText(replaceEdit->text());
         cr.endEditBlock();
         activeMdiChild()->textEdit->setTextCursor(cr);
         findNext();
      };

   };

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::replacePrevious()
{
   bool hasMdiChildNotReadOnly = ((activeMdiChild() != 0) && !activeMdiChild()->textEdit->isReadOnly());
   bool found = false;

   if(hasMdiChildNotReadOnly) //!replaceEdit->text().isEmpty() &&
   {
      if(activeMdiChild()->textEdit->textCursor().selectedText() == findEdit->text())
        found = true;
      else
        found = findPrevious();

      if(found)
       {
         QTextCursor cr = activeMdiChild()->textEdit->textCursor();
         cr.beginEditBlock();
         if(defaultMdiWindowProperites.underlineChanges)
         {
            QTextCharFormat format = cr.charFormat();
            format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            format.setUnderlineColor(QColor(defaultMdiWindowProperites.underlineColor));
            cr.setCharFormat(format);
         };
         cr.insertText(replaceEdit->text());
         cr.beginEditBlock();
         activeMdiChild()->textEdit->setTextCursor(cr);
         findPrevious();
      };

   };

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::replaceAll()
{
   bool hasMdiChildNotReadOnly = ((activeMdiChild() != 0) && !activeMdiChild()->textEdit->isReadOnly());
   bool found = false;
   QTextCursor startCursor, cr;
   QTextCharFormat format;

   if(hasMdiChildNotReadOnly) //!replaceEdit->text().isEmpty() &&
   {
      QApplication::setOverrideCursor(Qt::BusyCursor);
      if(activeMdiChild()->textEdit->textCursor().selectedText() == findEdit->text())
        found = true;
      else
        found = findNext();

      startCursor = activeMdiChild()->textEdit->textCursor();

      while(found)
      {
         cr = activeMdiChild()->textEdit->textCursor();
         cr.beginEditBlock();
         if(defaultMdiWindowProperites.underlineChanges)
         {
            format = cr.charFormat();
            format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            format.setUnderlineColor(QColor(defaultMdiWindowProperites.underlineColor));
            cr.setCharFormat(format);
         };
         cr.insertText(replaceEdit->text());
         cr.endEditBlock();
         activeMdiChild()->textEdit->setTextCursor(cr);

         found = findNext();
         if(startCursor.position() == activeMdiChild()->textEdit->textCursor().position())
           break;
         qApp->processEvents();
      };
      QApplication::restoreOverrideCursor();
   };

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::selAll()
{
   if(activeMdiChild())
      activeMdiChild()->textEdit->selectAll();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::config()
{
   _editor_properites opt;
   MdiChild *mdiChild;

   defaultMdiWindowProperites.tabbedMode = tabbedView;
   SetupDialog *setUpDialog = new SetupDialog(this, &defaultMdiWindowProperites);


   if(setUpDialog->exec() == QDialog::Accepted)
   {
      defaultMdiWindowProperites = setUpDialog->getSettings();

      foreach(QMdiSubWindow *window, mdiArea->subWindowList(QMdiArea::StackingOrder))
      {
         mdiChild = qobject_cast<MdiChild *>(window->widget());
         opt = mdiChild->getMdiWindowProperites();

         opt.fontName = defaultMdiWindowProperites.fontName;
         opt.fontSize = defaultMdiWindowProperites.fontSize;
         opt.syntaxH = defaultMdiWindowProperites.syntaxH;
         opt.hColors = defaultMdiWindowProperites.hColors;
         opt.intCapsLock = defaultMdiWindowProperites.intCapsLock;
         opt.lineColor = defaultMdiWindowProperites.lineColor;
         opt.underlineColor = defaultMdiWindowProperites.underlineColor;
         opt.underlineChanges = defaultMdiWindowProperites.underlineChanges;
         tabbedView = defaultMdiWindowProperites.tabbedMode;
         if(tabbedView)
           mdiArea->setViewMode(QMdiArea::TabbedView);
         else
           mdiArea->setViewMode(QMdiArea::SubWindowView);

         mdiChild->setMdiWindowProperites(opt);
      };

   };
   delete setUpDialog;
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::readOnly()
{
    if(activeMdiChild())
      activeMdiChild()->textEdit->setReadOnly(readOnlyAct->isChecked());
    updateMenus();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doBhc()
{
   BHCDialog *bhcDialog;
   bhcDialog = findChild<BHCDialog *>();
   if(!bhcDialog)
   {
      BHCDialog *bhcDialog = new BHCDialog(this);
      bhcDialog->show();
      bhcDialog->move((geometry().x() + width() - 10) - bhcDialog->width(), geometry().y()+35);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doInsertSpaces()
{
   if(activeMdiChild())
      activeMdiChild()->doInsertSpace();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doRemoveSpaces()
{
   if(activeMdiChild())
      activeMdiChild()->doRemoveSpace();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doRemoveEmptyLines()
{
   if(activeMdiChild())
      activeMdiChild()->doRemoveEmptyLines();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doInsertDot()
{
   MdiChild *child;


   DotDialog *dotDialog = new DotDialog(this);
   dotDialog->setState(defaultMdiWindowProperites.dotAdr, defaultMdiWindowProperites.atEnd, defaultMdiWindowProperites.dotAfter, defaultMdiWindowProperites.dotAftrerCount);

   child = activeMdiChild();

   if(dotDialog->exec() == QDialog::Accepted)
   {
      if(child)
      {
         defaultMdiWindowProperites = child->getMdiWindowProperites();
         dotDialog->getState(defaultMdiWindowProperites.dotAdr, defaultMdiWindowProperites.atEnd, defaultMdiWindowProperites.dotAfter, defaultMdiWindowProperites.dotAftrerCount);
         child->setMdiWindowProperites(defaultMdiWindowProperites);
         child->doInsertDot();
      };
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doRenumber()
{

   int startAt, inc, to, from, prec, mode;
   bool renumEmpty, renumComm;
   MdiChild *child;
   
   child = activeMdiChild();
   
   RenumberDialog *renumberDialog = new RenumberDialog(this);
   //renumberDialog->setState(editorOpt.dotAdr, editorOpt.atEnd, editorOpt.dotAfter, editorOpt.dotAftrerCount);

   if(renumberDialog->exec() == QDialog::Accepted)
   {
      renumberDialog->getState(mode, startAt, from, prec, inc, to, renumEmpty, renumComm);
      if(child)
      {
         inc = child->doRenumber(mode, startAt, from, prec, inc, to, renumEmpty, renumComm);
         if(mode == 3)
           statusBar()->showMessage(QString(tr("Removed : %1 line numbers.")).arg(inc), 9000);
         else
           statusBar()->showMessage(QString(tr("Renumbered : %1 lines.")).arg(inc), 9000);
      };

   };

   delete(renumberDialog);

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doSpeedFeed()
{
   FeedsDialog *feedsDialog;
   feedsDialog = findChild<FeedsDialog *>();
   if(!feedsDialog)
   {
      FeedsDialog *feedsDialog = new FeedsDialog(this);
      feedsDialog->move((geometry().x() + width() - 10) - feedsDialog->width(), geometry().y()+35);
      feedsDialog->show();
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doChamfer()
{
   ChamferDialog *chamferDialog = new ChamferDialog(this);
   chamferDialog->move((geometry().x() + width()) - chamferDialog->width(), geometry().y()+35);
   chamferDialog->show();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doTriangles()
{
   TriangleDialog *triangleDialog;
   triangleDialog = findChild<TriangleDialog *>();
   if(!triangleDialog)
   {
      TriangleDialog *triangleDialog = new TriangleDialog(this);
      triangleDialog->move((geometry().x() + width() - 10) - triangleDialog->width(), geometry().y()+35);
      triangleDialog->show();
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doConvert()
{
   I2MDialog *i2MDialog;
   i2MDialog = findChild<I2MDialog *>();
   if(!i2MDialog)
   {
      I2MDialog *i2MDialog = new I2MDialog(this);
      i2MDialog->move((geometry().x() + width() - 10) - i2MDialog->width(), geometry().y()+35);
      i2MDialog->show();
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doConvertProg()
{
   MdiChild *child;

   I2MProgDialog *i2mProgDialog = new I2MProgDialog(this);
   i2mProgDialog->setState(defaultMdiWindowProperites.i2mAdr, defaultMdiWindowProperites.i2mprec, defaultMdiWindowProperites.inch);

   child = activeMdiChild();

   if(i2mProgDialog->exec() == QDialog::Accepted)
   {
      if(child)
      {
         defaultMdiWindowProperites = child->getMdiWindowProperites();
         i2mProgDialog->getState(defaultMdiWindowProperites.i2mAdr, defaultMdiWindowProperites.i2mprec, defaultMdiWindowProperites.inch);
         child->setMdiWindowProperites(defaultMdiWindowProperites);
         child->doI2M();
      };
   };


   delete(i2mProgDialog);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doCalc()
{
   proc = findChild<QProcess *>();
   QString name = "";

#ifdef Q_OS_LINUX
   name = "kcalc";
#endif

#ifdef Q_OS_WIN32
   name = "calc.exe";
#endif

   if(!proc && !name.isNull())
   {
      proc = new QProcess(this);
      proc->start(name);
   }
   else
     if(!proc->pid() && !name.isNull())
       proc->start(name);

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::deleteText()
{
    if(activeMdiChild())
      activeMdiChild()->textEdit->textCursor().removeSelectedText();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::paste()
{
    if(activeMdiChild())
      activeMdiChild()->textEdit->paste();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::undo()
{
   if(activeMdiChild())
   {
      activeMdiChild()->doUndo();
      //activeMdiChild()->textEdit->undo();
      //activeMdiChild()->textEdit->ensureCursorVisible();
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::redo()
{
   if(activeMdiChild())
   {
      activeMdiChild()->doRedo();
      //activeMdiChild()->textEdit->redo();
      //activeMdiChild()->textEdit->ensureCursorVisible();
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::activeWindowChanged(QMdiSubWindow *window)
{
   Q_UNUSED(window);
   MdiChild *mdiChild;

   if(mdiArea->subWindowList().count() <= 1)
     defaultMdiWindowProperites.maximized = TRUE;

   mdiChild = activeMdiChild();
   if(mdiChild)
   {
      defaultMdiWindowProperites.maximized = mdiChild->parentWidget()->isMaximized();
      statusBar()->showMessage(mdiChild->currentFile(), 0);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::about()
{
   QMessageBox::about(this, tr("About EdytorNC"),
                            tr("The <b>EdytorNC</b> is text editor for CNC programmers."
                               "<P>Version: "
                               "2009.09"
                               "<P>Copyright (C) 1998 - 2009 by <a href=\"mailto:artkoz@poczta.onet.pl\">Artur Koziol</a>"
                               "<P><a href=\"http://sourceforge.net/projects/edytornc/\">http://sourceforge.net/projects/edytornc</a>"
                               "<P>"
                               "<P>Cross platform installer made by <a href=\"http://installbuilder.bitrock.com/\">BitRock InstallBuilder for Qt</a>"
                               "<P>"
                               "<P><i>EdytorNC is free software; you can redistribute it and/or modify"
                               "it under the terms of the GNU General Public License.</i>"));
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::updateMenus()
{
   bool hasMdiChild = (activeMdiChild() != 0);
   bool hasMdiChildNotReadOnly = (hasMdiChild && !activeMdiChild()->textEdit->isReadOnly());
   bool hasSelection = (hasMdiChild && activeMdiChild()->textEdit->textCursor().hasSelection());

   saveAct->setEnabled(hasMdiChild);
   saveAsAct->setEnabled(hasMdiChild);
   pasteAct->setEnabled(hasMdiChild);
   closeAct->setEnabled(hasMdiChild);
   closeAllAct->setEnabled(hasMdiChild);
   tileAct->setEnabled(hasMdiChild);
   cascadeAct->setEnabled(hasMdiChild);
   nextAct->setEnabled(hasMdiChild);
   previousAct->setEnabled(hasMdiChild);
   separatorAct->setVisible(hasMdiChild);
   selAllAct->setEnabled(hasMdiChildNotReadOnly);
   findAct->setEnabled(hasMdiChild);

   replaceAct->setEnabled(hasMdiChildNotReadOnly);
   readOnlyAct->setEnabled(hasMdiChild);
   renumberAct->setEnabled(hasMdiChildNotReadOnly);
   insertDotAct->setEnabled(hasMdiChildNotReadOnly);
   insertSpcAct->setEnabled(hasMdiChildNotReadOnly);
   removeSpcAct->setEnabled(hasMdiChildNotReadOnly);
   removeEmptyLinesAct->setEnabled(hasMdiChildNotReadOnly);
   convertProgAct->setEnabled(hasMdiChildNotReadOnly);
   cmpMacroAct->setEnabled(hasMdiChildNotReadOnly);

   redoAct->setEnabled(hasMdiChild && activeMdiChild()->textEdit->document()->isRedoAvailable());
   undoAct->setEnabled(hasMdiChild && activeMdiChild()->textEdit->document()->isUndoAvailable());


   if(!hasMdiChildNotReadOnly)
   {
      readOnlyAct->setChecked(TRUE);
      readOnlyAct->setIcon(QIcon(":/images/lock.png"));
   }
   else
   {
      readOnlyAct->setChecked(FALSE);
      readOnlyAct->setIcon(QIcon(":/images/unlock.png"));
   };


   cutAct->setEnabled(hasSelection && hasMdiChildNotReadOnly);
   deleteAct->setEnabled(hasSelection && hasMdiChildNotReadOnly);
   copyAct->setEnabled(hasSelection);

 
   pasteAct->setEnabled((!clipboard->text().isEmpty()) && hasMdiChildNotReadOnly);

   updateStatusBar();
    
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::cancelUnderline()
{
   bool hasMdiChild = (activeMdiChild() != 0);

   if(hasMdiChild)
   {
      QTextCharFormat format = activeMdiChild()->textEdit->currentCharFormat();

      if((format.underlineStyle() != QTextCharFormat::NoUnderline) &&
         !activeMdiChild()->textEdit->textCursor().hasSelection())
      {
         format.setUnderlineStyle(QTextCharFormat::NoUnderline);
         activeMdiChild()->textEdit->setCurrentCharFormat(format);
      };

      if(findToolBar != NULL)
        activeMdiChild()->highlightFindText(findEdit->text()); //findEdit->text()
      else
        activeMdiChild()->highlightFindText("");
   };

   updateStatusBar();

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::updateStatusBar()
{
   QTextBlock b, cb;
   int column = 1;
   int line = 1;

   bool hasMdiChild = (activeMdiChild() != 0);
   bool hasMdiChildNotReadOnly = (hasMdiChild && !activeMdiChild()->textEdit->isReadOnly());

   if(hasMdiChild)
   {
      b = activeMdiChild()->textEdit->textCursor().block();
      column = activeMdiChild()->textEdit->textCursor().position() - b.position();
      cb = activeMdiChild()->textEdit->textCursor().block();
      for(b = activeMdiChild()->textEdit->document()->begin(); b != activeMdiChild()->textEdit->document()->end(); b = b.next())
      {
         if(b == cb)
         {
            break;
         };
         line++;
      };

      labelStat1->setText(tr(" Col: ") + QString::number(column + 1) +
                          tr("  Line: ") + QString::number(line) +
                          (activeMdiChild()->textEdit->document()->isModified() ? tr("  <b>Modified</b>  "): " ") +
                          (!hasMdiChildNotReadOnly ? tr(" Read only  "): " ") +
                          (activeMdiChild()->textEdit->overwriteMode() ? tr(" Overwrite  "): tr(" Insert ")));

   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) 
    {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->currentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->currentFile());
        }
        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    };
}

//**************************************************************************************************
//
//**************************************************************************************************

MdiChild *edytornc::createMdiChild()
{
    MdiChild *child = new MdiChild();
    mdiArea->addSubWindow(child);

    connect(child->textEdit, SIGNAL(copyAvailable(bool)), this, SLOT(updateMenus()));
    connect(child->textEdit, SIGNAL(redoAvailable(bool)), redoAct, SLOT(setEnabled(bool)));
    connect(child->textEdit, SIGNAL(undoAvailable(bool)), undoAct, SLOT(setEnabled(bool)));
    connect(child->textEdit, SIGNAL(textChanged()), this, SLOT(updateMenus()));
    connect(child->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cancelUnderline()));
    connect(child->textEdit, SIGNAL(modificationChanged(bool)), this, SLOT(updateStatusBar()));
    connect(child, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));

    //connect(child->textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    //connect(child->textEdit, SIGNAL(selectionChanged()), this, SLOT(updateMenus()));

    return child;
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::createActions()
{
    newAct = new QAction(QIcon(":/images/filenew.png"), tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    openWPvAct = new QAction(QIcon(":/images/fileopen_preview.png"), tr("&Open file with preview"), this);
    openWPvAct->setStatusTip(tr("Open an existing file (Openfile dialog with preview)"));
    connect(openWPvAct, SIGNAL(triggered()), this, SLOT(openWithPreview()));

    saveAct = new QAction(QIcon(":/images/filesave.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/filesaveas.png"), tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(QIcon(":/images/exit.png"), tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    
    findFilesAct = new QAction(QIcon(":/images/filefind.png"), tr("&Find files..."), this);
    //openAct->setShortcut(tr("Ctrl+"));
    findFilesAct->setStatusTip(tr("Find files"));
    connect(findFilesAct, SIGNAL(triggered()), this, SLOT(findInFl()));

    printAct = new QAction(QIcon(":/images/document-print.png"), tr("&Print"), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setStatusTip(tr("Print file"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(printFile()));

  

    
    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcut(tr("Ctrl+Z"));
    undoAct->setStatusTip(tr("Undo last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));
    undoAct->setEnabled(FALSE);

    redoAct = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), this);
    redoAct->setShortcut(tr("Ctrl+Shift+Z"));
    redoAct->setStatusTip(tr("Redo last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));
    redoAct->setEnabled(FALSE);

    cutAct = new QAction(QIcon(":/images/editcut.png"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/editcopy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/editpaste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));


    findAct = new QAction(QIcon(":/images/find.png"), tr("&Find"), this);
    findAct->setShortcut(tr("Ctrl+F"));
    findAct->setStatusTip(tr("Find text"));
    connect(findAct, SIGNAL(triggered()), this, SLOT(createFindToolBar()));

    replaceAct = new QAction(QIcon(":/images/replace.png"), tr("&Replace"), this);
    replaceAct->setShortcut(tr("Ctrl+R"));
    replaceAct->setStatusTip(tr("Find and replace text"));
    connect(replaceAct, SIGNAL(triggered()), this, SLOT(createFindToolBar()));


    deleteAct = new QAction(QIcon(":/images/editdelete.png"), tr("&Delete"), this);
    //deleteAct->setShortcut(tr("Del"));
    deleteAct->setStatusTip(tr("Removes selected text"));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteText()));
    
    selAllAct = new QAction(QIcon(":/images/edit-select-all.png"), tr("&Select all"), this);
    selAllAct->setShortcut(tr("Ctrl+A"));
    selAllAct->setStatusTip(tr("Select all text"));
    connect(selAllAct, SIGNAL(triggered()), this, SLOT(selAll()));

    readOnlyAct = new QAction(QIcon(":/images/unlock.png"), tr("Read &only"), this);
    readOnlyAct->setShortcut(tr("F12"));
    readOnlyAct->setCheckable(TRUE);
    readOnlyAct->setStatusTip(tr("Makes text read only"));
    connect(readOnlyAct, SIGNAL(triggered()), this, SLOT(readOnly()));

    configAct = new QAction(QIcon(":/images/configure.png"), tr("Configuration"), this);
    //configAct->setShortcut(tr("Ctrl+R"));
    configAct->setStatusTip(tr("Open configuration dialog"));
    connect(configAct, SIGNAL(triggered()), this, SLOT(config()));





    bhcAct = new QAction(QIcon(":/images/bhc.png"), tr("&Bolt hole circle"), this);
    bhcAct->setShortcut(tr("F8"));
    bhcAct->setStatusTip(tr("Calculate bolt hole's positions"));
    connect(bhcAct, SIGNAL(triggered()), this, SLOT(doBhc()));
    
    insertSpcAct = new QAction(QIcon(":/images/insertspc.png"), tr("&Insert spaces"), this);
    insertSpcAct->setShortcut(tr("F4"));
    insertSpcAct->setStatusTip(tr("Inserts spaces"));
    connect(insertSpcAct, SIGNAL(triggered()), this, SLOT(doInsertSpaces()));

    removeSpcAct = new QAction(QIcon(":/images/removespc.png"), tr("Remove spaces"), this);
    removeSpcAct->setShortcut(tr("F5"));
    removeSpcAct->setStatusTip(tr("Removes spaces"));
    connect(removeSpcAct, SIGNAL(triggered()), this, SLOT(doRemoveSpaces()));

    removeEmptyLinesAct = new QAction(QIcon(":/images/removeemptylines.png"), tr("Remove empty lines"), this);
    //removeEmptyLinesAct->setShortcut(tr("F5"));
    removeEmptyLinesAct->setStatusTip(tr("Removes empty lines"));
    connect(removeEmptyLinesAct, SIGNAL(triggered()), this, SLOT(doRemoveEmptyLines()));

    insertDotAct = new QAction(QIcon(":/images/dots.png"), tr("Insert dots"), this);
    insertDotAct->setShortcut(tr("F6"));
    insertDotAct->setStatusTip(tr("Inserts decimal dot"));
    connect(insertDotAct, SIGNAL(triggered()), this, SLOT(doInsertDot()));

    renumberAct = new QAction(QIcon(":/images/renumber.png"), tr("Renumber"), this);
    renumberAct->setShortcut(tr("F7"));
    renumberAct->setStatusTip(tr("Renumber program blocks"));
    connect(renumberAct, SIGNAL(triggered()), this, SLOT(doRenumber()));

    speedFeedAct = new QAction(QIcon(":/images/vcf.png"), tr("Feed's speed's"), this);
    speedFeedAct->setShortcut(tr("F9"));
    speedFeedAct->setStatusTip(tr("Calculate speed, feed, cutting speed"));
    connect(speedFeedAct, SIGNAL(triggered()), this, SLOT(doSpeedFeed()));    

    chamferAct = new QAction(QIcon(":/images/chamfer.png"), tr("Chamfer"), this);
    //chamferAct->setShortcut(tr("F9"));
    chamferAct->setStatusTip(tr("Calculate chamfer"));
    connect(chamferAct, SIGNAL(triggered()), this, SLOT(doChamfer()));

    trianglesAct = new QAction(QIcon(":/images/triangles.png"), tr("Solution of triangles"), this);
    //trianglesAct->setShortcut(tr("F9"));
    trianglesAct->setStatusTip(tr("Solution of triangles"));
    connect(trianglesAct, SIGNAL(triggered()), this, SLOT(doTriangles()));

    convertAct = new QAction(QIcon(":/images/i2m.png"), tr("Convert inch <-> mm"), this);
    //convertAct->setShortcut(tr("F9"));
    convertAct->setStatusTip(tr("Convert inch <-> mm"));
    connect(convertAct, SIGNAL(triggered()), this, SLOT(doConvert()));

    convertProgAct = new QAction(QIcon(":/images/i2mprog.png"), tr("Convert program inch <-> mm"), this);
    //convertProgAct->setShortcut(tr("F9"));
    convertProgAct->setStatusTip(tr("Convert program inch <-> mm"));
    connect(convertProgAct, SIGNAL(triggered()), this, SLOT(doConvertProg()));

    calcAct = new QAction(QIcon(":/images/calc.png"), tr("Calculator"), this);
    //calcAct->setShortcut(tr("F9"));
    calcAct->setStatusTip(tr("Run calculator"));
    connect(calcAct, SIGNAL(triggered()), this, SLOT(doCalc()));

    showSerialToolBarAct = new QAction(QIcon(":/images/serial.png"), tr("Serial port send/receive - experimental"), this);
    //showSerialToolBarAct->setShortcut(tr("F9"));
    showSerialToolBarAct->setCheckable(TRUE);
    showSerialToolBarAct->setStatusTip(tr("Serial port send/receive"));
    connect(showSerialToolBarAct, SIGNAL(triggered()), this, SLOT(createSerialToolBar()));

    cmpMacroAct = new QAction(QIcon(":/images/compfile.png"), tr("Compile macro - experimental"), this);
    //cmpMacroAct->setShortcut(tr("F9"));
    cmpMacroAct->setStatusTip(tr("Translate EdytorNC macro into CNC program"));
    connect(cmpMacroAct, SIGNAL(triggered()), this, SLOT(doCmpMacro()));




    closeAct = new QAction(QIcon(":/images/fileclose.png"), tr("Cl&ose"), this);
    closeAct->setShortcut(tr("Ctrl+F4"));
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(QIcon(":/images/window-close.png"), tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(QIcon(":/images/tile_h.png"), tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(QIcon(":/images/cascade.png"), tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(QIcon(":/images/go-next.png"), tr("Ne&xt"), this);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(QIcon(":/images/go-previous.png"), tr("Pre&vious"), this);
    previousAct->setStatusTip(tr("Move the focus to the previous window"));
    connect(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(activeWindowChanged(QMdiSubWindow *)));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);



    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(openWPvAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(findFilesAct);
    fileMenu->addSeparator();
    recentFileMenu = fileMenu->addMenu(tr("&Recent files"));
    recentFileMenu->setIcon(QIcon(":/images/document-open-recent.png"));
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(closeAllAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(deleteAct);
    editMenu->addSeparator();
    editMenu->addAction(selAllAct);
    editMenu->addSeparator();
    editMenu->addAction(findAct);
    editMenu->addAction(replaceAct);
    editMenu->addSeparator();
    editMenu->addAction(readOnlyAct);
    editMenu->addSeparator();
    editMenu->addAction(configAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(showSerialToolBarAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(insertSpcAct);
    toolsMenu->addAction(removeSpcAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(removeEmptyLinesAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(insertDotAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(renumberAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(bhcAct);
    toolsMenu->addAction(speedFeedAct);
    toolsMenu->addAction(trianglesAct);
    toolsMenu->addAction(chamferAct);
    toolsMenu->addAction(convertAct);
    toolsMenu->addAction(convertProgAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(cmpMacroAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(calcAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName("File");
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    //fileToolBar->addAction(openWPvAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(findFilesAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("Edit");
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addSeparator();
    editToolBar->addAction(deleteAct);
    editToolBar->addSeparator();
    editToolBar->addAction(selAllAct);
    editToolBar->addSeparator();
    editToolBar->addAction(findAct);
    editToolBar->addAction(replaceAct);


    toolsToolBar = new QToolBar(tr("Tools"));
    addToolBar(Qt::LeftToolBarArea, toolsToolBar);
    toolsToolBar->setObjectName("Tools");
    toolsToolBar->addAction(showSerialToolBarAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(insertSpcAct);
    toolsToolBar->addAction(removeSpcAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(removeEmptyLinesAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(insertDotAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(renumberAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(bhcAct);
    toolsToolBar->addAction(speedFeedAct);
    toolsToolBar->addAction(trianglesAct);
    toolsToolBar->addAction(chamferAct);
    toolsToolBar->addAction(convertAct);
    toolsToolBar->addAction(convertProgAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(cmpMacroAct);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(calcAct);


    windowToolBar = addToolBar(tr("Window"));
    windowToolBar->setObjectName("Window");
    windowToolBar->addAction(previousAct);
    windowToolBar->addAction(nextAct);

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::createStatusBar()
{

   labelStat1 = new QLabel("    ");
   
   labelStat1->setFrameShadow(QFrame::Sunken);
   labelStat1->setFrameShape(QFrame::Box);

   readOnlyButton = new QToolButton();
   readOnlyButton->setDefaultAction(readOnlyAct);

   statusBar()->addPermanentWidget((labelStat1));
   statusBar()->addPermanentWidget(readOnlyButton);
   statusBar()->setSizeGripEnabled(TRUE);


   statusBar()->showMessage(tr("Ready"));
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::readSettings()
{
    QSettings settings("EdytorNC", "EdytorNC");

    QPoint pos = settings.value("Pos", QPoint(0, 0)).toPoint();
    QSize size = settings.value("Size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);


    restoreState(settings.value("State", QByteArray()).toByteArray());


    lastDir = settings.value("LastDir", QString(getenv("HOME"))).toString();

    openFileFilter = settings.value("FileOpenFilter", "*.nc").toString();

    defaultMdiWindowProperites.dotAdr = settings.value("DotAddress", "XYZB").toString();
    defaultMdiWindowProperites.dotAftrerCount = settings.value("DotAfterCount", 1000).toInt();
    defaultMdiWindowProperites.atEnd = settings.value("DotAtEnd", TRUE ).toBool();
    defaultMdiWindowProperites.dotAfter = settings.value("DotAfter", FALSE).toBool();

    defaultMdiWindowProperites.i2mAdr = settings.value("I2MAddress", "XYZB").toString();
    defaultMdiWindowProperites.i2mprec = settings.value("I2MPrec", 3).toInt();
    defaultMdiWindowProperites.inch = settings.value("I2M", TRUE).toBool();

    defaultMdiWindowProperites.fontName = settings.value("FontName", "Courier").toString();
    defaultMdiWindowProperites.fontSize = settings.value("FontSize", 12).toInt();
    defaultMdiWindowProperites.intCapsLock = settings.value("IntCapsLock", TRUE).toBool();
    defaultMdiWindowProperites.underlineChanges = settings.value("UnderlineChanges", TRUE).toBool();
    tabbedView = settings.value("TabbedView", FALSE).toBool();

    defaultMdiWindowProperites.lineColor = settings.value("LineColor", 0xFEFFB6).toInt();
    defaultMdiWindowProperites.underlineColor = settings.value("UnderlineColor", 0x00FF00).toInt();

    fileDialogState = settings.value("FileDialogState", QByteArray()).toByteArray(); 

    m_recentFiles = settings.value( "RecentFiles").toStringList();
    updateRecentFilesMenu();

    defaultMdiWindowProperites.maximized = settings.value("MaximizedMdi", TRUE).toBool();


    settings.beginGroup("Highlight" );
    defaultMdiWindowProperites.syntaxH = settings.value("HighlightOn", TRUE).toBool();

    defaultMdiWindowProperites.hColors.commentColor = settings.value("CommentColor", 0xde0020).toInt();
    defaultMdiWindowProperites.hColors.gColor = settings.value("GColor", 0x1600ee).toInt();
    defaultMdiWindowProperites.hColors.mColor = settings.value("MColor", 0x80007d).toInt();
    defaultMdiWindowProperites.hColors.nColor = settings.value("NColor", 0x808080).toInt();
    defaultMdiWindowProperites.hColors.lColor = settings.value("LColor", 0x535b5f).toInt();
    defaultMdiWindowProperites.hColors.fsColor = settings.value("FsColor", 0x516600).toInt();
    defaultMdiWindowProperites.hColors.dhtColor = settings.value("DhtColor", 0x660033).toInt();
    defaultMdiWindowProperites.hColors.rColor = settings.value("RColor", 0x24576f).toInt();
    defaultMdiWindowProperites.hColors.macroColor = settings.value("MacroColor", 0x000080).toInt();
    defaultMdiWindowProperites.hColors.keyWordColor = settings.value("KeyWordColor", 0x1d8000).toInt();
    defaultMdiWindowProperites.hColors.progNameColor = settings.value("ProgNameColor", 0x000000).toInt();
    defaultMdiWindowProperites.hColors.operatorColor = settings.value("OperatorColor", 0x9a2200).toInt();
    defaultMdiWindowProperites.hColors.zColor = settings.value("ZColor", 0x000080).toInt();
    defaultMdiWindowProperites.hColors.aColor = settings.value("AColor", 0x000000).toInt();
    defaultMdiWindowProperites.hColors.bColor = settings.value("BColor", 0x000000).toInt();
    settings.endGroup();

    settings.beginGroup("LastDoc" );

    int max = settings.value("OpenedFileCount", 0 ).toInt();
    for(int i = 1; i < max; ++i) 
    {

       defaultMdiWindowProperites.lastDir = lastDir.absolutePath();

       defaultMdiWindowProperites.fileName = settings.value("OpenedFile_" + QString::number(i) ).toString();
       if(!defaultMdiWindowProperites.fileName.isEmpty())
       {
          defaultMdiWindowProperites.cursorPos = settings.value("Cursor_" + QString::number(i), 1).toInt();
          defaultMdiWindowProperites.readOnly = settings.value( "ReadOnly_" + QString::number(i), FALSE).toBool();
          defaultMdiWindowProperites.geometry = settings.value("Geometry_" + QString::number(i), QByteArray()).toByteArray();
          loadFile(defaultMdiWindowProperites);
           
       };
        
    };


    settings.endGroup();

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::writeSettings()
{
    int i = 1;
    MdiChild *mdiChild;
    bool maximized = false;
   
    QSettings settings("EdytorNC", "EdytorNC");

    //cleanup old settings
    settings.beginGroup("LastDoc" );
    int max = settings.value("OpenedFileCount", 0 ).toInt();
    for(int i = 1; i < max; ++i)
    {
        settings.remove("OpenedFile_" + QString::number(i));
        settings.remove("Cursor_" + QString::number(i));
        settings.remove("ReadOnly_" + QString::number(i));
        settings.remove("Pos_" + QString::number(i));
        settings.remove("Size_" + QString::number(i));
        settings.remove("Geometry_" + QString::number(i));
    };
    settings.endGroup();


    settings.setValue("Pos", pos());
    settings.setValue("Size", size());


    settings.setValue("State", saveState());


    settings.setValue("LastDir", lastDir.path());
    
    settings.setValue("FileOpenFilter", openFileFilter);

    settings.setValue("DotAddress", defaultMdiWindowProperites.dotAdr);
    settings.setValue("DotAfterCount", defaultMdiWindowProperites.dotAftrerCount);
    settings.setValue("DotAtEnd", defaultMdiWindowProperites.atEnd);
    settings.setValue("DotAfter", defaultMdiWindowProperites.dotAfter);

    settings.setValue("I2MAddress", defaultMdiWindowProperites.i2mAdr);
    settings.setValue("I2MPrec", defaultMdiWindowProperites.i2mprec);
    settings.setValue("I2M", defaultMdiWindowProperites.inch);

    settings.setValue("FontName", defaultMdiWindowProperites.fontName);
    settings.setValue("FontSize", defaultMdiWindowProperites.fontSize);
    settings.setValue("IntCapsLock", defaultMdiWindowProperites.intCapsLock);
    settings.setValue("UnderlineChanges", defaultMdiWindowProperites.underlineChanges);
    settings.setValue("TabbedView", tabbedView);
    settings.setValue("LineColor", defaultMdiWindowProperites.lineColor);
    settings.setValue("UnderlineColor", defaultMdiWindowProperites.underlineColor);

    
    settings.setValue("FileDialogState", fileDialogState);
    settings.setValue( "RecentFiles", m_recentFiles);


    settings.beginGroup("Highlight" );
    settings.setValue("HighlightOn", defaultMdiWindowProperites.syntaxH);

    settings.setValue("CommentColor", defaultMdiWindowProperites.hColors.commentColor);
    settings.setValue("GColor", defaultMdiWindowProperites.hColors.gColor);
    settings.setValue("MColor", defaultMdiWindowProperites.hColors.mColor);
    settings.setValue("NColor", defaultMdiWindowProperites.hColors.nColor);
    settings.setValue("LColor", defaultMdiWindowProperites.hColors.lColor);
    settings.setValue("FsColor", defaultMdiWindowProperites.hColors.fsColor);
    settings.setValue("DhtColor", defaultMdiWindowProperites.hColors.dhtColor);
    settings.setValue("RColor", defaultMdiWindowProperites.hColors.rColor);
    settings.setValue("MacroColor", defaultMdiWindowProperites.hColors.macroColor);
    settings.setValue("KeyWordColor", defaultMdiWindowProperites.hColors.keyWordColor);
    settings.setValue("ProgNameColor", defaultMdiWindowProperites.hColors.progNameColor);
    settings.setValue("OperatorColor", defaultMdiWindowProperites.hColors.operatorColor);
    settings.setValue("BColor", defaultMdiWindowProperites.hColors.bColor);
    settings.setValue("AColor", defaultMdiWindowProperites.hColors.aColor);
    settings.setValue("ZColor", defaultMdiWindowProperites.hColors.zColor);

    settings.endGroup();

    settings.beginGroup("LastDoc");

    foreach (QMdiSubWindow *window, mdiArea->subWindowList(QMdiArea::StackingOrder)) 
    {
        mdiChild = qobject_cast<MdiChild *>(window->widget());
        _editor_properites Opt = mdiChild->getMdiWindowProperites();

        settings.setValue("OpenedFile_" + QString::number(i), Opt.fileName);
        settings.setValue("Cursor_" + QString::number(i), Opt.cursorPos);
        settings.setValue("ReadOnly_" + QString::number(i), Opt.readOnly);
        settings.setValue("Geometry_" + QString::number(i), mdiChild->parentWidget()->saveGeometry());
        if(mdiChild->parentWidget()->isMaximized())
          maximized =  true;

        i++;
    };
    
    settings.setValue("OpenedFileCount", (i));
    settings.endGroup();

    settings.setValue("MaximizedMdi", maximized);


}

//**************************************************************************************************
//
//**************************************************************************************************

MdiChild *edytornc::activeMdiChild()
{
    if(QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
      return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

//**************************************************************************************************
//
//**************************************************************************************************

QMdiSubWindow *edytornc::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) 
    {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::setActiveSubWindow(QWidget *window)
{
    if(!window)
      return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));

}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::loadFile(_editor_properites options)
{
    QFileInfo file;

    QMdiSubWindow *existing = findMdiChild(options.fileName);
    if(existing)
    {
       mdiArea->setActiveSubWindow(existing);
       return;
    };
    file.setFile(options.fileName);
    if((file.exists()) && (file.isReadable()))
    {
       MdiChild *child = createMdiChild();
       child->newFile();
       child->loadFile(options.fileName);
       child->setMdiWindowProperites(options);
       child->parentWidget()->restoreGeometry(options.geometry);
       if(defaultMdiWindowProperites.maximized)
         child->showMaximized();
       else
         child->showNormal();
    };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::updateRecentFiles(const QString &filename)
{
    m_recentFiles.prepend(filename);
    if(m_recentFiles.size() > MAX_RECENTFILES)
        m_recentFiles.removeLast();
    m_recentFiles.removeDuplicates();

    updateRecentFilesMenu();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::fileOpenRecent(QAction *act)
{
    defaultMdiWindowProperites.readOnly = FALSE;
    //defaultMdiWindowProperites.maximized = FALSE;
    defaultMdiWindowProperites.cursorPos = 0;
    defaultMdiWindowProperites.fileName = m_recentFiles[act->data().toInt()];
    loadFile(defaultMdiWindowProperites);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::updateRecentFilesMenu()
{
    QAction *newAct;
  
  
    recentFileMenu->clear();
    for(int i = 0; i < MAX_RECENTFILES; ++i)
    {
        if(i < int(m_recentFiles.size()))
        {
            newAct = recentFileMenu->addAction(QIcon(":/images/document-open-recent.png"), 
                                               QString( "&%1 - %2" ).arg( i + 1 ).arg(m_recentFiles[i]));
            connect(recentFileMenu, SIGNAL(triggered(QAction *)), this, SLOT(fileOpenRecent(QAction *)));
            newAct->setData(i);
        };
    }
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::loadFoundedFile(const QString &fileName)
{
    QFileInfo file;

    QMdiSubWindow *existing = findMdiChild(fileName);
    if(existing)
    {
       mdiArea->setActiveSubWindow(existing);
       return;
    };
    file.setFile(fileName);
    if((file.exists()) && (file.isReadable()))
    {
       MdiChild *child = createMdiChild();
       child->newFile();
       child->loadFile(fileName);
       //defaultMdiWindowProperites.maximized = FALSE;
       defaultMdiWindowProperites.cursorPos = 0;
       defaultMdiWindowProperites.readOnly = FALSE;
       defaultMdiWindowProperites.geometry = QByteArray();
       child->setMdiWindowProperites(defaultMdiWindowProperites);
       if(defaultMdiWindowProperites.maximized)
         child->showMaximized();
       else
         child->showNormal();
    };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::messReceived(const QString &text)
{
    QString str = text;
    QStringList list1 = str.split(";", QString::SkipEmptyParts);
    for(int i = 0; i < list1.size(); ++i)
      openFile(list1.at(i));
    emit needToShow();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::createFindToolBar()
{
   QString selText;
   QTextCursor cursor;

   if(findToolBar == NULL)
   {
      findToolBar = new QToolBar(tr("Find"));
      addToolBar(Qt::BottomToolBarArea, findToolBar);
      findToolBar->setObjectName("Find");

      findToolBar->setAttribute(Qt::WA_DeleteOnClose);

      findNextAct = new QAction(QIcon(":/images/arrow-right.png"), tr("Find next"), this);
      findNextAct->setShortcut(tr("F3"));
      findNextAct->setStatusTip(tr("Find next"));
      connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNext()));

      findPreviousAct = new QAction(QIcon(":/images/arrow-left.png"), tr("Find previous"), this);
      findPreviousAct->setShortcut(tr("Shift+F3"));
      findPreviousAct->setStatusTip(tr("Find previous"));
      connect(findPreviousAct, SIGNAL(triggered()), this, SLOT(findPrevious()));

      replaceNextAct = new QAction(QIcon(":/images/arrow-right.png"), tr("Replace && find next"), this);
      //replaceNextAct->setShortcut(tr("F3"));
      replaceNextAct->setStatusTip(tr("Replace && find next"));
      connect(replaceNextAct, SIGNAL(triggered()), this, SLOT(replaceNext()));

      replacePreviousAct = new QAction(QIcon(":/images/arrow-left.png"), tr("Replace && find previous"), this);
      //replacePreviousAct->setShortcut(tr("F3"));
      replacePreviousAct->setStatusTip(tr("Replace && find previous"));
      connect(replacePreviousAct, SIGNAL(triggered()), this, SLOT(replacePrevious()));

      replaceAllAct = new QAction(QIcon(":/images/arrow-right-double.png"), tr("Replace all"), this);
      //replaceAllAct->setShortcut(tr("F3"));
      replaceAllAct->setStatusTip(tr("Replace all"));
      connect(replaceAllAct, SIGNAL(triggered()), this, SLOT(replaceAll()));

      findCloseAct = new QAction(QIcon(":/images/close_small.png"), tr("Close find toolbar"), this);
      findCloseAct->setStatusTip(tr("Close find toolbar"));
      connect(findCloseAct, SIGNAL(triggered()), this, SLOT(closeFindToolBar()));

      findLabel = new QLabel(tr("Find:"));
      findToolBar->addWidget(findLabel);
      findEdit = new QLineEdit();
      findEdit->installEventFilter(this);
      findToolBar->addWidget(findEdit);
      findToolBar->addAction(findPreviousAct);
      findToolBar->addAction(findNextAct);   
      findToolBar->addSeparator();

      replaceLabel = new QLabel(tr("Replace with:"));
      findToolBar->addWidget(replaceLabel);
      replaceEdit = new QLineEdit();
      replaceEdit->installEventFilter(this);
      findToolBar->addWidget(replaceEdit);
      findToolBar->addAction(replacePreviousAct);
      findToolBar->addAction(replaceNextAct);
      findToolBar->addAction(replaceAllAct);
      findToolBar->addSeparator();

      mCheckIgnoreCase = new QCheckBox(tr("Ignore c&ase"));
      mCheckIgnoreCase->setChecked(TRUE);
      connect(mCheckIgnoreCase, SIGNAL(clicked()), this, SLOT(findTextChanged()));
      findToolBar->addWidget(mCheckIgnoreCase);
      mCheckFindWholeWords = new QCheckBox(tr("&Whole words only"));
      connect(mCheckFindWholeWords, SIGNAL(clicked()), this, SLOT(findTextChanged()));
      findToolBar->addWidget(mCheckFindWholeWords);
      findToolBar->addSeparator();
      findToolBar->addAction(findCloseAct);
   }
   else
     findToolBar->show();

   disconnect(findEdit, SIGNAL(textChanged(QString)), this, SLOT(findTextChanged()));

   if(!activeMdiChild()->textEdit->textCursor().hasSelection())
   {
      cursor = activeMdiChild()->textEdit->textCursor();
      cursor.select(QTextCursor::WordUnderCursor);
      selText = cursor.selectedText();
      if((selText.size() > 32) || (selText.size() < 2))
        cursor.clearSelection();
      activeMdiChild()->textEdit->setTextCursor(cursor);
   };

   cursor = activeMdiChild()->textEdit->textCursor();

   if(cursor.hasSelection())
   {
      selText = cursor.selectedText();

      if((selText.size() < 32))
        findEdit->setText(selText);
      else
      {
         cursor.clearSelection();
         activeMdiChild()->textEdit->setTextCursor(cursor);
      };
   };

   findEdit->setPalette(QPalette());
   connect(findEdit, SIGNAL(textChanged(QString)), this, SLOT(findTextChanged()));
   findEdit->setFocus(Qt::MouseFocusReason);

   activeMdiChild()->highlightFindText(findEdit->text());
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::closeFindToolBar()
{
   if(activeMdiChild())
   {
      activeMdiChild()->setFocus(Qt::MouseFocusReason);
      activeMdiChild()->highlightFindText("");
      activeMdiChild()->textEdit->centerCursor();
   };
   findToolBar->close();
   findToolBar = NULL;
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::findTextChanged()
{
   bool hasMdiChild = (activeMdiChild() != 0);
   QTextCursor cursor;
   int pos;

   if(hasMdiChild)
   {
      //activeMdiChild()->highlightFindText(findEdit->text(),
      //                                   ((mCheckFindWholeWords->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags(0)) |
      //                                   (!mCheckIgnoreCase->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags(0))));
      if(!findEdit->text().isEmpty())
      {
         cursor = activeMdiChild()->textEdit->textCursor();
         pos = cursor.position() - findEdit->text().size();
         if(pos < 0)
           pos = 0;
         do
         {
            cursor.movePosition(QTextCursor::Left);  //cursor.movePosition(QTextCursor::StartOfWord)
         }while((pos <= cursor.position()) && (cursor.position() > 0));

         activeMdiChild()->textEdit->setTextCursor(cursor);

         findNext();
      }
      else
      {
         findEdit->setPalette(QPalette());
         cursor.clearSelection();
         activeMdiChild()->textEdit->setTextCursor(cursor);
      };

   };
}

//**************************************************************************************************
//
//**************************************************************************************************

bool edytornc::eventFilter(QObject *obj, QEvent *ev)
{
   if((obj == findEdit) || (obj == replaceEdit))
   {
       if( ev->type() == QEvent::KeyPress )
       {
          QKeyEvent *k = (QKeyEvent*) ev;

          if(k->key() == Qt::Key_Comma) //Keypad comma should always prints period
          {
             if((k->modifiers() == Qt::KeypadModifier) || (k->nativeScanCode() == 0x53)) // !!! Qt::KeypadModifier - Not working for keypad comma !!!
             {
                QApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier, ".", FALSE, 1));
                return true;
             };

          };

          if(defaultMdiWindowProperites.intCapsLock)
          {
             if(k->text()[0].isLower() && (k->modifiers() == Qt::NoModifier))
             {
                QApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, k->key(), Qt::NoModifier, k->text().toUpper(), FALSE, 1));
                return true;

             };

             if(k->text()[0].isUpper() && (k->modifiers() == Qt::ShiftModifier))
             {
                QApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, k->key(), Qt::ShiftModifier, k->text().toLower(), FALSE, 1));
                return true;
             };
          };
       };

       return FALSE;
   }
   else
   {
      // pass the event on to the parent class
      return eventFilter(obj, ev);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::createSerialToolBar()
{
   if(serialToolBar == NULL)
   {
      serialToolBar = new QToolBar(tr("Serial port toolbar"));
      addToolBar(serialToolBar);
      serialToolBar->setObjectName("SerialToolBar");

      configPortAct = new QAction(QIcon(":/images/serialconfig.png"), tr("Serial port configuration"), this);
      //configPortAct->setShortcut(tr("F3"));
      configPortAct->setStatusTip(tr("Serial port configuration"));
      connect(configPortAct, SIGNAL(triggered()), this, SLOT(serialConfig()));

      receiveAct = new QAction(QIcon(":/images/receive.png"), tr("Receive new file"), this);
      //receiveAct->setShortcut(tr("Shift+F3"));
      receiveAct->setStatusTip(tr("Receive new file"));
      connect(receiveAct, SIGNAL(triggered()), this, SLOT(receiveButtonClicked()));

      sendAct = new QAction(QIcon(":/images/send.png"), tr("Send current file"), this);
      //sendAct->setShortcut(tr("F3"));
      sendAct->setStatusTip(tr("Send current file"));
      connect(sendAct, SIGNAL(triggered()), this, SLOT(sendButtonClicked()));

      serialCloseAct = new QAction(QIcon(":/images/close_small.png"), tr("Close send/receive toolbar"), this);
      serialCloseAct->setStatusTip(tr("Close find toolbar"));
      connect(serialCloseAct, SIGNAL(triggered()), this, SLOT(closeSerialToolbar()));


      configBox = new QComboBox();
      configBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

      serialToolBar->addWidget(configBox);
      serialToolBar->addAction(configPortAct);
      serialToolBar->addSeparator();
      serialToolBar->addAction(receiveAct);
      serialToolBar->addSeparator();
      serialToolBar->addAction(sendAct);
      serialToolBar->addSeparator();
      serialToolBar->addAction(serialCloseAct);
   }
   else
     if(!showSerialToolBarAct->isChecked())
     {
        closeSerialToolbar();
        return;
     }
     else
     {
        serialToolBar->show();
        showSerialToolBarAct->setChecked(TRUE);
     };

   comPort = new QextSerialPort();
   stop = true;

   loadSerialConfignames();
   configBox->adjustSize();
   loadConfig();
   //connect(configBox, SIGNAL(currentIndexChanged(int)), SLOT(loadConfig()));
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::closeSerialToolbar()
{
   stop = true;
   comPort->close();
   delete(comPort);

   serialToolBar->close();
   delete(serialToolBar);
   showSerialToolBarAct->setChecked(FALSE);
   serialToolBar = NULL;
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::serialConfig()
{
   SPConfigDialog *serialConfigDialog = new SPConfigDialog(this);

   serialConfigDialog->show();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::loadSerialConfignames()
{
    int id;
    QStringList list;
    QString item;

    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");


    configBox->clear();
    list = settings.value("SettingsList", QStringList(tr("Default"))).toStringList();
    configBox->addItems(list);
    item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
    id = configBox->findText(item);
    configBox->setCurrentIndex(id);



    settings.endGroup();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::serialConfigTest()
{
   TransmissionDialog *trDialog = new TransmissionDialog(this);

   trDialog->show();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::loadConfig()
{
    QString port, fTx, nTx;
    int pos;
    QRegExp exp;
    char chr;
    bool ok;


    stop = true;
    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");

#ifdef Q_OS_WIN32
       port = "COM1";
#else
       port = "/dev/ttyS0";
#endif

    settings.beginGroup(configBox->currentText());

    portName = settings.value("PortName", port).toString();
    baudRate = settings.value("BaudRate", BAUD9600).toInt();
    dataBits = settings.value("DataBits", DATA_8).toInt();
    stopBits = settings.value("StopBits", STOP_2).toInt();
    parity = settings.value("Parity", PAR_NONE).toInt();
    flowControl = settings.value("FlowControl", FLOW_HARDWARE).toInt();
    lineDelay = settings.value("LineDelay", 0).toDouble();

    sendAtEnd = settings.value("SendAtEnd", "").toString();
    sendAtBegining = settings.value("SendAtBegining", "").toString();

    settings.endGroup();
    settings.endGroup();

    comPort->setPortName(portName);
    comPort->setBaudRate(BaudRateType(baudRate));
    comPort->setDataBits(DataBitsType(dataBits));
    comPort->setFlowControl(FlowType(flowControl));
    comPort->setParity(ParityType(parity));
    comPort->setStopBits(StopBitsType(stopBits));


    exp.setPattern("0x[0-9a-fA-F]{1,2}");
    pos = 0;
    while((pos = sendAtBegining.indexOf(exp, pos)) >= 0)
    {
       fTx = sendAtBegining.mid(pos, exp.matchedLength());
       chr = fTx.toInt(&ok, 16);
       sendAtBegining.replace(pos, exp.matchedLength(), QString(chr));
    };
    sendAtBegining.remove(" ");

    pos = 0;
    while((pos = sendAtEnd.indexOf(exp, pos)) >= 0)
    {
       fTx = sendAtEnd.mid(pos, exp.matchedLength());
       chr = fTx.toInt(&ok, 16);
       sendAtEnd.replace(pos, exp.matchedLength(), QString(chr));
    };
    sendAtEnd.remove(" ");

    //comPort->setTimeout(0,100);
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::lineDelaySlot()
{
   readyCont = true;
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::sendButtonClicked()
{
   int i;
   QString tx;
   QTextCursor cursor, prevCursor;
   MdiChild *activeWindow;

   activeWindow = activeMdiChild();
   if(!(activeWindow != 0))
     return;

   loadConfig();
   if(comPort->open(QIODevice::ReadWrite))
     stop = false;
   else
   {
      stop = true;
      showError(E_INVALID_FD);
      return;
   };
   comPort->flush();
   comPort->reset();

   showError(E_NO_ERROR);
   receiveAct->setEnabled(FALSE);
   sendAct->setEnabled(FALSE);
   QApplication::setOverrideCursor(Qt::BusyCursor);

   cursor = activeWindow->textEdit->textCursor();
   prevCursor = cursor;
   cursor.movePosition(QTextCursor::Start);
   activeWindow->textEdit->setTextCursor(cursor);

   tx = sendAtBegining;
   tx.append(activeWindow->textEdit->toPlainText());
   tx.append(sendAtEnd);
   if(!tx.contains("\r\n"))
      tx.replace("\n", "\r\n");

   QProgressDialog progressDialog(this);
   progressDialog.setRange(0, tx.size());
   progressDialog.setModal(TRUE);
   progressDialog.open();
   qApp->processEvents();

   i = 0;
   while(i < tx.size())
   {
      qApp->processEvents();

      if(progressDialog.wasCanceled())
        break;

      if(stop)
        break;

      progressDialog.setValue(i);
      if(comPort->bytesToWrite() == 0)
      {
         if(!comPort->putChar(tx[i].toAscii()))
         {
            stop = true;
            showError(comPort->lastError());
            break;
         };

         if(tx[i].toAscii() != '\r')
           cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

         activeWindow->textEdit->setTextCursor(cursor);
         progressDialog.setLabelText(tr("Sending byte %1 of %2").arg(i).arg(tx.size()));

         if(lineDelay > 0)
         {
            if(tx[i].toAscii() == '\n')
            {
               readyCont = false;
               QTimer::singleShot(int(lineDelay * 1000), this, SLOT(lineDelaySlot()));
               while(!readyCont)
               {
                  qApp->processEvents();
               };
            };
         };

         i++;
      };
   };

   comPort->flush();
   comPort->close();
   progressDialog.close();
   activeWindow->textEdit->setTextCursor(prevCursor);
   receiveAct->setEnabled(TRUE);
   sendAct->setEnabled(TRUE);
   QApplication::restoreOverrideCursor();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::receiveButtonClicked()
{
   QString tx;
   int count, i;
   char buf[1024];
   MdiChild *activeWindow;


   showError(E_NO_ERROR);
   count = 0;

   loadConfig();
   if(comPort->open(QIODevice::ReadWrite))
     stop = false;
   else
   {
      stop = true;
      showError(E_INVALID_FD);
      return;
   };
   comPort->flush();
   comPort->reset();

   newFile();
   activeWindow = activeMdiChild();
   if(!(activeWindow != 0))
     return;

   receiveAct->setEnabled(FALSE);
   sendAct->setEnabled(FALSE);
   QApplication::setOverrideCursor(Qt::BusyCursor);

   QProgressDialog progressDialog(this);
   progressDialog.setRange(0, 32768);
   progressDialog.setModal(TRUE);
   progressDialog.setLabelText(tr("Waiting for data..."));
   progressDialog.open();
   qApp->processEvents();

   tx.clear();
   while(!stop)
   {
      //progressDialog.setValue(count);

      qApp->processEvents();

      if(progressDialog.wasCanceled())
      {
         if(!tx.isEmpty())
           activeWindow->textEdit->insertPlainText(tx);
         break;
      };

      progressDialog.setValue(count);

      i = comPort->bytesAvailable();
      if(i > 0)
      {
         i = comPort->readLine(buf, sizeof(buf));
         if(i < 0)
         {
            stop = true;
            showError(comPort->lastError());
            break;
         };
         buf[i] = '\0';
         count += i;
         tx.append(buf);
         progressDialog.setLabelText(tr("Reciving byte %1").arg(count));
         if(tx.contains("\n"))
         {
            activeWindow->textEdit->insertPlainText(tx);
            tx.clear();
         };
         activeWindow->textEdit->centerCursor();
         qApp->processEvents();
      };

   };

   comPort->flush();
   comPort->close();
   progressDialog.close();
   receiveAct->setEnabled(TRUE);
   sendAct->setEnabled(TRUE);
   QApplication::restoreOverrideCursor();
   if(activeWindow)
     if(activeWindow->textEdit->document()->isEmpty())
     {
        activeWindow->parentWidget()->close();
     };
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::stopButtonClicked()
{
   stop = true;
   qApp->processEvents();
}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::showError(int error)
{
   switch(error)
   {
      case E_INVALID_FD                   : statusBar()->showMessage(tr("Invalid file descriptor (port was not opened correctly)"));
                                            break;
      case E_NO_MEMORY                    : statusBar()->showMessage(tr("Unable to allocate memory tables"));
                                            break;
      case E_CAUGHT_NON_BLOCKED_SIGNAL    : statusBar()->showMessage(tr("Caught a non-blocked signal"));
                                            break;
      case E_PORT_TIMEOUT                 : statusBar()->showMessage(tr("Operation timed out"));
                                            break;
      case E_INVALID_DEVICE               : statusBar()->showMessage(tr("The file opened by the port is not a character device"));
                                            break;
      case E_BREAK_CONDITION              : statusBar()->showMessage(tr("The port detected a break condition"));
                                            break;
      case E_FRAMING_ERROR                : statusBar()->showMessage(tr("The port detected a framing error (incorrect baud rate settings ?)"));
                                            break;
      case E_IO_ERROR                     : statusBar()->showMessage(tr("There was an I/O error while communicating with the port"));
                                            break;
      case E_BUFFER_OVERRUN               : statusBar()->showMessage(tr("Character buffer overrun"));
                                            break;
      case E_RECEIVE_OVERFLOW             : statusBar()->showMessage(tr("Receive buffer overflow"));
                                            break;
      case E_RECEIVE_PARITY_ERROR         : statusBar()->showMessage(tr("The port detected a parity error in the received data"));
                                            break;
      case E_TRANSMIT_OVERFLOW            : statusBar()->showMessage(tr("Transmit buffer overflow"));
                                            break;
      case E_READ_FAILED                  : statusBar()->showMessage(tr("General read operation failure"));
                                            break;
      case E_WRITE_FAILED                 : statusBar()->showMessage(tr("General write operation failure"));
                                            break;
      case E_NO_ERROR                     : statusBar()->showMessage(tr("No Error has occured"));
                                            break;
      default                             : statusBar()->showMessage(tr("Unknown error"));
   };


}

//**************************************************************************************************
//
//**************************************************************************************************

void edytornc::doCmpMacro()
{
   if(activeMdiChild())
     activeMdiChild()->compileMacro();
}

//**************************************************************************************************
//
//**************************************************************************************************




//**************************************************************************************************
//
//**************************************************************************************************


//**************************************************************************************************
//
//**************************************************************************************************

