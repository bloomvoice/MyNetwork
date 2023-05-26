#include "TcpPacketHeader.h"

namespace AzNetworking
{
	bool TcpPacketHeader::Serialize(ISerializer& serializer)
	{
		serializer.Serialize(m_packetFlags, "Flags");
		serializer.Serialize(m_packetType, "Type");
		serializer.Serialize(m_packetSize, "Size");
	}
}