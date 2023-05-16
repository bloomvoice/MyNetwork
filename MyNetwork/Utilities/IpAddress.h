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
		static constexpr uint32_t MaxIpStringLength = 32;
		using IpString = std::string/*AZStd::fixed_string<MaxIpStringLength>*/;

		uint32_t GetAddress(ByteOrder byteOrder) const;

		uint16_t GetPort(ByteOrder byteOrder) const;
	};

}

#endif // !IP_ADDRESS_H


