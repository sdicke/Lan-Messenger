#ifndef PMPACKET_HPP
#define PMPACKET_HPP

#include "packet.hpp"


class PMPacket : public Packet
{
	public:
		unsigned int minSize = 5;
		enum fields {FROM = 3, TEXT};
		std::unordered_map<unsigned int, QString> fieldMap = {{Packet::fields::TYPE, "PM"}};
};

#endif // PMPACKET_HPP
