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

	bool TcpSocketManager::ClearSocket(SocketFd socketFd)
	{
		FD_CLR(static_cast<int32_t>(socketFd), &m_sourceFdSet);
		ClearSocketHelper(socketFd);
		return true;
	}

	void TcpSocketManager::ProcessEvents(AzNetworking::TimeMS maxBlockMS, const SocketEventCallBack& readCallBack, const SocketEventCallBack& writeCallBack)
	{
		if (m_maxFd <= SocketFd(0) || m_socketFds.empty())
		{
			return;
		}

		m_readerFdSet = m_sourceFdSet;
		m_writerFdSet = m_sourceFdSet;

		struct timeval tv = { 0, static_cast<int32_t>(maxBlockMS) * 1000 };
		const int32_t selectResult = ::select(static_cast<int32_t>(m_maxFd) + 1, &m_readerFdSet, &m_writerFdSet, nullptr, &tv);
		if (selectResult < 0)
		{
			const int32_t error = GetLastNetworkError();
			AZLOG_ERROR("select returned an error(%d:%s)", error, GetNetworkErrorDesc(error));
		}


		for (auto socketFd : m_socketFds)
		{
			if (FD_ISSET(static_cast<int32_t>(socketFd), &m_readerFdSet))
			{
				readCallBack(socketFd);
			}

			if (FD_ISSET(static_cast<int32_t>(socketFd), &m_writerFdSet))
			{
				writeCallBack(socketFd);
			}
		}

	}


}

#endif // AZ_TRAIT_USE_SOCKET_SERVER_SELECT


