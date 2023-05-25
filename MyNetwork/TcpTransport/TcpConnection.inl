#ifndef TCP_CONNECTION_INL
#define TCP_CONNECTION_INL



namespace AzNetworking
{
	inline TcpSocket* TcpConnection::GetTcpSocket() const
	{
		return m_socket.get();
	}

	inline bool TcpConnection::IsOpen() const
	{
		return m_socket->IsOpen();
	}


	inline void TcpConnection::SetRegisteredSocketFd(SocketFd registeredSocketFd)
	{
		m_registeredSocketFd = registeredSocketFd;
	}

	inline SocketFd TcpConnection::GetRegisterSocketFd() const
	{
		return m_registeredSocketFd;
	}

}



#endif // !TCP_CONNECTION_INL


