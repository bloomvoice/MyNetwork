#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "../ConnectionLayer/IConnection.h"
#include "TcpSocket.h"
#include "../DataStructures/ByteBuffer.h"
#include "TcpPacketHeader.h"

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

		bool ReceivePacketInternal(TcpPacketHeader& outHeader, TcpPacketEncodingBuffer& outBuffer, AzNetworking::TimeMS currentMs);

		bool DecompressPacket(const uint8_t* packetBuffer, std::size_t packetSize, TcpPacketEncodingBuffer& packetBufferOut);

		TcpConnection& operator=(const TcpConnection&) = delete;

		TcpNetworkInterface& m_networkInterface;

		std::unique_ptr<TcpSocket> m_socket;
		std::unique_ptr<ICompressor> m_socket;

		PacketId m_lastSentPacketId = InvalidPacketId;

		ConnectionState m_state = ConnectionState::DisConnected;
		ConnectionRole m_connectRole = ConnectionRole::Connector;
		SocketFd m_registeredSocketFd;

		static const uint32_t SendRingbufferSize = 1024 * 1024;
		TcpRingBuffer<SendRingbufferSize> m_sendRingbuffer;

		static const uint32_t RecvRingbufferSize = 1024 * 1024;
		TcpRingBuffer<RecvRingbufferSize> m_sendRingbuffer;
	};


}


#include "TcpConnection.inl"

#endif // !TCP_CONNECTION_H


