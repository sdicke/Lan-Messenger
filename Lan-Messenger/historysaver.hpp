// Lan Messenger
// Copyright (C) 2017 Sebastian Martin Dicke
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

#ifndef HISTORYSAVER_HPP
#define HISTORYSAVER_HPP

#include <QObject>
#include <QFile>
#include <QStandardPaths>
#include <QStringList>
#include <QDir>
#include <QByteArray>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>

/*
 *
 * Providing a interface to read saved chats from a file and to save new chat
 * messages successive in a file
 *
 */

class HistorySaver : public QObject
{

	Q_OBJECT

	public:
		HistorySaver(QString partner);


	private:
		QFile file;

	signals:

		void hasBlock(QByteArray block);

	public slots:
		void saveLine(QString line);

};

#endif // HISTORYSAVER_HPP
