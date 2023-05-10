#include "NetworkCommon.h"

namespace AzNetworking
{

	namespace Platform
	{
		bool SetSocketNonBlocking(SocketFd socketFd)
		{
			DWORD nonBlocking = 1;

			if (ioctlsocket(int32_t(socketFd), FIONBIO, &nonBlocking) != 0 )
			{
				int32_t error = GetLastNetworkError();
				AZLOG_ERROR("Failed to set non-blocking for socket (%d:%s)"/*, error, GetNetworkErrorDesc(error)*/)
				return false;
			}

			return true;
		}

		void CloseSocket(SocketFd socketFd)
		{
			closesocket(int32_t(socketFd));
		}
	}


}


