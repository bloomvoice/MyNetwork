#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

#include <cstdint>

namespace AzNetworking
{
	enum class ByteOrder
	{
		Host
		, Network
	};

	class IpAddress
	{

	public:
		uint32_t GetAddress(ByteOrder byteOrder) const;

		uint16_t GetPort(ByteOrder byteOrder) const;
	};

}

#endif // !IP_ADDRESS_H


