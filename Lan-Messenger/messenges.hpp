#ifndef MESSENGES_HPP
#define MESSENGES_HPP

#include <QByteArray>
#include <QString>

struct Message
{

		Message (QByteArray &data);
		QString id;
		QString empty;
		QString features;
		QString type;

};

struct Discover : public Message
{

		Discover (QByteArray &data);
		QString payload;

};

struct Roomlist : public Message
{

		Roomlist(QByteArray &data);
		QString payload;

};

struct PM : public Message
{

		PM (QByteArray &data);
		QString from;
		QString text;

};

struct Room : public Message
{

		Room (QByteArray &data);
		QString room;
		QString from;
		QString text;

};


#endif // MESSENGES_HPP
