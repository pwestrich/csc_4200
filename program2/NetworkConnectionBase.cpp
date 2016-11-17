
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "NetworkConnectionBase.h"

using namespace std;

const int NetworkConnectionBase::bufferSize = 1024;

NetworkConnectionBase::NetworkConnectionBase(const int _port){

//check incoming port
	if (_port < 1024 || port > 32000){

		throw new invalid_argument("Invalid port number");

	}

	port = _port;

	//set addresses to zero
	mySockID = 0;
	theirSockID = 0;
	mySockLength = 0;
	theirSockLength = 0;

	myAddress 	 = new sockaddr_in;
	theirAddress = new sockaddr_in;

	memset(myAddress, 0, sizeof(sockaddr_in));
	memset(theirAddress, 0, sizeof(sockaddr_in));

	buffer = new char[NetworkConnectionBase::bufferSize];

	open = false;

}


NetworkConnectionBase::~NetworkConnectionBase(){

	delete buffer;
	delete myAddress;
	delete theirAddress;

}

void NetworkConnectionBase::close(){

	if (open){

		shutdown(theirSockID, 2);
		open = false;

	}

}

//these methods assume that the child class has set up the addresses correctly.
void NetworkConnectionBase::sendMessage(const char *message){

	if (message == NULL){

		throw new invalid_argument("Null argument");

	}

	int numBytesSent = send(theirSockID, message, strlen(message), 0);

	if (numBytesSent < 0){

		throw new runtime_error(strerror(errno));

	}

}

const char *NetworkConnectionBase::recieveMessage(){

	memset(buffer, '\0', NetworkConnectionBase::bufferSize);

	int numByesRecieved = recv(theirSockID, buffer, NetworkConnectionBase::bufferSize, 0);

	if (numByesRecieved < 0){

		throw new runtime_error(strerror(errno));

	}

	return buffer;

}
