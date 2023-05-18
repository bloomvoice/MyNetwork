#include "IpAddress.h"
#include "WinSock2.h"

namespace AzNetworking
{
	namespace Platform
	{
		void InitIpAddress(uint32_t& ipv4address, uint16_t port, const char* hostname, const char* service, ProtocolType type);
	}

	IpAddress::IpAddress(const char* hostname, const char* service, ProtocolType type)
	{
		Platform::InitIpAddress(m_ipv4Address, m_port, hostname, service, type);
	}

	IpAddress::IpAddress(const char* hostname, uint16_t port, ProtocolType type)
		: IpAddress(hostname, nullptr, type)
	{
		m_port = port;
	}

	IpAddress::IpAddress(uint8_t quadA, uint8_t quadB, uint8_t quadC, uint8_t quadD, uint16_t port)
		:m_ipv4Address((quadA) << 24 | (quadB) << 16 | (quadC) << 8 | quadD)
		, m_port(port)
	{

	}

	IpAddress::IpAddress(ByteOrder byteOrder, uint32_t address, uint16_t port)
		:m_ipv4Address(byteOrder == ByteOrder::Network ? ntohl(address) : address)
		, m_port(byteOrder == ByteOrder::Network ? ntohs(port) : port)
	{

	}

	IpAddress::~IpAddress()
	{

	}

	IpAddress::IpString IpAddress::GetString() const
	{
		return std::string();
	}

	IpAddress::IpString IpAddress::GetIpString() const
	{
		return std::string();
	}

	uint32_t IpAddress::GetAddress(ByteOrder byteOrder) const
	{
		return byteOrder == ByteOrder::Network ? htonl(m_ipv4Address) : m_ipv4Address;
	}

	uint16_t IpAddress::GetPort(ByteOrder byteOrder) const
	{
		return byteOrder == ByteOrder::Network ? htons(m_port) : m_port;
	}


	bool IpAddress::Serialize(ISerializer& serialize)
	{
		/*serialize.Serialize(m_ipv4Address, "Ipv4Address");
		serialize.Serialize(m_port, "Port");
		return serialize.IsValid();*/
		return true;
	}
}