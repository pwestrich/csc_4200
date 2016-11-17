
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "NetworkClient.h"

using namespace std;

NetworkClient::NetworkClient(const int _port, const char *serverAddress) : NetworkConnectionBase(_port){

	if (!serverAddress){

		throw new invalid_argument("Null server address");

	}

	serverHostname = gethostbyname(serverAddress);

	if (!serverHostname){

		throw new invalid_argument("Invalid hostname");

	}

	theirSockID = mySockID = socket(AF_INET, SOCK_STREAM, 0);

	if (mySockID < 0){

		throw new runtime_error("Error obtaining socket");

	}

	theirAddress->sin_family = AF_INET;
	theirAddress->sin_addr = *(in_addr*) serverHostname->h_addr;
	theirAddress->sin_port = htons(port);

}

NetworkClient::~NetworkClient(){

	close();

}

//tries to connect to a server
void NetworkClient::connect(){

	if ( ::connect(mySockID, (sockaddr*) theirAddress, sizeof(sockaddr_in)) < 0){

		throw new runtime_error("Error connecting to server.");

	}

}
