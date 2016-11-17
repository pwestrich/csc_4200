
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <cerrno>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "NetworkServer.h"

using namespace std;

NetworkServer::NetworkServer(const int _port) : NetworkConnectionBase(_port){

	mySockID = socket(AF_INET, SOCK_STREAM, 0);

	if (mySockID < 0){

		throw new runtime_error("Error obtaining socket");

	}

	myAddress->sin_family = AF_INET;
	myAddress->sin_port = htons(port);
	myAddress->sin_addr.s_addr = htonl(INADDR_ANY);

	if ( ::bind(mySockID, (sockaddr*) myAddress, sizeof(sockaddr_in)) < 0){

		cerr << "errno:   " << errno << ": " << strerror(errno) << endl;
		cerr << "port:    " << port << endl;
		cerr << "address: " << myAddress << endl;
		throw new runtime_error("Error binding");

	}

	//queue 5 connections
	listen(mySockID, 5);

}

NetworkServer::~NetworkServer(){

	close();

}

//listens for a connection
void NetworkServer::accept(){

	theirSockID = ::accept(mySockID, (struct sockaddr*) theirAddress, (socklen_t*) &theirSockLength);

	if (theirSockID < 0){

		throw new runtime_error("Error accepting connection");

	}

	open = true;

}
