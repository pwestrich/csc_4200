
#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "NetworkConnectionBase.h"

struct hostent;

class NetworkClient : public NetworkConnectionBase {

private:

	hostent *serverHostname;

public:

	NetworkClient(const int _port, const char *serverAddress);
	~NetworkClient();

	//tries to connect to a server
	void connect();

};

#endif
