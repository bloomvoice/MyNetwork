#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "../ConnectionLayer/IConnection.h"
#include "TcpSocket.h"
#include "../DataStructures/ByteBuffer.h"
#include "../PacketLayer/IPacketHeader.h"

namespace AzNetworking
{
	class TcpConnection final
		: public IConnection
	{
	public:
		TcpConnection
		(
			ConnectionId connectionId,
			const IpAddress& ipaddress,
			TcpNetworkInterface& networkInterface,
			TcpSocket& socket
		);

		TcpConnection
		(
			ConnectionId connectionId,
			const IpAddress& ipaddress,
			TrustZone trustzone,
			bool useEncryption
		);

		~TcpConnection() override;

		TcpSocket* GetTcpSocket() const;

		bool IsOpen() const;

		bool Connect(uint16_t localPort);

		void UpdateSend();

		void UpdateRecv();

		virtual bool SendReliablePacket(const IPacket& packet) override;

		virtual PacketId SendUnReliablePacket(const IPacket& packet) override;

		virtual bool WasPacketAcked(PacketId packetId) const override;

		virtual ConnectionState GetConnectionState() const override;

		virtual ConnectionRole GetConnectionRole() const override;

		virtual bool Disconnect(DisconnectReason reason, TerminationEndpoint endpoint) override;

		virtual void SetConnectionMtu(uint32_t connectionMtu);

		virtual	uint32_t GetConnectionMtu() const override;

		void SetRegisteredSocketFd(SocketFd RegisteredSocketFd);

		SocketFd GetRegisterSocketFd() const;

	private:

		bool SendPacketInternal(PacketType packetType, TcpPacketEncodingBuffer& payLoadBuffer, AzNetworking::TimeMS currentTimeMs);

		bool ReceivePacketInternal(TcpPacketHeader)
	};

}



#endif // !TCP_CONNECTION_H


