#ifndef NETWORK_COMMON_H
#define NETWORK_COMMON_H

#include "TypeSafeIntegral.h"
#include "WinSock2.h"
#include <iostream>

namespace AzNetworking
{
	AZ_TYPE_SAFE_INTEGRAL(SocketFd, int32_t);
	static constexpr SocketFd InvalidSocketFd = SocketFd{ -1 };

	bool SetSocketNonBlocking(SocketFd socketFd);

	bool SetSocketNoDelay(SocketFd socketFd);

	void CloseSocket(SocketFd socketFd);

	static const int32_t SocketOpResultSuccess = 0;
	static const int32_t SocketOpResultError = -1;
	static const int32_t SocketOpResultDisconnected = -2;
	static const int32_t SocketOpResultNotOpen = -3;
	static const int32_t SocketOpResultNoSsl = -4;

	#define aznumeric_cast static_cast

	int32_t GetLastNetworkError();

	bool ErrorIsWouldBlock(int32_t errorCode);

	#define AZLOG_WARN(MESSAGE, ...)                                                                             \
	{                                                                                                            \
		/*AZ::ILogger* logger = AZ::Interface<AZ::ILogger>::Get();                                                 \
		if (logger != nullptr && AZ::LogLevel::Warn >= logger->GetLogLevel())                                    \
		{                                                                                                        \
        logger->LogInternal(AZ::LogLevel::Warn, MESSAGE, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);   \
		}  */                                                                                                      \
		std::cout<<"log warn:"<<MESSAGE<<endl;																		\
	}

}


#endif // !NETWORK_COMMON_H

