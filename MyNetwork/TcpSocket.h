#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <cstdint>
#include "Utilities/NetworkCommon.h"
#include "Utilities/IpAddress.h"

namespace AzNetworking
{

	class TcpSocket
	{
		TcpSocket();

		TcpSocket(SocketFd socketFd);

		virtual ~TcpSocket();

		virtual TcpSocket* CloneAndTakeOwnerShip();

		virtual bool IsEncrypt() const;

		virtual bool Listen(uint16_t wPort);

		virtual bool Connect(const IpAddress& address, uint16_t  localPort);

		virtual void Close();

		bool IsOpen() const;

		void SetSocketFd(SocketFd socketFd);

		SocketFd GetSokcetFd() const;

		int32_t Send(const uint8_t* data, uint32_t size) const;


		int32_t Receive(uint8_t* outData, uint32_t size) const;

	protected:
		
		virtual int32_t SendInternal(const uint8_t* data, uint32_t size) const;
		virtual int32_t ReceiveInternal(uint8_t* outData, uint32_t size) const;

		bool BindSocketForListenInternal(uint16_t wPort);
		bool BindSocketForConnectInternal(const IpAddress & address, uint16_t wPort);
		bool SocketCreateInternal();

		SocketFd m_socketFd;

	};
}

#endif // !TCP_SOCKET_H

