#ifndef IPACKET_H
#define IPACKET_H

#include <memory>
#include "../Utilities/TypeSafeIntegral.h"
#include "../Utilities/NetworkCommon.h"

namespace AzNetworking
{
	class ISerializer;
	AZ_TYPE_SAFE_INTEGRAL(PacketType, uint16_t);

	class IPacket
	{
	public:
		AZ_TYPE_INFO(IPacket, "");

		virtual ~IPacket() = default;

		virtual PacketType GetPacketType() const = 0;

		virtual std::unique_ptr<IPacket> Clone() const = 0;

		virtual bool Serialize(ISerializer& serializer) = 0;



	private:

	};


}


#endif // !IPACKET_H
