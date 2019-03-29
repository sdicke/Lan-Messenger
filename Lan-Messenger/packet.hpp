#ifndef PACKET_HPP
#define PACKET_HPP

#include <limits>
#include <unordered_map>
#include <any>
#include <typeinfo>
#include <QString>

class Packet
{
	public:
		unsigned int minSize = 0;
		unsigned int maxSize = std::numeric_limits<unsigned int>::max();
		enum fields : unsigned int {PROTOCOL, VERSION, TYPE};
		std::unordered_map<fields, QString> fieldMap{{PROTOCOL, "288"}, {VERSION, "0"}, {TYPE, "DISCOVERY"}};
		QString name;
};

#endif // PACKET_HPP
