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

#include "historysaver.hpp"

HistorySaver::HistorySaver(QString partner) : QObject()
{
	QString homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
	QString targetpath = homePath + "/.cache/LanMessenger/history/";
	QString path = targetpath + partner;
	this->file.setFileName(path);
	QDir wpath = QDir(targetpath);
	if (!wpath.exists()) {
		wpath.mkpath(targetpath);
	}
	if (!this->file.exists()) {
		this->file.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream stream(&file);
		stream << "";
	}
	if (!this->file.open(QIODevice::Append)) {
		qDebug() << "Cannot open " << path.toStdString().c_str();
	}
}

void HistorySaver::saveLine(QString line)
{
	QString abc = QDateTime::currentDateTime().toString();
	line.prepend(abc.toUtf8());
	QByteArray data = line.toUtf8();
	QTextStream stream (&file);
	stream << data << "\n";
}

