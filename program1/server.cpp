
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_LEN 1024

using namespace std;

//function declarations
void handleConnection(int clientSockID);

int main(int argc, char **argv){

	//don't segfault
	if (argc <= 1){

		cout << "Usage: ./server <port_number>" << endl;
		return 0;

	}

	int listenPort = atoi(argv[1]);

	//try to open a socket
	int listenSockID = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSockID < 0){

		cerr << "Error: Could not open socket." << endl;
		return 1;

	}

	//initalize the server address
	struct sockaddr_in serverAddress;
	memset((void*) &serverAddress, 0, sizeof serverAddress);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(listenPort);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	//try to bind
	if (bind(listenSockID, (struct sockaddr*) &serverAddress, sizeof serverAddress) < 0){

		cerr << "Could not bind: " << strerror(errno) << endl;
		return 1;

	}

	//now listen on the newly made socket
	listen(listenSockID, 5);

	cout << "Server listening on port " << listenPort << endl;

	//now wait for an incoming connection
	while (true){
		
		//accept an incoming connection
		struct sockaddr_in clientAddress;
		unsigned int clientAddrLength = sizeof clientAddress;

		int clientSockID = accept(listenSockID, (struct sockaddr*) &clientAddress, &clientAddrLength);

		if (clientSockID < 0){

			cerr << "Error acception connection." << endl;
			continue;

		}

		//handle the connection
		cout << "Connection established." << endl;

		handleConnection(clientSockID);

	}

	return 0;

}

void handleConnection(int clientSockID){

	//we've recieved a connection, now serve it
	//first, set up a buffer to send and recieve messages with
	char buffer[BUFFER_LEN];

	//now wait for a message
	while (true){

		//try to recieve a message, and check error stuff
		int messageSize = 0;
		memset(buffer, '\0', BUFFER_LEN);

		messageSize = recv(clientSockID, buffer, BUFFER_LEN - 1, 0);

		if (messageSize < 0){

			cerr << "Error recieving message: " << strerror(errno) << endl;
			continue;

		} else if (messageSize == 0){

			cerr << "Clent closed connection." << endl;
			shutdown(clientSockID, SHUT_RDWR);
			return;

		}

		//now interptet the command 
		if (strncmp(buffer, "STOR:", 5) == 0){

			//get ready to recieve a file
			//check if file exists first, then send appropriate message
			cout << "Opening file: " << buffer + 5 << endl;

			//check if file can be written to
			fstream outFile(buffer + 5, ios::out);

			if (outFile.eof()){

				cerr << "Error: File cannot be written: " << (buffer + 5) << endl;
				send(clientSockID, "ERR:File cannot be written.\0", 28, 0);
				continue;

			} else if (!outFile.is_open()){

				cerr << "Cannot open file: " << strerror(errno) << endl;
				send(clientSockID, "ERR:File cannot be opened.\0", 26, 0);
				outFile.close();
				continue;

			} else { //send the ready message

				char readyMessage[4 + messageSize];
				memset(readyMessage, '\0', 4 + messageSize);

				strcpy(readyMessage, "CTS:");
				strcpy(readyMessage + 4, buffer + 5);

				send(clientSockID, readyMessage, strlen(readyMessage) + 1, 0);

			}

			//now recieve the file data

			memset(buffer, '\0', BUFFER_LEN);
			messageSize = recv(clientSockID, (void*) buffer, BUFFER_LEN - 1, 0);

			if (messageSize < 0){

				cerr << "Error recieving message." << endl;
                outFile.close();
				break;

			} else if (messageSize == 0){

				cerr << "Client unexpectedly closed connection." << endl;
				outFile.close();
				return;

			} 

			//close the file if a different message is recieved
			if (strncmp(buffer, "CONT:", 5) == 0){

				//else, write the contents of the message to the file
				cout << "Writing recieved data to file..." << endl;
				outFile << (buffer + 5);

			} else {

				cerr << "ERROR: Bad message recieved while attempting to write to file." << endl;

			}

			outFile.close();

		} else if (strncmp(buffer, "RTRV:", 5) == 0){

			//get ready to send a file
			//check if the file exists first, then send appropriate message
			fstream inFile(buffer + 5);

			if (!inFile){

				//send the error signal, the file could not be opened, or does not exist
				cerr << "Error: file " << buffer + 5 << " could not be opened for reading." << endl;

				char errorMessage[] = "ERR:The file could not be opened for reading.";
				send(clientSockID, errorMessage, strlen(errorMessage) + 1, 0);
				continue;

			}

			//otherwise, send the ready message, and start sending the file
			char readyMessage[strlen(buffer + 5) + 10];

			memset(readyMessage, '\0', sizeof readyMessage);
			strcpy(readyMessage, "RTS:");
			strcat(readyMessage, buffer + 5);

			send(clientSockID, readyMessage, strlen(readyMessage) + 1, 0);

			//read the entire file into a string
			inFile.seekg(0, ios::end);
			char fileData[(int) inFile.tellg() + 1];
			memset(fileData, '\0', sizeof fileData);

			inFile.seekg(0, ios::beg);
			inFile.read(&fileData[0], sizeof fileData);
			inFile.close();

			memset(buffer, '\0', sizeof buffer);
			strncpy(buffer, "CONT:", 5);
			strncpy(buffer + 5, fileData, strlen(fileData));

			//now send the data
			send(clientSockID, buffer, strlen(buffer) + 1, 0);

		} else if (strncmp(buffer, "ERR:", 4) == 0){

			//the client had an error
			//print it and move on with your life
			cout << "Error: " << (buffer + 4) << endl;

		} else {

			//a bad message was recieved
			//send an error message back and move on
			cerr << "Error: bad message recieved: " << buffer << endl;
			send(clientSockID, "ERR:Bad message recieved.\0", 27, 0);

		}

	}
	
}
