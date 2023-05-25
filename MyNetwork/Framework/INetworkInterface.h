#ifndef INETWORKINTERFACE_H
#define INETWORKINTERFACE_H



namespace AzNetworking
{


	class INetworkInterface
	{
	public:
		INetworkInterface(ConnectionId connectionId, const IpAddress & address);
		virtual ~INetworkInterface() = default;

		


	private:



	};


}



#endif // !INETWORKINTERFACE_H
