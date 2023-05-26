#ifndef TCP_PACKET_HEADER_H
#define TCP_PACKET_HEADER_H

#include "../PacketLayer/IPacketHeader.h"
#include "../PacketLayer/IPacket.h"
#include "../Serialization/ISerializer.h"

namespace AzNetworking
{
	class TcpPacketHeader final
		: public IPacketHeader
	{
		TcpPacketHeader(PacketType packetType, uint16_t packetSize);

		virtual ~TcpPacketHeader() = default;

		PacketType GetPacketType() const override;

		PacketId GetPacketId() const override;

		bool IsPacketFlagSet(PacketFlag flag) const override;

		void SetPacketFlag(PacketFlag flag, bool value) override;

		uint16_t GetPacketSize() const;

		bool Serialize(ISerializer& serializer);


	private:
		PacketType m_packetType;

		uint16_t m_packetSize;

		PacketFlagBitset m_packetFlags;
	};

#include "TcpPacketHeader.inl"
}



#endif // !TCP_PACKET_HEADER_H


