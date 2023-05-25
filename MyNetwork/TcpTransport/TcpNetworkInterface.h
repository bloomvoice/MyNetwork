#ifndef TCP_NETWORK_INTERFACE_H
#define TCP_NETWORK_INTERFACE_H

#include "../Framework/INetworkInterface.h"
#include <string>

namespace AzNetworking
{
	class TcpNetworkInterface final
		: public INetworkInterface
	{
	public:
		TcpNetworkInterface
		(
			const std::string &name,
			IConnectionListener& connectionListener,
			TrustZone trustzone,
			TcpListenThread& listenThread
		);




	};

}



#endif // !TCP_NETWORK_INTERFACE_H


