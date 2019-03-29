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

#include "messenger.hpp"

Messenger::Messenger(QObject *parent) :
	QObject(parent)
{
	connect(&_timerdiscovery, SIGNAL(timeout()), this, SLOT(onTimerdiscovery()));
	connect(&_udp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	_myips = QNetworkInterface::allAddresses();
	//qDebug() << _myips;
}

void Messenger::start()
{
	_udp.bind(2880, QUdpSocket::ReuseAddressHint);
	_timerdiscovery.start(5000);
}

void Messenger::setName(QString name)
{
	_mypeer.Name = name;
	_mypeer.Domain = QHostInfo::localHostName();
}

QString Messenger::Name()
{
	return _mypeer.ID();
}

void Messenger::onTimerdiscovery()
{
	QString packet = PCK_HEADER "DISCOVERY:" + _mypeer.ID();
	QHostAddress target = QHostAddress::Broadcast;
	logSent(packet, target);
	_udp.writeDatagram(packet.toUtf8(), target, 2880);

	// chekc for olds
	for(int i = 0; i < _peers.count(); i++)
		if(_peers[i].Lastseen.secsTo(QTime::currentTime()) > 10)
		{
			_peers.removeAt(i);
			emit peersUpdated();
		}

	// room list
	for(int i = 0; i < _rooms.count(); i++)
		roomList(_rooms[i]);

	for(int i = 0; i < _rooms.count(); i++)
	{
		QString room = _rooms[i];
		if(!_roomslist.contains(room))
		{
			continue;
		}
		for(int j = 0; j < _roomslist[room].count(); j++)
		{
			if(_roomslist[room].at(j).Lastseen.secsTo(QTime::currentTime()) > 10)
			{
				QString name = _roomslist[room][j].ID();
				_roomslist[room].removeAt(j);
				emit roomListUpdated(room, "*** " + name + " Left. ***");
			}
		}
	}
}

void Messenger::onReadyRead()
{
	while (_udp.hasPendingDatagrams())
	{
		QByteArray datagram;
		constexpr int maxSize = std::numeric_limits<int>::max();
		qint64 datagramSize = _udp.pendingDatagramSize();
		if (datagramSize > maxSize) {
			qDebug() << "Datagram size exceeds processing limit of " << maxSize << " bytes.";
		}
		datagram.resize(static_cast<int>(datagramSize));
		QHostAddress sender;
		quint16 senderPort;

		_udp.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

		bool flag = true;
		for(int i = 0; i < _myips.count(); ++i)
		{
			if(sender.toString() == _myips[i].toString())
			{
				flag = false;
			}
		}

		if (flag)
		{
			logReceived(datagram, sender);
			processTheDatagram(datagram, sender);
		}
	}
}

void Messenger::log(QString data, QString destination, bool isSent)
{
	QString message;
	message += isSent? "Sent ":"Received ";
	message += QString::number(data.length()) + " bytes";
	message += isSent? " to   ":" from ";
	QString fill;
	fill.fill(' ', 15 - destination.size());
	message += destination + fill + "  Data: ";
	message += data;
	qDebug() << message.toStdString().c_str();
}

void Messenger::logSent(QString data, QHostAddress dest)
{
	log(data, dest.toString(), true);
}

void Messenger::logReceived(QString data, QHostAddress dest)
{
	log(data, dest.toString(), false);
}

PeerList& Messenger::getPeers()
{
	return _peers;
}

PeerList& Messenger::getRoomPeers(QString room)
{
	return _roomslist[room];
}

void Messenger::processTheDatagram(QByteArray data, QHostAddress sender)
{
	const QString str_packet = QString(data);
	const QStringList packet = str_packet.split(':');
	constexpr int protocol = 0;
	constexpr int version = 1;
	constexpr int type = 2;
	if(packet.count() < 4)
	{
		qWarning("Warning: Unknown Packet. ");
		return;
	}
	if(packet[protocol] != "288")
	{
		qWarning("Warning: Protocol Mismatch. ");
		return;
	}
	if(packet[version] != "0")
	{
		qWarning("Warning: Protocol Version Mismatch. ");
		return;
	}
	if(packet[type] == "DISCOVERY")
	{
		DiscoverPacket obj;
		if (!this->isInFieldsCountRange(packet, obj))
		{
			this->handleDiscoverDatagram(packet, sender);
		}
		else
		{
			this->printHeaderFieldMismatch("DISCOVERY", 4);
		}
	}

	if(packet[type] == "ROOMLIST")
	{
		RoomListPacket obj;
		if (this->isInFieldsCountRange(packet, obj))
		{
			this->handleRoomListDatagram(packet, sender);
		}
	}

	if(packet[type] == "PM")
	{
		PMPacket obj;
		if (this->isInFieldsCountRange(packet, obj))
		{
			this->handlePMDatagram(packet);
		}
		else
		{
			this->printHeaderFieldMismatch("PM", 5);
		}
	}
	if(packet[type] == "ROOM")
	{
		RoomPacket obj;
		if (this->isInFieldsCountRange(packet, obj))
		{
			this->handleRoomDatagram(packet);
		}
		else
		{
			this->printHeaderFieldMismatch("ROOM", 6);
		}
	}
}

void Messenger::handleDiscoverDatagram(const QStringList &packet, const QHostAddress &sender) {
	int found = -1;
	for(int i = 0; i < _peers.count(); i++)
		if(_peers[i].ID() == packet[3]) found = i;
	if(found == -1)
	{
		Peer newpeer;
		QString peerid = packet[3];
		QStringList peerids = peerid.split('@');
		if(peerids.count() != 2)
		{
			qWarning("Warning: Wronge new Peer ID.");
			return;
		}
		newpeer.Name = peerids[0];
		newpeer.Domain = peerids[1];
		newpeer.Host = sender;
		newpeer.Lastseen = QTime::currentTime();
		_peers.append(newpeer);
		emit peersUpdated();
	}
	else
	{
		_peers[found].Lastseen = QTime::currentTime();
	}
}

void Messenger::handleRoomListDatagram(const QStringList &packet, const QHostAddress &sender) {
	QString room = packet[3];
	if(!_rooms.contains(room))
	{
		return;
	}
	if(!_roomslist.contains(room))
	{
		_roomslist.insert(room, PeerList());
	}

	int found = -1;
	for(int i = 0; i < _roomslist[room].count(); i++)
	{
		if(_roomslist[room][i].ID() == packet[4])
		{
			found = i;
		}
	}

	if(found == -1)
	{
		Peer newpeer;
		QString peerid = packet[4];
		QStringList peerids = peerid.split('@');
		if(peerids.count() != 2)
		{
			qWarning("Warning: Wronge new Peer ID.");
			return;
		}
		newpeer.Name = peerids[0];
		newpeer.Domain = peerids[1];
		newpeer.Host = sender;
		newpeer.Lastseen = QTime::currentTime();
		_roomslist[room].append(newpeer);
		emit roomListUpdated(room, "***  " + newpeer.ID() + "  Joined. ***");
	}
	else
	{
		_roomslist[room][found].Lastseen = QTime::currentTime();
	}
}

void Messenger::handlePMDatagram(const QStringList &packet) {
	QString from = packet[3];
	QString text = packet[4];
	for(int i=5; i < packet.count(); i++)
	{
		text += ":" + packet[i];
	}
	emit receivedPM(from, text);
}

void Messenger::handleRoomDatagram(const QStringList &packet) {
	QString room = packet[3];
	QString from = packet[4];
	QString text = packet[5];
	for(int i=6; i < packet.count(); i++)
	{
		text += ":" + packet[i];
	}
	bool found=false;
	for(int i = 0; i < _rooms.count(); i++)
	{
		if(_rooms[i] == room)
		{
			found = true;
		}
	}

	if(found)
	{
		emit receivedRoom(room, from, text);
	}
}


void Messenger::sendPM(QString text, QString to)
{
	QHostAddress address;
	for(int i = 0; i < _peers.count(); i++)
	{
		if(_peers[i].ID() == to)
		{
			address = _peers[i].Host;
		}
	}
	QString packet = PCK_HEADER "PM:" + _mypeer.ID() + ":" + text;
	logSent(packet, address);
	_udp.writeDatagram(packet.toUtf8(), address, 2880);
}

void Messenger::sendRoom(QString text, QString room)
{
	QString packet = PCK_HEADER "ROOM:" + room + ":" + _mypeer.ID() + ":" + text;
	QHostAddress target = QHostAddress::Broadcast;
	logSent(packet, target);
	_udp.writeDatagram(packet.toUtf8(), target, 2880);
}

void Messenger::joinRoom(QString room)
{
	_rooms.append(room);
}

void Messenger::leaveRoom(QString room)
{
	_rooms.removeAll(room);
}

void Messenger::roomList(QString room)
{
	QString packet = PCK_HEADER "ROOMLIST:" + room + ":" + _mypeer.ID();
	QHostAddress target = QHostAddress::Broadcast;
	logSent(packet, target);
	_udp.writeDatagram(packet.toUtf8(), target, 2880);
}

//Do not expose this function to outside
void Messenger::printHeaderFieldMismatch(const QString &type, unsigned int count)
{
	QString message("Warning: Header Field Mismatch: "  + type + " packet with lesser than " + QString::number(count) + "fields.");
	qWarning(message.toStdString().c_str());
}

bool Messenger::isInFieldsCountRange(const QStringList &packet, const Packet &protype)
{
	return packet.size() > protype.minSize && packet.size() < protype.maxSize;
}
