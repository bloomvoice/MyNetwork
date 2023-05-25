#ifndef ICONNECTION_INL
#define ICONNECTION_INL



namespace AzNetworking
{


	inline ConnectionQuality::ConnectionQuality(int32_t lossPercentage, AzNetworking::TimeMS latencyMs, AzNetworking::TimeMS varianceMs)
		:m_lossPercentage(lossPercentage)
		,m_latenceyMs(latencyMs)
		,m_varianceMs(varianceMs)
	{

	}


}

#endif // !ICONNECTION_INL
