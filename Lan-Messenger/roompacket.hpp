#ifndef ROOMPACKET_HPP
#define ROOMPACKET_HPP

#include "packet.hpp"

class RoomPacket : public Packet
{
	public:
		unsigned int minSize = 6;
		unsigned int maxSize = 6;
		enum fields {TYPE = 3, ROOM, FROM, TEXT};
		std::unordered_map<unsigned int, QString> fieldMap = {{Packet::fields::TYPE, "ROOM"}};
};

#endif // ROOMPACKET_HPP
