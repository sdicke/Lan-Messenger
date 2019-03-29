#ifndef PMPACKET_HPP
#define PMPACKET_HPP

#include "packet.hpp"

class PMPacket : public Packet
{
	public:
		unsigned int minSize = 5;
		enum fields {PROTOCOL, VERSION, TYPE, FROM, TEXT};
		PMPacket();
};

#endif // PMPACKET_HPP
