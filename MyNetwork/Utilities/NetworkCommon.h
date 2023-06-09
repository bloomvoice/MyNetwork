#ifndef NETWORK_COMMON_H
#define NETWORK_COMMON_H

#include "TypeSafeIntegral.h"
#include "WinSock2.h"
#include <iostream>
#include <cstdint>

namespace AzNetworking
{

	AZ_TYPE_SAFE_INTEGRAL(SocketFd, int32_t);
	static constexpr SocketFd InvalidSocketFd = SocketFd(-1);

	AZ_TYPE_SAFE_INTEGRAL(PacketId, uint32_t);
	static constexpr PacketId InvalidPacketId = PacketId(0xFFFFFFFF);


	bool SetSocketNonBlocking(SocketFd socketFd);

	bool SetSocketNoDelay(SocketFd socketFd);

	void CloseSocket(SocketFd socketFd);

	static const int32_t SocketOpResultSuccess = 0;
	static const int32_t SocketOpResultError = -1;
	static const int32_t SocketOpResultDisconnected = -2;
	static const int32_t SocketOpResultNotOpen = -3;
	static const int32_t SocketOpResultNoSsl = -4;

	DisconnectReason GetDisconnectReasonForSocketResult(int32_t socketResult);

	AZ_TYPE_SAFE_INTEGRAL(TimeMS, int64_t);

	AZ_TYPE_SAFE_INTEGRAL(TimeUS, int64_t);

	namespace Time
	{
		static constexpr AzNetworking::TimeMS ZeroTimeMS = AzNetworking::TimeMS(0);
		static constexpr AzNetworking::TimeUS ZeroTimeUS = AzNetworking::TimeUS(0);
	}

	#define AZ_TRAIT_USE_SOCKET_SERVER_EPOLL 0;
	#define AZ_TRAIT_USE_SOCKET_SERVER_SELECT 1;


	#define AZ_TRAIT_COMPILER_INT64_T_IS_LONG 0

	typedef int64_t s64;
	typedef uint64_t u64;

	#define AZ_DISABLE_MOVE(_Class) \
	_Class(const _Class&&) = delete; _Class& operator =(const _Class &&) = delete;

	#define AZ_DISABLE_COPY_MOVE(_Class) \
	_Class(const _Class&) = delete; _Class& operator =(const _Class &) = delete; AZ_DISABLE_MOVE(_Class)


	const char* GetNetworkErrorDesc(int32_t errorCode);


#define aznumeric_cast static_cast

	int32_t GetLastNetworkError();

	bool ErrorIsWouldBlock(int32_t errorCode);

#define AZ_TYPE_INFO(MESSAGE, ...)

#define AZLOG_WARN(MESSAGE, ...)                                                                             \
	{                                                                                                            \
		/*AZ::ILogger* logger = AZ::Interface<AZ::ILogger>::Get();                                                 \
		if (logger != nullptr && AZ::LogLevel::Warn >= logger->GetLogLevel())                                    \
		{                                                                                                        \
        logger->LogInternal(AZ::LogLevel::Warn, MESSAGE, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);   \
		}  */                                                                                                      \
		std::cout<<"log warn:"<<MESSAGE<<std::endl;																		\
	}

#define AZLOG_ERROR(MESSAGE, ...)                                                                            \
{                                                                                                            \
    /*AZ::ILogger* logger = AZ::Interface<AZ::ILogger>::Get();                                                 \
    if (logger != nullptr && AZ::LogLevel::Error >= logger->GetLogLevel())                                   \
    {                                                                                                        \
        logger->LogInternal(AZ::LogLevel::Error, MESSAGE, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);  \
    }   */                                                                                                     \
	std::cout << "log error:" << MESSAGE << std::endl;															\
}

}

#endif // !NETWORK_COMMON_H

