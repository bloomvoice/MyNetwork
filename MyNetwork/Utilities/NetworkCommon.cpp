#include "NetworkCommon.h"

namespace AzNetworking
{

	namespace Platform
	{
		bool SetSocketNonBlocking(SocketFd socketFd);
		void CloseSocket(SocketFd socketFd);
	}

	bool SetSocketNonBlocking(SocketFd socketFd)
	{
		return Platform::SetSocketNonBlocking(socketFd);
	}

	bool SetSocketNoDelay(SocketFd socketFd)
	{
		int flag = 1;
		if (setsockopt(int32_t(socketFd), IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag) != SocketOpResultSuccess))
		{
			int32_t error = GetLastNetworkError();
			AZLOG_ERROR("Failed to disable flow control for socket (%d:%s)"/*, error, GetNetworkErrorDesc(error)*/);
			return false;
		}

		return true;
	}

	void CloseSocket(SocketFd socketFd)
	{
		if (int32_t(socketFd) <= 0)
		{
			return;
		}

		Platform::CloseSocket(socketFd);
	}


	int32_t GetLastNetworkError()
	{
		return WSAGetLastError();
	}

	bool ErrorIsWouldBlock(int32_t errorCode)
	{
		return WSAEWOULDBLOCK == errorCode;
	}


}


