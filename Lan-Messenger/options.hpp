#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <QObject>
#include <QString>

/*
 *
 * Provides an interface for other classes to load, save and update settings
 *
 */

class Options : public QObject
{
	Q_OBJECT
	public:
		explicit Options(QObject *parent = nullptr);
		QString name();

	private:
		 QString name_;

	signals:
		void nameChanged(QString name);

	public slots:
		void onSetName(QString name);

};

#endif // OPTIONS_HPP
