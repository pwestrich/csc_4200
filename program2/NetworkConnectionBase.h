
#ifndef NETOWRK_BASE
#define NETOWRK_BASE

using namespace std;

struct sockaddr_in;

class NetworkConnectionBase {

protected:

	const static int bufferSize;

	int port;
	int mySockID;
	int theirSockID;
	int mySockLength;
	int theirSockLength;
	bool open;
	char *buffer;

	sockaddr_in *myAddress;
	sockaddr_in *theirAddress;

public:

	NetworkConnectionBase(const int _port);
	virtual ~NetworkConnectionBase();

	void close();

	void sendMessage(const char *message);
	const char *recieveMessage();

};

#endif
