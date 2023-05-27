#ifndef TCP_PACKET_HEADER_H
#define TCP_PACKET_HEADER_H

#include "../PacketLayer/IPacketHeader.h"
#include "../PacketLayer/IPacket.h"

namespace AzNetworking
{
	inline TcpPacketHeader::TcpPacketHeader(PacketType packetType, uint16_t packetSize)
		:m_packetType(packetType)
		,m_packetSize(packetSize)
	{
		;
	}

	inline PacketType TcpPacketHeader::GetPacketType() const
	{
		return m_packetType;
	}

	inline PacketId TcpPacketHeader::GetPacketId() const
	{
		return InvalidPacketId;
	}

	inline uint16_t TcpPacketHeader::GetPacketSize() const
	{
		return m_packetSize;
	}

	inline bool TcpPacketHeader::IsPacketFlagSet(PacketFlag flag) const
	{
		return m_packetFlags.test(static_cast<uint32_t>(flag));
	}

	inline void TcpPacketHeader::SetPacketFlag(PacketFlag flag, bool value)
	{
		m_packetFlags.set(static_cast<uint32_t>(flag));
	}

}



#endif // !TCP_PACKET_HEADER_H


