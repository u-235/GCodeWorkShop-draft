/***************************************************************************
 *   Copyright (C) 2008 by Artur Kozioł                                    *
 *   artkoz@poczta.onet.pl                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
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



#include "mdichild.h"




//**************************************************************************************************
//
//**************************************************************************************************

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    setWordWrapMode(QTextOption::NoWrap);
    highlighter = 0;
    setAcceptRichText(FALSE);
    installEventFilter( this );
    //resize(QSize(400, 400));


    //setContentsMargins(200,200,200,200); 

    //connect(this, SIGNAL(moveEvent(QMoveEvent *)), this, SLOT(savePosMoveEvent(QMoveEvent *)));
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.nc").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    setWindowIcon(QIcon(":/images/ncfile.png"));

    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
}

//**************************************************************************************************
//
//**************************************************************************************************

bool MdiChild::loadFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("EdytorNC"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString tex = in.readAll();
    setPlainText(tex);
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName, tex);
    //document()->adjustSize();

    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

    return true;
}

//**************************************************************************************************
//
//**************************************************************************************************

bool MdiChild::save()
{
    setFocus();
    if(isUntitled) 
    {
       return saveAs();
    } 
    else 
    {
       return saveFile(curFile);
    }
}

//**************************************************************************************************
//
//**************************************************************************************************

bool MdiChild::saveAs()
{
    
    CustomFDialog *fileDialog;
    QStringList filters;
    QFileInfo file;
    QString fileName;
    

    fileDialog = new CustomFDialog(this, fdShowPreview);

    filters << tr("CNC programs files (*.nc)")
            << tr("CNC programs files (*.min)")
            << tr("CNC programs files (*.anc)")
            << tr("CNC programs files (*.cnc)")
            << tr("Text files (*.txt)")
            << tr("All files (*.* *)");

    fileDialog->setNameFilters(filters);
    fileDialog->setHighlightColors(mdiWindowProperites.hColors);

    fileDialog->restoreState(saveDialogState);

    if(!saveFileFilter.isEmpty())
      fileDialog->selectNameFilter(saveFileFilter);

    fileDialog->selectFile(curFile); 
    fileDialog->setConfirmOverwrite(FALSE);

    if(fileDialog->exec() == QDialog::Accepted)
    {
       saveDialogState = fileDialog->saveState();
       saveFileFilter = fileDialog->selectedNameFilter();
       fileName = fileDialog->selectedFile();
       
       if((QFile(fileName).exists()))
       {
          switch( QMessageBox::warning(this, tr("Warring"), tr("File : %1 exists. Overwrite ?").arg(fileName),
                                       tr("Yes"), tr("No")))
          {
             case 0:  break;
             case 1:  return false;
          }
       }
       return saveFile(fileName);
    };
    return false;

    

}

//**************************************************************************************************
//
//**************************************************************************************************

bool MdiChild::saveFile(const QString &fileName)
{
    int curPos;
    QRegExp exp;
    QString f_tx;
    QTextCursor cursor;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) 
    {
        QMessageBox::warning(this, tr("EdytorNC"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    };

    QApplication::setOverrideCursor(Qt::WaitCursor);
    curPos = textCursor().position(); 
   
    QDate dat = QDate::currentDate();


    exp.setPattern("(DATA|DATE)[:\\s]*[\\d][\\d](\\.|-)[\\d][\\d](\\.|-)[\\d][\\d][\\d][\\d]");
    cursor = textCursor();
    cursor.setPosition(0);
    //setTextCursor(cursor);

    cursor = document()->find(exp, cursor);
    if(!cursor.isNull())
    {
       setUpdatesEnabled( FALSE );
       cursor.removeSelectedText();
       cursor.insertText(dat.toString("dd.MM.yyyy"));

       setUpdatesEnabled( TRUE );
       repaint();
    }
    else
    {
       exp.setPattern("[\\d][\\d](\\.|-)[\\d][\\d](\\.|-)[\\d][\\d][\\d][\\d]");
       cursor = textCursor();
       cursor.setPosition(0);
       cursor = document()->find(exp, cursor);
       if(!cursor.isNull())
       {
          setUpdatesEnabled( FALSE );
          cursor.removeSelectedText();
          cursor.insertText(dat.toString("dd.MM.yyyy"));

          setUpdatesEnabled( TRUE );
          repaint();
       }

    };

    
    QTextStream out(&file);
    QString tex = toPlainText();
    if(!tex.contains("\r\n"))
      tex.replace("\n", "\r\n" ); 
    out << tex;
    QApplication::restoreOverrideCursor();
    
    cursor = textCursor();
    cursor.setPosition(curPos);
    setTextCursor(cursor);
    
    setCurrentFile(fileName, tex);
    return true;
}

//**************************************************************************************************
//
//**************************************************************************************************

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::closeEvent(QCloseEvent *event)
{
    if(maybeSave()) 
    {
       event->accept();
    } 
    else 
    {
       event->ignore();
    }
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::documentWasModified()
{
    setWindowModified(document()->isModified());
}

//**************************************************************************************************
//
//**************************************************************************************************

bool MdiChild::maybeSave()
{
    if(document()->isModified()) 
    {
	QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("EdytorNC"),
                                         tr("'%1' has been modified.\n"
                                            "Do you want to save your changes?").arg(userFriendlyCurrentFile()),
                                         QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save)
          return save();
        else 
          if(ret == QMessageBox::Cancel)
            return FALSE;
    }
    return TRUE;
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::setCurrentFile(const QString &fileName, const QString &text)
{

    int pos;
    QRegExp exp;
    QString f_tx;


    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);

    exp.setPattern("\\([^\\n\\r]*\\)|;[^\\n\\r]*"); //find first comment and set it in window tilte
    pos = 0;
    pos = text.indexOf(exp, pos);
    while(pos != -1)
    {
       f_tx = text.mid(pos, exp.matchedLength());
       if(!(f_tx.mid(0, 2) == ";$"))
       {
          f_tx.remove('(');
          f_tx.remove(')');
          f_tx.remove(';');
          break;
       };
       pos += exp.matchedLength();
       pos = text.indexOf(exp, pos);

    };
   
 
    setWindowTitle(QString("%2 ----> %1 [*]").arg(userFriendlyCurrentFile()).arg(f_tx.simplified()));

}

//**************************************************************************************************
//
//**************************************************************************************************

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

//**************************************************************************************************
//
//**************************************************************************************************

_editor_properites MdiChild::getMdiWindowProperites()
{
   mdiWindowProperites.isRedo = document()->isRedoAvailable();
   mdiWindowProperites.isUndo = document()->isUndoAvailable();
   mdiWindowProperites.ins = overwriteMode();
   mdiWindowProperites.modified = document()->isModified();
   mdiWindowProperites.readOnly = isReadOnly();
   mdiWindowProperites.isSel = textCursor().hasSelection();
   mdiWindowProperites.cursorPosX = textCursor().position(); //textCursor().blockNumber();
   mdiWindowProperites.geometry = saveGeometry();
   QRect rect = frameGeometry();
   QPoint position = QPoint(rect.x(), rect.y());
   mdiWindowProperites.winPos = position;
   mdiWindowProperites.fileName = curFile;
   return(mdiWindowProperites);
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::setMdiWindowProperites(_editor_properites opt)
{
   mdiWindowProperites = opt;
   setReadOnly(mdiWindowProperites.readOnly);
   setFont(QFont(mdiWindowProperites.fontName, mdiWindowProperites.fontSize, QFont::Normal));
   //editorOpt.lastDir = opt.lastDir;

   
   if(mdiWindowProperites.syntaxH)
   {
      if(highlighter <= 0)
        highlighter = new Highlighter(document(), mdiWindowProperites.hColors);
      if(highlighter > 0)
        highlighter->rehighlight();
   }
   else
   {
      if(highlighter > 0)
        delete(highlighter);
      highlighter = 0;
   };
    
   
   QTextCursor cursor = textCursor();
   cursor.setPosition(mdiWindowProperites.cursorPosX);
   setTextCursor(cursor);
   restoreGeometry(mdiWindowProperites.geometry);
   ensureCursorVisible();
}

//**************************************************************************************************
//
//**************************************************************************************************

bool MdiChild::eventFilter(QObject *obj, QEvent *ev)
{
   if((obj == this) && !(isReadOnly()))
   {
       if ( ev->type() == QEvent::KeyPress )
       {
          QKeyEvent *k = (QKeyEvent*)ev;

          if((k->key() == Qt::Key_Comma) && (k->modifiers() == Qt::KeypadModifier))
          {
             insertPlainText(".");
             return TRUE;
          };

          if(mdiWindowProperites.intCapsLock)
          {
             if((k->text().toAscii() >= QByteArray("a")) && (k->text().toAscii() <= QByteArray("z")))
             {
                insertPlainText(k->text().toUpper());
                return TRUE;
             };

             if(((k->text().toAscii() >= QByteArray("A")) && (k->text().toAscii() <= QByteArray("Z"))) 
                && (k->modifiers() == Qt::ShiftModifier))
             {
                insertPlainText(k->text().toLower());
                return TRUE;
             };
          };
       };

       return FALSE;
   }
   else
   {
      // pass the event on to the parent class
      return QTextEdit::eventFilter(obj, ev);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

int MdiChild::doRenumber(int &mode, int &startAt, int &from, int &prec, int &inc, bool &renumEmpty, bool &renumComm)
{
   int pos, i, num, it, count;
   QString tx, f_tx, line, i_tx, new_tx;
   QRegExp exp;
   bool ok;

   exp.setCaseSensitivity (Qt::CaseInsensitive);

   tx = toPlainText();
   count = 0;
   while(1)
   {
      if(mode == 3)
      {
         pos = 0;
         num = 0;
         exp.setPattern("[N]{1,1}[0-9\\s]+|\\([^\\n\\r]*\\)|\'[^\\n\\r]*\'|;[^\\n\\r]*");
         while((pos = tx.indexOf(exp, pos)) >= 0)
         {
            f_tx = tx.mid(pos, exp.matchedLength());
            if(((f_tx.contains('(') == 0) && (f_tx.contains('\'') == 0) && (f_tx.contains(';') == 0)))
            {
               tx.remove(pos, exp.matchedLength());
               num++;
            }
            else
              pos += exp.matchedLength();
         };
         break;
      };

      if(mode == 1)
      {
         pos = 0;
         num = startAt;
         exp.setPattern("[N]{1,1}[0-9\\s]+|\\([^\\n\\r]*\\)|\'[^\\n\\r]*\'|;[^\\n\\r]*");
         while((pos = tx.indexOf(exp, pos)) >= 0)
         {
            f_tx = tx.mid(pos, exp.matchedLength());

            if((f_tx.contains(' ') == 0) && (f_tx.contains('\n') == 0))
              i = exp.matchedLength();
            else
              i = exp.matchedLength() - 1;

            if(((f_tx.contains('(') == 0) && (f_tx.contains('\'') == 0) && (f_tx.contains(';') == 0)))
            {
                f_tx.remove(0, 1);
                f_tx.remove(' ');
                if(!f_tx.isEmpty())
                  it = f_tx.toInt(&ok);
                else
                  it = 0;
                if((it >= from))
                {
                   f_tx = QString("N%1").arg(num, prec);
                   f_tx.replace(' ', '0');
                   tx.replace(pos, i, f_tx);
                   num += inc;
                   count++;
                };
            };
            pos += exp.matchedLength();
         };
         break;
      };

      if(mode == 2)
      {
         num = startAt;
         for(i = 0; i < (document()->lineCount() - 1); i++)
         {
            line = tx.section('\n', i, i);
            exp.setPattern("[N]{1,1}[0-9]+|\\([^\\n\\r]*\\)|\'[^\\n\\r]*\'|;[^\\n\\r]*");
            pos = 0;
            while(1)
            {
               if(line.isEmpty())
               {
                  if(!renumEmpty)
                    break;
                  f_tx = QString("N%1").arg(num, prec);
                  f_tx.replace(' ', '0');
                  num += inc;
                  count++;
                  line.insert(0, f_tx);
                  break;
               };

               if(((pos = line.indexOf(exp, pos)) >= 0) && (line.at(0) != '$'))
               {
                  i_tx = line.mid(pos, exp.matchedLength());

                  if(((i_tx.contains('(') == 0) && (i_tx.contains('\'') == 0) && (i_tx.contains(';') == 0)))
                  {
                     f_tx = QString("N%1").arg(num, prec);
                     f_tx.replace(' ', '0');
                     num += inc;
                     count++;
                     line.replace(i_tx, f_tx);
                     break;
                  }
                  else
                    if(renumComm)
                      break;
               }

               if((line.at(0) == 'N') && (!line.at(1).isLetter()))
               {
                  f_tx = QString("N%1").arg(num, prec);
                  f_tx.replace(' ', '0');
                  num += inc;
                  count++;
                  line.replace(0, 1, f_tx);
                  break;
               };

               if(((line.at(0) != '%') && (line.at(0) != ':') && (line.at(0) != 'O') && (line.at(0) != '$')))
               {
                  f_tx = QString("N%1").arg(num, prec);
                  f_tx.replace(' ', '0');
                  num += inc;
                  count++;
                  line.insert(0, f_tx);
                  break;
               };
               break;
            };
            new_tx += line + '\n';
         };
         tx = new_tx;
         break;
      };
      break;
   };

   selectAll();
   insertPlainText(tx);
   QTextCursor cursor = textCursor();
   cursor.setPosition(0);
   setTextCursor(cursor);

}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::doRemoveSpace()
{
   int i;
   QString tx;

   tx = toPlainText();

   for(i = 0; i <              tx.length(); i++)
   {
      if(tx.at(i) == '(')
        do
        {
           i++;
           if(i > tx.length())
             break;
        }while(!((tx.at(i) == ')') || (tx.at(i) == '\n')));

      if(tx.at(i) == '\'')
        do
        {
           i++;
           if(i > tx.length())
             break;
        }while(!((tx.at(i) == '\'') || (tx.at(i) == '\n')));

      if(tx.at(i) == ';')
        do
        {
           i++;
           if(i > tx.length())
             break;
        }while(!((tx.at(i) == '\n')));

      if(tx.at(i) == ' ')
      {
          tx.remove(i, 1);
          i--;
      };
   };

   selectAll();
   insertPlainText(tx);
   QTextCursor cursor = textCursor();
   cursor.setPosition(0);
   setTextCursor(cursor);
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::doInsertSpace()
{
   int pos;
   QString tx;
   QRegExp exp;

   exp.setCaseSensitivity (Qt::CaseInsensitive);

   exp.setPattern("[A-Z]+|[#@;:(\\']");

   tx = toPlainText();
   pos = 1;
   while((pos = tx.indexOf(exp, pos)) > 0)
   {

      while(1)
      {

         if(tx.at(pos) == '(')
         {
            if((tx.at(pos-1) != ' ') && (tx.at(pos-1) != '\n'))
            {
               tx.insert(pos, ' ');
               pos++;
            };
            do
            {
               pos++;
               if(pos > tx.length())
                 break;
            }while(!((tx.at(pos) == ')') || (tx.at(pos) == '\n')));
            break;
         };

         if(tx.at(pos) == ';')
         {
            if((tx.at(pos-1) != ' ') && (tx.at(pos-1) != '\n'))
            {
               tx.insert(pos, ' ');
               pos++;
            };
            do
            {
               pos++;
               if(pos > tx.length())
                 break;
            }while(!((tx.at(pos) == '\n')));
            break;
         };

         if(tx.at(pos) == '\'')
         {
            do
            {
               pos++;
               if(pos > tx.length())
                 break;
            }while(!((tx.at(pos) == '\'') || (tx.at(pos) == '\n')));
            break;
         };

         if((tx.at(pos) == '#'))
         {
            if(tx.at(pos-1).isDigit())
            {
               tx.insert(pos, ' ');
               pos++;
            };
            break;
         };

         if((tx.at(pos-1) != ' ') && (tx.at(pos-1) != '\n'))
         {
            tx.insert(pos, ' ');
            pos++;
            break;
         };

         break;
      };
      pos +=  exp.matchedLength();
   };

   selectAll();
   insertPlainText(tx);
   QTextCursor cursor = textCursor();
   cursor.setPosition(0);
   setTextCursor(cursor);
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::doInsertDot()
{
   int pos, count;
   QString tx, f_tx;
   QRegExp exp;
   double it;
   bool ok;

   exp.setCaseSensitivity (Qt::CaseInsensitive);

   exp.setPattern(QString("[%1]{1,1}[-.+0-9]+|\\([^\\n\\r]*\\)|\'[^\\n\\r]*\'|;[^\\n\\r]*$").arg(mdiWindowProperites.dotAdr));

   count = 0;
   tx = toPlainText();
   pos = 0;
   while((pos = tx.indexOf(exp, pos)) >= 0)
   {
      f_tx = tx.mid(pos, exp.matchedLength());
      pos++;

      if(((f_tx.contains('(') == 0) && (f_tx.contains('\'') == 0) && (f_tx.contains(';') == 0)))
      {
         if(mdiWindowProperites.dotAfter && (f_tx.contains('.') == 0))
         {
            f_tx.remove(0, 1);

            //f_tx.remove('+');
            it = f_tx.toDouble(&ok);
            if(ok)
            {
               it = it / mdiWindowProperites.dotAftrerCount;
               tx.replace(pos, exp.matchedLength() - 1, QString("%1").arg(it, 0, 'f', 3));
               count++;
            };
         };

         if((mdiWindowProperites.atEnd && (f_tx.contains('.') == 0)))
         {
            tx.insert(pos + exp.matchedLength() - 1, '.');
            pos++;
            count++;
         };
     }
     else
       pos += (exp.matchedLength());
  };

  //emit message( QString(tr("Inserted : %1 dots.")).arg(count), 6000 );
  cleanUp(&tx);
  selectAll();
  insertPlainText(tx);
  QTextCursor cursor = textCursor();
  cursor.setPosition(0);
  setTextCursor(cursor);
}

//**************************************************************************************************
//
//**************************************************************************************************

void MdiChild::cleanUp(QString *str)  //remove not needed zeros
{

   QRegExp exp;
   int pos;


   pos = 1;
   exp.setPattern("[\\d]+[.][-+.0-9]+|\\([^\\n\\r]*\\)|\'[^\\n\\r]*\'|;[^\\n\\r]*$");

   while((pos = str->indexOf(exp, pos)) > 0)
   {
      if((str->at(pos + exp.matchedLength() - 1) == '0') && str->at(pos-1) != ';')
        str->remove(pos + exp.matchedLength() - 1, 1);
      else
        pos += exp.matchedLength();
   };

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


//**************************************************************************************************
//
//**************************************************************************************************


