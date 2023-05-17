#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

#include <cstdint>
#include <string>

namespace AzNetworking
{
	class ISerializer;

	enum class ByteOrder
	{
		Host
	,	Network
	};

	enum class ProtocolType
	{
		TCP
	,	UDP
	};

	class IpAddress
	{

	public:
		static constexpr uint32_t MaxIpStringLength = 32;
		using IpString = std::string;

		IpAddress() = default;

		IpAddress(const char* hostname, const char* service, ProtocolType type);

		IpAddress(const char* hostname, uint16_t port, ProtocolType type);

		IpAddress(uint8_t quadA, uint8_t quadB, uint8_t quadC, uint8_t quadD, uint16_t port);

		IpAddress(ByteOrder byteOrder, uint32_t address, uint16_t port);

		virtual ~IpAddress();

		IpString GetString() const;

		IpString GetIpString() const;

		uint8_t GetQuadA() const;
		uint8_t GetQuadB() const;
		uint8_t GetQuadC() const;
		uint8_t GetQuadD() const;

		bool operator ==(const IpAddress& rhs)const;
		bool operator !=(const IpAddress& rhs)const;
		bool operator <(const IpAddress& rhs)const;
		bool operator <= (const IpAddress & rhs)const;
		bool operator > (const IpAddress& rhs)const;
		bool operator >= (const IpAddress& rhs)const;

		bool Serialize(ISerializer& serialize);

		uint32_t GetAddress(ByteOrder byteOrder) const;

		uint16_t GetPort(ByteOrder byteOrder) const;
	private:
		uint32_t m_ipv4Address = 0;
		uint16_t m_port = 0;
	};

}

#include "IpAddress.inl"

#endif // !IP_ADDRESS_H


