#ifndef HISTORYSAVER_HPP
#define HISTORYSAVER_HPP

#include <cstdint>
#include <QObject>
#include <QFile>
#include <QStandardPaths>
#include <QStringList>
#include <QDir>
#include <QByteArray>
#include <QDateTime>

#include <iostream>

class HistorySaver : public QObject
{

	Q_OBJECT

	public:
		HistorySaver(QString partner);


	private:
		qint64 endpos;
		QFile file;

	signals:
		void saveFailed();
		void lineLoaded();
		void loadFailed();
		void hasLine(QByteArray line);
		void hasBlock(QByteArray block);

	public slots:
		void saveLine(QString line);
		void loadLine();
		void loadBlock();

};

#endif // HISTORYSAVER_HPP
