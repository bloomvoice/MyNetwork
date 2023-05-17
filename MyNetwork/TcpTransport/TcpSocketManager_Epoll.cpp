#include "TcpSocketManager.h"

#if AZ_TRAIT_USE_SOCKET_SERVER_EPOLL
#include <sys/epoll.h>

namespace AzNetworking
{
	static constexpr uint32_t MaxEpollEvents = 256;

	TcpSocketManager::TcpSocketManager()
		:m_epollFd(static_cast<SocketFd>(epoll_createl(EPOLL_CLOEXEC)))
	{
		if (InvalidSocketFd == m_epollFd)
		{
			const int32_t error = GetLastNetworkError();
			AZLOG_ERROR("Failed to create epollFd, terminating application (%d:%s)", error, GetNetworkErrorDesc(error));
			//AZ_ASSERT(false, "Failed to create epollFd, terminating application");
			exit(EXIT_FAILURE);
		}
	}

	bool TcpSocketManager::AddSocket(SocketFd socketFd)
	{
		if (socketFd < SocketFd(0))
		{
			return false;
		}

		struct epoll_event fdEvents;
		fdEvents.events = EPOLLIN | EPOLLOUT | EPOLLET;
		fdEvents.data.fd = static_cast<int32_t>(socketFd);

		if (epoll_ctl(static_cast<int32_t>(m_epollFd), EPOLL_CTL_ADD, static_cast<int32_t>(socketFd), &fdEvents) < 0)
		{
			const int32_t error = GetLastNetworkError();
			AZLOG_ERROR("Call to epoll_ctl to bind socket failed (%d:%s)", error, GetNetworkErrorDesc(error));
			return false;
		}

		AddSocketHelper(socketFd);
		return true;
	}

	bool TcpSocketManager::ClearSocket(SocketFd socketFd)
	{
		ClearSocketHelper(socketFd);
		return true;
	}

	void TcpSocketManager::ProcessEvents(AzNetworking::TimeMS maxBlockMS, const SocketEventCallBack& readCallBack, const SocketEventCallBack& writeCallBack)
	{
		struct epoll_event socketEvent[MaxEpollEvents];
		const int32_t numEpollEvents = epoll_wait(static_cast<int32_t>(m_epollFd), socketEvent, MaxEpollEvents, -1);
		if (numEpollEvents < 0)
		{
			const int32_t error = GetLastNetworkError();
			AZLOG_ERROR("epoll_wait return an error (%d:%s)", error, GetNetworkErrorDesc(error));
			return;
		}

		if (numEpollEvents > 0)
		{
			for (int32_t event = 0; event < numEpollEvents; ++event)
			{
				const SocketFd socketFd = static_cast<SocketFd>(socketEvent[event].data.fd);
				if (socketEvent[event].events & EPOLLIN)
				{
					readCallBack(socketFd);
				}

				if (socketEvent[event].events & EPOLLOUT)
				{
					writeCallBack(socketFd);
				}
			}

		}

	}

	
}

#endif // AZ_TRAIT_USE_SOCKET_SERVER_EPOLL


