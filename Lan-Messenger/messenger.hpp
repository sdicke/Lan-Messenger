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

#ifndef MESSENGER_H
#define MESSENGER_H

#include <QtCore>
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <limits>
#include "peer.hpp"
#include "discoverpacket.hpp"
#include "pmpacket.hpp"
#include "roomlistpacket.hpp"
#include "roompacket.hpp"

#define PCK_HEADER "288:0:"
typedef QList<Peer> PeerList;

class Messenger : public QObject
{
	Q_OBJECT
public:
	explicit Messenger(QObject *parent = nullptr);
	QString Name();
	QString ID();
	PeerList& getPeers();
	PeerList& getRoomPeers(QString room);
	void sendPM(QString text, QString to);
	void sendRoom(QString text, QString room);
	void joinRoom(QString room);
	void leaveRoom(QString room);
	
signals:
	void peersUpdated();
	void roomListUpdated(QString room, QString message);
	void receivedPM(QString from, QString text);
	void receivedRoom(QString room, QString from, QString text);
	
public slots:
	void setName(QString name);
	void start();

private slots:
	void onTimerdiscovery();
	void onReadyRead();

private:
	Peer _mypeer;
	PeerList _peers;
	QUdpSocket _udp;
	QTimer _timerdiscovery;
	QList<QHostAddress> _myips;
	QList<QString> _rooms;
	QMap<QString, PeerList> _roomslist;
	void log(QString data, QString dest, bool isSent);
	void logSent(QString data, QHostAddress dest);
	void logReceived(QString data, QHostAddress dest);
	void processTheDatagram(QByteArray data, QHostAddress sender);
	void handleDiscoverDatagram(const QStringList &packet, const QHostAddress &sender);
	void handleRoomListDatagram(const QStringList &packet, const QHostAddress &sender);
	void handlePMDatagram(const QStringList &packet);
	void handleRoomDatagram(const QStringList &packet);
	void roomList(QString room);
	void printHeaderFieldMismatch(const QString &type, unsigned int count);
	bool isInFieldsCountRange(const QStringList &packet, const Packet &protype);
};

#endif // MESSENGER_H
