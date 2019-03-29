#ifndef ROOMPACKET_HPP
#define ROOMPACKET_HPP

#include "packet.hpp"

class RoomPacket : public Packet
{
	public:
		unsigned int minSize = 6;
		unsigned int maxSize = 6;
		enum fields {PROTOCOL, VERSION, TYPE, ROOM, FROM, TEXT};
};

#endif // ROOMPACKET_HPP
