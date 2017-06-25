#include "historysaver.hpp"

HistorySaver::HistorySaver(QString partner) : QObject()
{
	QString homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
	this->file.setFileName(homePath + ".config/LanMessenger/history/" + partner);
	if (!this->file.open(QIODevice::ReadWrite)) {
		emit loadFailed();
	}
	this->endpos = this->file.bytesAvailable();
}

HistorySaver::~HistorySaver() {
	this->file.close();
}

void HistorySaver::saveLine(QString line)
{
	this->file.seek(this->endpos);
	QByteArray data = line.toUtf8();
	if (!this->file.write(data)) {
		emit saveFailed();
	}
	else {
		this->endpos += data.size();
	}
}

void HistorySaver::loadLine() {
	QByteArray data = this->file.readLine();
	if (data.isEmpty()) {
		emit loadFailed();
	}
	else {
		emit hasLine(data);
	}
}

void HistorySaver::loadBlock() {
	QByteArray data = this->file.readAll();
	if (data.isEmpty()){
		emit loadFailed();
	}
	else {
		emit hasBlock(data);
	}
}
