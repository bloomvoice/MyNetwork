#ifndef ICONNECTION_H
#define ICONNECTION_H

#include "../Utilities/IpAddress.h"
#include "../Utilities/NetworkCommon.h"
#include "../Preprocessor/Enum.h"

namespace AzNetworking
{
	class IPacket;

	AZ_TYPE_SAFE_INTEGRAL(ConnectionId, uint32_t);
	static constexpr ConnectionId InvalidConnectionId = ConnectionId(0xFFFFFFFF);

	struct ConnectionQuality
	{
		ConnectionQuality() = default;
		ConnectionQuality(int32_t lossPercentage, AzNetworking::TimeMS latencyMs, AzNetworking::TimeMS varianceMs);

		int32_t m_lossPercentage = 0;
		AzNetworking::TimeMS m_latenceyMs = AzNetworking::Time::ZeroTimeMS;
		AzNetworking::TimeMS m_varianceMs = AzNetworking::Time::ZeroTimeMS;
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

		

	private:

	};

	IConenction::IConenction()
	{
	}

	IConenction::~IConenction()
	{
	}

}

#endif // !ICONNECTION_H
