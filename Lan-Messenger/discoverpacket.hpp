#ifndef DISCOVERPACKET_HPP
#define DISCOVERPACKET_HPP

#include "packet.hpp"

class DiscoverPacket : public Packet
{
	public:
		unsigned int minSize = 4;
		unsigned int maxSize = 4;
		enum fields {ID = 3};
		std::unordered_map<unsigned int, QString> fieldMap = {{Packet::fields::TYPE, "DISCOVERY"}};
};

#endif // DISCOVERPACKET_HPP
