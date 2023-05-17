#ifndef TCPSOCKET_MANAGER_H
#define TCPSOCKET_MANAGER_H

#include <vector>
#include "../Utilities/NetworkCommon.h"
#include <functional>


namespace AzNetworking
{
	class TcpSocketManager
	{
	public:
		using SocketEventCallBack = std::function<void(SocketFd)>;

		TcpSocketManager();

		bool AddSocket(SocketFd socketFd);

		bool ClearSocket(SocketFd socketFd);

		void ProcessEvents(AzNetworking::TimeMS maxBlockMS, const SocketEventCallBack& readCallBack, const SocketEventCallBack& writeCallBack);

	private:
		
		void AddSocketHelper(SocketFd socketFd);

		void ClearSocketHelper(SocketFd socketFd);

		AZ_DISABLE_COPY_MOVE(TcpSocketManager);

#if AZ_TRAIT_USE_SOCKET_SERVER_EPOLL
		SocketFd m_epollFd = InvalidSocketFd;
#elif AZ_TRAIT_USE_SOCKET_SERVER_SELECT
		fd_set m_sourceFdSet;
		fd_set m_readerFdSet;
		fd_set m_writerFdSet;
		SocketFd m_maxFd = SocketFd(0);

#endif // AZ_TRAIT_USE_SOCKET_SERVER_EPOLL

		std::vector<SocketFd> m_socketFds;

	};

	inline void TcpSocketManager::AddSocketHelper(SocketFd socketFd)
	{
		auto element = std::find(m_socketFds.begin(), m_socketFds.end(), socketFd);
		if (m_socketFds.end() == element)
		{
			m_socketFds.push_back(socketFd);
		}
	}

	inline void TcpSocketManager::ClearSocketHelper(SocketFd socketFd)
	{
		auto element = std::find(m_socketFds.begin(), m_socketFds.end(), socketFd);
		if (m_socketFds.end() != element)
		{
			m_socketFds.erase(element);
		}
	}

}



#endif // !TCPSOCKET_MANAGER_H
