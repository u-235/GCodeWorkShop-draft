/***************************************************************************
 *   Copyright (C) 2006-2018 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of GCodeWorkShop.                                   *
 *                                                                         *
 *   GCodeWorkShop is free software; you can redistribute it and/or modify *
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

#ifndef SERIALPORTTESTDIALOG_H
#define SERIALPORTTESTDIALOG_H

#include <QDialog>  // for QDialog
#include <QObject>  // for Q_OBJECT, slots
#include <QString>  // for QString
#include <Qt>       // for WindowType, WindowFlags

class QSerialPort;
class QTimer;
class QWidget;

#include "serialportsettings.h"         // for SerialPortSettings
#include "ui_serialporttestdialog.h"    // for Ui::SerialPortTestDialog


class SerialPortTestDialog : public QDialog, private Ui::SerialPortTestDialog
{
	Q_OBJECT

public:
	SerialPortTestDialog(QWidget* parent = 0, Qt::WindowFlags f = Qt::Dialog);
	~SerialPortTestDialog();

private slots:
	void closeButtonClicked();
	void clearButtonClicked();
	void connectButtonToggled(bool tg);
	void configButtonClicked();
	void changeSettings();
	void updateLeds();
	void setRtsButtonClicked();
	void setDtrButtonClicked();
	void sendTextEditChanged();
	void lineDelaySlot();
	void setXonButtonClicked();
	void setXoffButtonClicked();
	void textEditScroll(int pos);
	void hexTextEditScroll(int pos);

private:
	void loadSerialConfignames();
	void showError(int error);
	void sendText(QString tx);

	bool stop;
	QString portName;
	QString sendAtEnd;
	QString sendAtBegining;

	QSerialPort* comPort;
	QTimer* timer;
	long int count;
	double lineDelay;
	bool readyCont;
	SerialPortSettings portSettings;
};

//class TransProgressDialog : public QDialog, private Ui::TransProgressDialog
//{
//    Q_OBJECT

//public:
//    TransProgressDialog(QWidget *parent = 0, Qt::WindowFlags f = Qt::Dialog);
//    ~TransProgressDialog();

//public slots:
//    void setLabelText(const QString text);
//    void open(QSerialPort *port);
//    bool wasCanceled();
//    void setRange(int min, int max);
//    void setValue(int val);

//protected:
//    void closeEvent(QCloseEvent *event);

//private slots:
//    void cancelButtonClicked();
//    void updateLeds();


//private:
//    QSerialPort *comPort;
//    bool canceled;

//    QTimer *timer;
//};

#endif // SERIALPORTTESTDIALOG_H
