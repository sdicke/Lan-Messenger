// Lan Messenger
// Copyright (C) 2012 Faraz Fallahi <fffaraz@gmail.com>
// Copyrigh (C) 2017 - 2018 improved by Sebastian Martin Dicke <Sebastianmartindicke [AT] gmx.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 

#include "pmwindow.h"
#include "ui_pmwindow.h"

PMWindow::PMWindow(QString &partner, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::PMWindow)
{
	ui->setupUi(this);
	ui->txtInput->setFocus();
	ui->txtChat->setText("<p>You chats now with <span style=\"color: blue; font-weight: bold; line-height: 0.25;\">" +  partner + "</p>");
	this->saver = new HistorySaver(partner);
//	connect(this, SIGNAL(foc))
}

PMWindow::~PMWindow()
{
	delete saver;
	delete ui;
}

void PMWindow::focusInEvent(QFocusEvent* event) {
	event->accept();
	ui->txtInput->setFocus();
}

void PMWindow::on_btnSend_clicked()
{
	QString message = ui->txtInput->text();
	if (!message.isEmpty())
	{
		emit enteredText(message);
		QString style = "<span style=\"color: blue; font-weight: bold;\">Me: </span>";
		QString string("<p>" + style + ui->txtInput->text() + "</p>");
		ui->txtChat->append(string);
		ui->txtInput->clear();
		ui->txtInput->setFocus();
		this->saver->saveLine(string);
	}
	ui->txtInput->setFocus();
}

void PMWindow::receivedPM(QString text)
{
	QString style = "<span style=\"color: red; font-weight: bold;\">";
	QString string("<p>" + style + this->windowTitle() + "</span>" + ": " + text + "</p>");
	ui->txtChat->append(string);
	this->saver->saveLine(string);
	ui->txtInput->setFocus();
}

void PMWindow::on_txtInput_returnPressed()
{
	on_btnSend_clicked();
}

void PMWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	emit closedWindow();
	this->deleteLater();
}

void PMWindow::IDchanged(QString name)
{

}
