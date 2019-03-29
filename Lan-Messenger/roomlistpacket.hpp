#ifndef ROOMLISTPACKET_HPP
#define ROOMLISTPACKET_HPP

#include "packet.hpp"

class RoomListPacket : public Packet
{
	public:
		unsigned int minSize = 4;
		unsigned int maxSize = 4;
		enum fields {ROOM = 3};
		std::unordered_map<unsigned int, QString> fieldMap = {{Packet::fields::TYPE, "ROOMLIST"}};
};

#endif // ROOMLISTPACKET_HPP
