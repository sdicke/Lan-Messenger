#ifndef ROOMLISTPACKET_HPP
#define ROOMLISTPACKET_HPP

#include "packet.hpp"

class RoomListPacket : public Packet
{
	public:
		unsigned int minSize = 4;
		unsigned int maxSize = 4;
		enum fields {PROTOCOL, VERSION, TYPE, ROOM};
};

#endif // ROOMLISTPACKET_HPP
