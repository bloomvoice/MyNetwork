//#ifndef IPADRESS_INL
//#define IPADRESS_INL
#pragma once

namespace AzNetworking
{
	inline uint8_t IpAddress::GetQuadA() const
	{
		return uint8_t((m_ipv4Address >> 24) & 0xFF);
	}

	inline uint8_t IpAddress::GetQuadB() const
	{
		return uint8_t((m_ipv4Address >> 16) & 0xFF);
	}

	inline uint8_t IpAddress::GetQuadC() const
	{
		return uint8_t((m_ipv4Address >> 8) & 0xFF);
	}

	inline uint8_t IpAddress::GetQuadD() const
	{
		return uint8_t((m_ipv4Address) & 0xFF);
	}

	inline bool IpAddress::operator ==(const IpAddress& rhs)const
	{
		return (m_ipv4Address == rhs.m_ipv4Address) && (m_port == rhs.m_port);
	}

	inline bool IpAddress::operator !=(const IpAddress& rhs)const
	{
		return (m_ipv4Address != rhs.m_ipv4Address) || (m_port != rhs.m_port);
	}

	inline bool IpAddress::operator <(const IpAddress& rhs)const
	{
		if (m_ipv4Address == rhs.m_ipv4Address)
		{
			return m_port < rhs.m_port;
		}

		return m_ipv4Address < rhs.m_ipv4Address;
	}

	inline bool IpAddress::operator <= (const IpAddress& rhs)const
	{
		if (m_ipv4Address == rhs.m_ipv4Address)
		{
			return m_port <= rhs.m_port;
		}

		return m_ipv4Address <= rhs.m_ipv4Address;
	}

	inline bool IpAddress::operator > (const IpAddress& rhs)const
	{
		return !(*this <= rhs);
	}

	inline bool IpAddress::operator >= (const IpAddress& rhs)const
	{
		return !(*this < rhs);
	}

}


//#endif // !IPADRESS_INL

