// Lan Messenger
// Copyright (C) 2012 Faraz Fallahi <fffaraz@gmail.com>
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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	msgr = new Messenger(this);
	DialogNickName *dlgName = new DialogNickName(this);
	connect(dlgName, SIGNAL(setName(QString)), &this->options, SLOT(onSetName(QString)));
	connect(&this->options, SIGNAL(nameChanged(QString)), this, SLOT(onSetName(QString)) );
	connect(msgr, SIGNAL(peersUpdated()), this , SLOT(onUpdateList()));
	connect(msgr, SIGNAL(roomListUpdated(QString,QString)), this, SLOT(onRoomListUpdated(QString,QString)));
	connect(msgr, SIGNAL(receivedPM(QString,QString)), this, SLOT(onReceivedPM(QString,QString)));
	connect(msgr, SIGNAL(receivedRoom(QString,QString,QString)), this, SLOT(onReceivedRoom(QString,QString,QString)));
	connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(onAbout()));
	connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(onExit()));
	connect(ui->actionJoin, SIGNAL(triggered(bool)), this, SLOT(onJoin()));
	connect(ui->actionList, SIGNAL(triggered(bool)), this, SLOT(onList()));
	connect(ui->actionOptions, SIGNAL(triggered(bool)), this, SLOT(onOptions()));
	dlgName->setModal(true);
	dlgName->setWindowTitle("You Nickname, Please");
	dlgName->show();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onSetName(QString name)
{
	msgr->setName(name);
	ui->lblWelcome->setText("Welcome " + msgr->Name());
	this->myName = name;
	msgr->start();
}

void MainWindow::onUpdateList()
{
	ui->listUsers->clear();
	QList<Peer>& peers = msgr->getPeers();
	for (auto &current : peers)
	{
		ui->listUsers->addItem(current.ID());
	}
}

void MainWindow::onRoomListUpdated(QString room, QString msg)
{
	QList<QString> ids;
	QList<Peer>& peers = msgr->getRoomPeers(room);
	for(int i=0; i<peers.count(); i++)
	{
		ids.append(peers[i].ID());
	}
	makeRoomWindow(room)->updateList(ids);
	makeRoomWindow(room)->receivedPM("", msg);
}

void MainWindow::on_listUsers_doubleClicked(const QModelIndex &index __attribute__((unused)))
{
	QString userid = ui->listUsers->currentItem()->text();
	makePMWindow(userid);
}

PMWindow* MainWindow::makePMWindow(QString partner)
{
	if(pms.keys().contains(partner))
	{
		pms[partner]->setFocus();
		return pms[partner];
	}
	else
	{
		PMWindow* newpm = new PMWindow(partner, myName);
		pms.insert(partner, newpm);
		pmr.insert(newpm, partner);
		connect(newpm, SIGNAL(enteredText(QString)), this, SLOT(onPMSend(QString)));
		connect(newpm, SIGNAL(closedWindow()), this, SLOT(onPMClosed()));
		connect(&this->options, SIGNAL(nameChanged(QString)), newpm, SLOT(IDchanged(QString)));
		newpm->setWindowTitle(partner);
		newpm->show();
		return newpm;
	}
}

RoomWindow* MainWindow::makeRoomWindow(QString title)
{
	if(rms.keys().contains(title))
	{
		rms[title]->setFocus();
		return rms[title];
	}
	else
	{
		RoomWindow* newrm = new RoomWindow();
		rms.insert(title, newrm);
		rmr.insert(newrm, title);
		connect(newrm, SIGNAL(enteredText(QString)), this, SLOT(onRoomSend(QString)));
		connect(newrm, SIGNAL(closedWindow()), this, SLOT(onRoomClosed()));
		connect(newrm, SIGNAL(startPM(QString)), this, SLOT(onRoomPM(QString)));
		newrm->setWindowTitle(title);
		newrm->show();
		return newrm;
	}
}

void MainWindow::onRoomPM(QString id)
{
	makePMWindow(id);
}

void MainWindow::onPMSend(QString text)
{
	PMWindow* to = qobject_cast<PMWindow*>(sender());
	msgr->sendPM(text, pmr[to]);
}

void MainWindow::onRoomSend(QString text)
{
	RoomWindow* room = qobject_cast<RoomWindow*>(sender());
	msgr->sendRoom(text, rmr[room]);
}

void MainWindow::onRoomClosed()
{
	RoomWindow* window = qobject_cast<RoomWindow*>(sender());
	msgr->leaveRoom(rmr[window]);
	rms.remove(rmr[window]);
	rmr.remove(window);
}

void MainWindow::onPMClosed()
{
	PMWindow* window = qobject_cast<PMWindow*>(sender());
	//msgr->sendPM("Closed chat window.", pmr[window]);
	pms.remove(pmr[window]);
	pmr.remove(window);
}

void MainWindow::onReceivedPM(QString from, QString text)
{
	auto pmwindow = makePMWindow(from);
	pmwindow->receivedPM(text);
}

void MainWindow::onReceivedRoom(QString room, QString from, QString text)
{
	makeRoomWindow(room)->receivedPM(from, text);
}

void MainWindow::onJoinRoom(QString room)
{
	makeRoomWindow(room)->setFocus();
	msgr->joinRoom(room);
}

void MainWindow::onOptions()
{
	OptionsWindow* options = new OptionsWindow();
	connect(options, SIGNAL(nameSet(QString)), &this->options, SLOT(onSetName(QString)));
	options->setWindowTitle("288 LanMessenger - Options");
	options->show();
}

void MainWindow::onAbout()
{
	DialogAbout* about = new DialogAbout(this);
	about->setModal(true);
	about->setWindowTitle("About 288 L.M.");
	about->show();
}

void MainWindow::onList()
{

}

void MainWindow::onJoin()
{
	DialogRoom* joinroom = new DialogRoom(this);
	connect(joinroom, SIGNAL(joinRoom(QString)), this, SLOT(onJoinRoom(QString)));
	joinroom->setModal(true);
	joinroom->setWindowTitle("Room Name");
	joinroom->show();
}

void MainWindow::onExit()
{
	QMainWindow::close();
}
