#ifndef IPACKET_HEADER_H
#define IPACKET_HEADER_H

#include <memory>
#include "IPacket.h"
#include "../Utilities/NetworkCommon.h"
#include <bitset>

namespace AzNetworking
{
	enum class PacketDispatchResult
	{
		 Failure
		,Skipped
		,Success
	};

	enum class PacketFlag
	{
		Compressed
		,MAX
	};

	using PacketFlagBitset = std::bitset<8>;

	class IPacketHeader
	{
	public:
		AZ_TYPE_INFO(IPacket, "");

		virtual ~IPacketHeader() = default;

		virtual PacketType GetPacketType() const = 0;

		virtual PacketId GetPacketId() const = 0;

		virtual bool Serialize(ISerializer& serializer) = 0;

		virtual bool IsPacketFlagSet(PacketFlag flag) const = 0;

		virtual void SetPacketFlag(PacketFlag flag, bool value) = 0;


	};


}


#endif // !IPACKET_HEADER_H
