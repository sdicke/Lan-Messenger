#include "options.hpp"

Options::Options(QObject *parent) : QObject(parent)
{
}

QString Options::name()
{
	return this->name_;
}

void Options::onSetName(QString name)
{
	emit nameChanged(name);
}

