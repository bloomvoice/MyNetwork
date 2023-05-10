#include <cassert>
#include "TcpSocket.h"

namespace AzNetworking
{
	TcpSocket::TcpSocket()
		: m_socketFd(InvalidSocketFd)
	{

	}


	TcpSocket::TcpSocket(SocketFd socketFd)
		: m_socketFd(socketFd)
	{
		if (InvalidSocketFd != m_socketFd)
		{
			if (!(SetSocketNonBlocking(socketFd) && SetSocketNoDelay(socketFd)))
			{
				Close();
			}
		}
	}

	TcpSocket::~TcpSocket()
	{
		Close();
	}

	bool TcpSocket::IsEncrypt() const
	{
		return false;
	}

	TcpSocket* TcpSocket::CloneAndTakeOwnerShip()
	{
		TcpSocket* result = new TcpSocket(m_socketFd);
		m_socketFd = InvalidSocketFd;
		return result;
	}

	bool TcpSocket::Listen(uint16_t wPort)
	{
		Close();

		if (!SocketCreateInternal() || !BindSocketForListenInternal(wPort) 
			|| !(SetSocketNonBlocking(m_socketFd) && SetSocketNoDelay(m_socketFd)))
		{
			Close();
			return false;
		}

		return true;
	}

	bool TcpSocket::Connect(const IpAddress& address, uint16_t  localPort)
	{
		Close();

		if (!SocketCreateInternal() || !BindSocketForConnectInternal(address, localPort)
			|| !(SetSocketNonBlocking(m_socketFd) && SetSocketNoDelay(m_socketFd)))
		{
			Close();
			return false;
		}

		return true;
	}

	void TcpSocket::Close()
	{
		CloseSocket(m_socketFd);
		m_socketFd = InvalidSocketFd;
	}

	int32_t TcpSocket::Send(const uint8_t* data, uint32_t size) const
	{
		assert(size > 0);
		assert(nullptr != data);
		if (!IsOpen())
		{
			return SocketOpResultNotOpen;
		}

		return SendInternal(data, size);
	}

	int32_t TcpSocket::Receive(uint8_t* outData, uint32_t size) const
	{
		assert(size > 0);
		assert(nullptr != outData);
		if (!IsOpen())
		{
			return SocketOpResultNotOpen;
		}

		return ReceiveInternal(outData, size);
	}

	int32_t TcpSocket::SendInternal(const uint8_t* data, uint32_t size) const
	{
		const int32_t dSendBytes = send(aznumeric_cast<int32_t>(m_socketFd), (const char*)data, size, 0);

		if (dSendBytes < 0)
		{
			const int32_t error = GetLastNetworkError();

			if (ErrorIsWouldBlock(error))
			{
				return 0;
			}

			AZLOG_WARN("Failed to write to socket (%d:%s)"/*, error, GetNetworkErrorDesc(error)*/);

		}

		return dSendBytes;
	}

	int32_t TcpSocket::ReceiveInternal(uint8_t* outData, uint32_t size) const
	{
		const int32_t dReceiveBytes = recv(aznumeric_cast<int32_t>(m_socketFd), (char*)outData, size, 0);

		if (dReceiveBytes < 0)
		{
			const int32_t error = GetLastNetworkError();

			if (ErrorIsWouldBlock(error))
			{
				return 0;
			}

			AZLOG_WARN("Failed to read from socket (%d:%s)"/*, error, GetNetworkErrorDesc(error)*/);
		}
		else if (dReceiveBytes == 0)
		{
			// Clean disconnect, force the endpoint to disconnect and cleanup
			return SocketOpResultDisconnected;
		}

		return dReceiveBytes;
	}


	bool TcpSocket::BindSocketForListenInternal(uint16_t wPort)
	{
		sockaddr_in hints;
		memset(&hints, 0, sizeof(hints));
		hints.sin_family = AF_INET;
		hints.sin_addr.s_addr = INADDR_ANY;
		hints.sin_port = htons(wPort);

		if (::bind(aznumeric_cast<int32_t>(m_socketFd), (const sockaddr*)&hints, sizeof(hints)) != 0)
		{
			const int32_t error = GetLastNetworkError();
			AZLOG_WARN("Failed to bind TCP socket to port %u (%d:%s)"/*, uint32_t(port), error, GetNetworkErrorDesc(error)*/);
			return false;
		}

		if (::listen(aznumeric_cast<int32_t>(m_socketFd), SOMAXCONN) != 0)
		{
			const int32_t error = GetLastNetworkError();
			AZLOG_WARN("Failed to bind TCP socket to port %u (%d:%s)"/*, uint32_t(port), error, GetNetworkErrorDesc(error)*/);
			return false;
		}

		return true;
	}

	bool TcpSocket::BindSocketForConnectInternal(const IpAddress& address, uint16_t wPort)
	{
		if (wPort != 0)
		{
			sockaddr_in hints;
			memset(&hints, 0, sizeof(hints));
			hints.sin_family = AF_INET;
			hints.sin_port = htons(wPort);

			if (::bind(aznumeric_cast<int32_t>(m_socketFd), (const sockaddr*)&hints, sizeof(hints)) != 0)
			{
				const int32_t error = GetLastNetworkError();
				AZLOG_WARN("Failed to bind TCP socket to port %u (%d:%s)"/*, uint32_t(port), error, GetNetworkErrorDesc(error)*/);
				return false;
			}
		}


		{
			sockaddr_in dest;
			memset(&dest, 0, sizeof(dest));
			dest.sin_family = AF_INET;
			dest.sin_addr.s_addr = address.GetAddress(ByteOrder::Network);
			dest.sin_port = htons(address.GetPort(ByteOrder::Network));
			if (::connect(aznumeric_cast<int32_t>(m_socketFd), (const sockaddr*)&dest, sizeof(dest)) != 0)
			{
				const int32_t error = GetLastNetworkError();
				AZLOG_WARN("Failed to connect to remote endpoint %u (%d:%s)"/*, uint32_t(port), error, GetNetworkErrorDesc(error)*/);
				return false;
			}
		}

		return true;
	}

	bool TcpSocket::SocketCreateInternal()
	{
		assert(!IsOpen(), "Open called on an active socket");

		if (!IsOpen())
		{
			return false;
		}

		{
			m_socketFd = (SocketFd)::socket(AF_INET, SOCK_STREAM, 0);

			if (!IsOpen())
			{
				const int32_t error = GetLastNetworkError();
				AZLOG_WARN("Failed to create socket (%d:%s)", error, GetNetworkErrorDesc(error));
				m_socketFd = InvalidSocketFd;
				return false;
			}
		}

		return true;
	}
}
