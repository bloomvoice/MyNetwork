#ifndef ICONNECTION_H
#define ICONNECTION_H

#include "../Utilities/IpAddress.h"
#include "../Utilities/NetworkCommon.h"
#include "../Preprocessor/Enum.h"
#include "ConnectionMetrics.h"


namespace AzNetworking
{
	class IPacket;

	AZ_TYPE_SAFE_INTEGRAL(ConnectionId, uint32_t);
	static constexpr ConnectionId InvalidConnectionId = ConnectionId(0xFFFFFFFF);

	struct ConnectionQuality
	{
		ConnectionQuality() = default;
		ConnectionQuality(int32_t lossPercentage, TimeMS latencyMs, TimeMS varianceMs);

		int32_t m_lossPercentage = 0;
		TimeMS m_latenceyMs = AzNetworking::Time::ZeroTimeMS;
		TimeMS m_varianceMs = AzNetworking::Time::ZeroTimeMS;
	};

	enum class TrustZone
	{
		ExternalClientToServer,
		InternalClientToServer,
	};

	class IConenction
	{
	public:
		IConenction(ConnectionId connectionId, const IpAddress & address);
		virtual ~IConenction() = default;

		virtual bool SendReliablePacket(const IPacket& packet) = 0;

		virtual bool SendUnReliablePacket(const IPacket& packet) = 0;

		virtual bool WasPacketAcked(PacketId packetId)const = 0;

		virtual ConnectionState GetConnectionState() const = 0;

		virtual ConnectionRole GetConnectRole() const = 0;

		virtual bool Disconnect(DisconnectReason reason, TerminationEndpoint endpoint) = 0;

		virtual void SetConnectionMtu(uint32_t connectionMtu) = 0;

		virtual uint32_t GetConnectionMtu() const = 0;

		ConnectionId GetConnectionId() const;

		void SetUserData(void* userData);

		void* GetUserData() const;

		void SetRemoteAddress(const IpAddress& address);

		const IpAddress& GetRemoteAddress();

		ConnectionMetrics& GetMetrics();

		const ConnectionQuality& GetConnectionQuality() const;

		ConnectionQuality& GetConnectionQuality();


	private:

		ConnectionId m_connectionId = InvalidConnectionId;
		IpAddress m_remoteAddress;
		ConnectionMetrics m_connectionMetrics;
		ConnectionQuality m_connectionQuality;
		void* m_userdata = nullptr;

	};


}

#include "IConnection.inl"

#endif // !ICONNECTION_H
