
#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include "NetworkConnectionBase.h"

class NetworkServer : public NetworkConnectionBase {

private:


public:

	NetworkServer(const int _port);
	~NetworkServer();

	//listens for a connection
	void accept();

};

#endif
