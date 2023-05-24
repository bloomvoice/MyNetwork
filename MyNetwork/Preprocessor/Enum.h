#ifndef CONNECTION_ENUM_H
#define CONNECTION_ENUM_h

#define AZ_ENUM enum class

AZ_ENUM ConnectionState
{
	 DisConnected
	,DisConnecting
	,Connected
	,Connecting
};

AZ_ENUM ConnectionRole
{
	 Connector
	,Acceptor
};

AZ_ENUM TerminationEndpoint
{
	 Local
	,Remote
};

AZ_ENUM DisconnectReason
{
	 None
	,UnKown
	,StreamError
	,NetworkError
	,Timeout
	,ConnectTimeOut
	,ConnectionRetry
	,HeartbeatTimeError
	,TransportError
	,TerminateByClient
	,TerminateByServer
	,TerminateByUser
	,TerminateByMultipleLogin
	,RemoteHostClosedConnection
	,ReliableTransportError
	,ReliableQueueFull
	,ConnectionRejected
	,ConnectionDeleted
	,ServerNoLevelLoaded//???
	,ServerError
	,ClientMigrated//???
	,SslFailure
	,VersionMismatch
	,NonceRejected//???
	,DtlsHandshakeError//???
	,MAX
};

#endif // !CONNECTION_ENUM_H
