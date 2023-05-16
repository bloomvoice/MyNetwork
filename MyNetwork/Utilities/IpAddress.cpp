#pragma once


namespace AzNetworking
{
	inline uint8_t IpAddress::GetQuaA() const
	{
		return ((m_ipv4Address >> 24) & 0xFF);
	}

	inline uint8_t IpAddress::GetQuaB() const
	{
		return ((m_ipv4Address >> 16) & 0xFF);
	}

	inline uint8_t IpAddress::GetQuaC() const
	{
		return ((m_ipv4Address >> 8) & 0xFF);
	}

	inline uint8_t IpAddress::GetQuaD() const
	{
		return ((m_ipv4Address) & 0xFF);
	}

	inline bool IpAddress::operator==(const IpAddress& rhs) const
	{
		return (m_ipv4Address == rhs.m_ipv4Address) && (m_port == rhs.m_ipv4Address);
	}

	inline bool IpAddress::operator!=(const IpAddress& rhs) const
	{
		return (m_ipv4Address != rhs.m_ipv4Address) || (m_port != rhs.m_ipv4Address);
	}

	inline bool IpAddress::operator!=(const IpAddress& rhs) const
	{
		return (m_ipv4Address != rhs.m_ipv4Address) || (m_port != rhs.m_ipv4Address);
	}

	inline bool IpAddress::operator<(const IpAddress& rhs) const
	{
		if (m_ipv4Address == rhs.m_ipv4Address)
		{
			return m_port < rhs.m_ipv4Address;
		}
		return m_ipv4Address < rhs.m_ipv4Address;
	}

	inline bool IpAddress::operator<=(const IpAddress& rhs) const
	{
		if (m_ipv4Address == rhs.m_ipv4Address)
		{
			return m_port <= rhs.m_ipv4Address;
		}
		return m_ipv4Address <= rhs.m_ipv4Address;
	}

	inline bool IpAddress::operator>(const IpAddress& rhs) const
	{
		return !(*this <= rhs);
	}

	inline bool IpAddress::operator>=(const IpAddress& rhs) const
	{
		return !(*this < rhs);
	}
}

namespace AZStd
{
	template<>
	struct  hash<AzNetworking::IpAddress>
	{
		inline size_t operator()(const AzNetworking::IpAddress& key)const
		{
			const uint64_t address = key.GetAddress(AzNetworking::ByteOrder::Host);
			const uint64_t port = key.GetPort(AzNetworking::ByteOrder::Host);
			const uint64_t hashValue = (port << 32) | address;
			return AZStd::hash<uint64_t>()(hashValue);
		}
	};
}