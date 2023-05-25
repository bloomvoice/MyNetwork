#ifndef CONNECTION_METRICS_H
#define CONNECTION_METRICS_H
#include "../Utilities/NetworkCommon.h"

class DataRateMetrics
{
public:
	DataRateMetrics();
	~DataRateMetrics();

private:

};

class ConnectionComputeRtt
{
public:
	ConnectionComputeRtt();
	~ConnectionComputeRtt();

private:

};


class ConnectionMetrics
{
	ConnectionMetrics() = default;

	ConnectionMetrics& operator=(const ConnectionMetrics& rhs) = default;

	void Reset();

	void LogPacketSent(uint32_t byteCount, AzNetworking::TimeMS);
	void LogPacketRecv(uint32_t byteCount, AzNetworking::TimeMS);
	void LogPacketLost();
	void LogPacketAcked();


	uint32_t m_packetsSent = 0;
	uint32_t m_packetsRecv = 0;
	uint32_t m_packetsLost = 0;
	uint32_t m_packetsAcked = 0;

	DataRateMetrics m_sendDatarate;
	DataRateMetrics m_recvDatarate;
	ConnectionComputeRtt m_connectionRtt;
};


#endif // !CONNECTION_METRICS_H


