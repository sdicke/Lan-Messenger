#ifndef PACKET_HPP
#define PACKET_HPP

#include <limits>
#include <QMap>

class Packet
{
	public:
		unsigned int minSize = 0;
		unsigned int maxSize = std::numeric_limits<unsigned int>::max();
		enum fields : unsigned int;
};

#endif // PACKET_HPP
