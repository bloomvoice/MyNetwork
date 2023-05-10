#ifndef TCPSOKCET_INL
#define TCPSOKCET_INL

namespace AzNetworking
{
	inline bool TcpSocket::IsOpen() const
	{
		return (m_socketFd > SocketFd(0));
	}

	inline void TcpSocket::SetSocketFd(SocketFd socketFd)
	{
		m_socketFd = socketFd;
	}

	inline SocketFd TcpSocket::GetSocketFd() const
	{
		return m_socketFd;
	}
}

#endif // !TCPSOKCET_INL
