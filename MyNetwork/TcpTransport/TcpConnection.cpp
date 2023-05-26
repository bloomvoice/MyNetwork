#include "TcpConnection.h"
#include "TcpNetworkInterface.h"

namespace AzNetworking
{
	TcpConnection::TcpConnection(ConnectionId connectionId,
		const IpAddress& ipaddress,
		TcpNetworkInterface& networkInterface,
		TcpSocket& socket)
		:IConnection(connectionId, ipaddress)
		,m_networkInterface(networkInterface)
		,m_socket(socket.CloneAndTakeOwnerShip())
		,m_state(m_socket->IsOpen() ? ConnectionState::Connected : ConnectionState::DisConnected)
		,m_connectRole(ConnectionRole::Acceptor)
		,m_registeredSocketFd(InvalidSocketFd)
	{

	}

	TcpConnection
	(
		ConnectionId connectionId,
		const IpAddress& ipaddress,
		TrustZone trustzone,
		bool useEncryption
	)
		:IConnection(connectionId, ipaddress)
		, m_networkInterface(networkInterface)
		, m_socket(nullptr)
		, m_state(ConnectionState::DisConnected)
		, m_connectRole(ConnectionRole::Acceptor)
		, m_registeredSocketFd(InvalidSocketFd)

	{
		if (useEncryption)
		{
			m_socket = std::make_unique<TlsSocket>(TrustZone);
		}
		else
		{
			m_socket = std::make_unique<TcpSocket>;
		}
	}

	TcpConnection::~TcpConnection()
	{
		if (ConnectionState::Connected == m_state)
		{
			m_state = ConnectionState::DisConnecting;
			m_networkInterface.GetConnectionListener().OnDisconnect(this, DisconnectReason::ConnectionDeleted, TerminationEndpoint::Local);
			m_state = ConnectionState::DisConnected;
		}
	}

	TcpConnection::Connect(uint16_t localPort)
	{
		Disconnect(DisconnectReason::TerminateByClient, TerminationEndpoint::Local);
		if (!m_socket->Connect(GetRemoteAddress(), localPort))
		{
			m_networkInterface.GetConnectionListener().OnDisconnect(this, DisconnectReason::ConnectionRejected, TerminationEndpoint::Local);
			return false;
		}
		m_state = ConnectionState::Connecting;
		SendReliablePacket(InitiateConnectionPacket());
	}

	bool TcpConnection::Disconnect(DisconnectReason reason, TerminationEndpoint endpoint)
	{
		if (ConnectionState::DisConnected == m_state)
		{
			return true;
		}

		if (ConnectionState::DisConnecting == m_state)
		{
			AZLOG_WARN("Disconnecting a connection already");
			return false;
		}

		m_state = ConnectionState::DisConnecting;
		m_networkInterface.GetConnectionListener().OnDisconnect(this, reason, endpoint);
		m_networkInterface.RequestDisconnect(this, reason);

		m_state = ConnectionState::DisConnected;
		//GetMetrics().Reset();

		return true;
	}

	void TcpConnection::SetConnectionMtu(uint32_t connectionMtu)
	{
		;
	}

	void TcpConnection::GetConnectionMtu() const
	{
		return 0;
	}

	bool TcpConnection::SendPacketInternal(PacketType packetType, TcpPacketEncodingBuffer& payLoadBuffer, AzNetworking::TimeMS currentTimeMs)
	{
		assert(payLoadBuffer.GetCapacity() < std::numeric_limits<uint16_t>::max());

		int32_t payLoadSize = static_cast<int32_t>(payLoadBuffer.GetSize());

		TcpPacketEncodingBuffer headerBuffer;
		{
			TcpPacketHeader header(packetType, static_cast<int16_t>(payLoadSize));
			header.SetPacketFlag(PacketFlag::NotCompressed, false);

			NetworkInputSerializer serializer(headerBuffer.GetBuffer(), static_cast<uint32_t>(headerBuffer.GetCapacity()));
			if (!header.Serialize(serializer))
			{
				return false;
			}

			headerBuffer.Resize(serializer.GetSize());
		}

		//ÔÝ²»Ñ¹Ëõ
		const uint8_t* srcData = payLoadBuffer.GetBuffer();

		const uint16_t headerSize = headerBuffer.GetSize();
		uint8_t* dstData = m_sendRingbuffer.ReserveBlockForWrite(headerSize + payLoadSize);

		if (nullptr == dstData)
		{
			AZLOG_ERROR("sending ringbuffer full,drop packet");
			return false;
		}

		{
			memcpy(dstData, headerBuffer.GetBuffer(), headerSize);
		}

		{
			memcpy(dstData + headerSize, srcData, payLoadSize);
		}

		m_sendRingbuffer.AdvacneWriteBuffer(headerSize + payLoadSize);
		GetMetrics().LogPacketSent(headerSize + payLoadSize, currentTimeMs);

		++m_networkInterface.GetMetrics().m_sendPacket;

		UpdateSend();
		return true;
	}

	bool TcpConnection::ReceivePacketInternal(TcpPacketHeader& outHeader, TcpPacketEncodingBuffer& outBuffer, AzNetworking::TimeMS currentMs)
	{
		NetworkOutputSerializer serializer(m_recvRingbuffer.GetReadBufferData(), m_recvRingbuffer.GetReadBufferSize());
		if (!outHeader.Serialize(serializer))
		{
			return false;
		}

		uint16_t packetSize = outHeader.GetPacketSize();
		const uint32_t unreadSize = serializer.GetUnReadSize();
		if (packetSize > unreadSize)
		{
			AZLOG_ERROR("packet size not match");
			return false;
		}

		if (packetSize > outBuffer.GetCapacity())
		{
			AZLOG_ERROR("packet length too long");
			return false;
		}

		const uint16_t transmittedPacketSize = packetSize;
		const uint8_t* srcData = serializer.GetUnreadData();

		outBuffer.Resize(packetSize);
		uint8_t* dstData = outBuffer.GetBuffer();
		memcpy(dstData, srcData, packetSize);

		m_recvRingbuffer.AdvanceReadBuffer(serializer.GetReadSize() + transmittedPacketSize);
		GetMetrics().LogPacketRecv(packetSize, currentMs);
		++m_networkInterface.GetMetrics().m_recvPacket;

		return true;

	}
}