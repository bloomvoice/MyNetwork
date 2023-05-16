#include "TcpSocketManager.h"

#if AZ_TRAIT_USE_SOCKET_SERVER_SELECT

namespace AzNetworking
{
	TcpSocketManager::TcpSocketManager()
	{
		FD_ZERO(&m_sourceFdSet);
		FD_ZERO(&m_readerFdSet);
		FD_ZERO(&m_writerFdSet);
	}

	bool TcpSocketManager::AddSocket(SocketFd socketFd)
	{
		if (socketFd < SocketFd(0))
		{
			return false;
		}

		FD_SET(static_cast<int32_t>(socketFd), &m_sourceFdSet);
		m_maxFd = std::max<SocketFd>(m_maxFd, socketFd);

		AddSocketHelper(socketFd);

		return true;

	}
}

#endif // AZ_TRAIT_USE_SOCKET_SERVER_SELECT


