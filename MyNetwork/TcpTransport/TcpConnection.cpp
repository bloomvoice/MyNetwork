#include <chrono>
#include "TcpConnection.h"
#include "TcpNetworkInterface.h"
#include "../Utilities/NetworkCommon.h"
#include "../Serialization/NetworkInputSerializer.h"
#include "../Serialization/NetworkOutputSerializer.h"
#include "../ConnectionLayer/IConnection.h"

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

	TcpConnection(
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

	void TcpConnection::UpdateSend()
	{
		const uint32_t numSendBytes = m_sendRingbuffer.GetReadBufferSize();
		if (numSendBytes <= 0)
		{
			return;
		}

		uint32_t sendData = m_sendRingbuffer.GetReadBufferData();
		const int32_t sentBytes = m_socket->Send(sendData, numSendBytes);
		const DisconnectReason disconnectReason = GetDisconnectReasonForSocketResult(sentBytes);
		if (DisconnectReason::MAX != disconnectReason)
		{
			Disconnect(disconnectReason, TerminationEndpoint::Remote);
			return;
		}

		m_sendRingbuffer.AdvanceReadBuffer(sentBytes);
		m_networkInterface.GetMetrics().m_sendBytes += numSendBytes;
		
		if (m_socket->IsEncrypt() && sentBytes > 0)
		{
			m_networkInterface.GetMetrics().m_sendBytesEncryptionInflation += (static_cast<uint32_t>(sentBytes) - numSendBytes);
			++m_networkInterface.GetMetrics().m_sendPacketEncrpted;
		}
	}

	bool TcpConnection::UpdateRecv()
	{
		//log packet
		uint64_t startTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		{
			uint8_t* srcData = m_recvRingbuffer.ReserverBlockForWrite(MaxPacketSize);
			if (nullptr == srcData)
			{
				AZLOG_ERROR("receive ringbuffer full")
				Disconnect(DisconnectReason::StreamError, TerminationEndpoint::Local);
				return false;
			}

			const int32_t receiveBytes = m_socket->Receive(srcData, MaxPacketSize);
			if (0 ==receiveBytes)
			{
				return true;
			}

			const DisconnectReason disconnectReason = GetDisconnectReasonForSocketResult(receiveBytes);
			if (DisconnectReason::MAX != disconnectReason)
			{
				Disconnect(disconnectReason, TerminationEndpoint::Remote);
				return true;
			}

			m_recvRingbuffer.AdvanceWriteBuffer(receiveBytes);
			m_networkInterface.GetMetrics().m_recvBytes += receiveBytes;

		}
		for (;;)
		{
			TcpPacketHeader header(PacketType(0), 0);
			TcpPacketEncodingBuffer buffer;


			if (!ReceivePacketInternal(header ,buffer, 0))
			{
				break;
			}

			NetworkOutputSerializer serializer(buffer.GetBuffer(), static_cast<uint32_t>(buffer.GetSize()));
			if (ConnectionState::Connecting == m_state)
			{
				const ConnectResult connectResult = m_networkInterface.GetConnectionListener().ValidateConnect(GetRemoteAddress(), header, serializer);
				if (ConnectResult:Rejected == connectResult)
				{
					Disconnect(DisconnectReason::ConnectionRejected, TerminationEndpoint::Local);
				}
				else
				{
					m_state = ConnectionState::Connected;
				}
			}

			if (ConnectionState::Connected == m_state)
			{
				m_networkInterface.GetConnectionListener.OnPacketReceived(this, header, serializer);

			}

		}

		uint64_t curTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		m_networkInterface.GetMetrics().m_recvTimeMs += (curTime - startTime);

		return true;
	}

	bool TcpConnection::SendReliablePacket(const IPacket& packet)
	{
		TcpPacketEncodingBuffer buffer;
		{
			NetworkInputSerializer serializer(buffer.GetBuffer(), static_cast<uint32_t>(buffer.GetCapacity()));
			if (!const_cast<IPacket&>(packet).Serialize(serializer))
			{
				AZLOG_ERROR("unable to serialize reliable pakcet");
				return false;
			}
			buffer.Resize(serializer.GetSize());
		}

		uint64_t curTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		++m_lastSentPacketId;
		return SendPacketInternal(packet.GetPacketType(), buffer, curTime);
	}

	bool TcpConnection::SendUnReliablePacket(const IPacket& packet)
	{
		if (SendReliablePacket(packet))
		{
			return m_lastSentPacketId
		}

		return InvalidPacketId;
	}

	bool TcpConnection::WasPacketAcked(PacketId packetId) const
	{
		return !SequenceMoreRecent(packetId, m_lastSentPacketId);
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

	bool TcpConnection::SendPacketInternal(PacketType packetType, TcpPacketEncodingBuffer& payLoadBuffer,  uint64_t curTime)
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
		uint8_t* dstData = m_sendRingbuffer.ReserverBlockForWrite(headerSize + payLoadSize);

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
		GetMetrics().LogPacketSent(headerSize + payLoadSize, curTime);

		++m_networkInterface.GetMetrics().m_sendPacket;

		UpdateSend();
		return true;
	}

	bool TcpConnection::ReceivePacketInternal(TcpPacketHeader& outHeader, TcpPacketEncodingBuffer& outBuffer, uint64_t curTime)
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
		GetMetrics().LogPacketRecv(packetSize, curTime);
		++m_networkInterface.GetMetrics().m_recvPacket;

		return true;

	}
}