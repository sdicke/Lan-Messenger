#ifndef DISCOVERPACKET_HPP
#define DISCOVERPACKET_HPP

#include "packet.hpp"

class DiscoverPacket : public Packet
{
	public:
		unsigned int minSize = 4;
		unsigned int maxSize = 4;
		enum fields {PROTOCOL, VERSION, TYPE, ID};
};

#endif // DISCOVERPACKET_HPP
