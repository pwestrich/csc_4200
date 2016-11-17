
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_LEN 1024
#define FILENAME_LEN 64

using namespace std;

int main(int argc, char **argv){

	//check arguments
	if (argc <= 2){

		cout << "Usage: ./client <server_address> <server_port>" << endl;
		return 1;

	}

	//now bind to the port, and try to connect to the server
	int port = atoi(argv[2]);

	struct hostent *serverHostname = gethostbyname(argv[1]);

	if (!serverHostname){

		cerr << "Error: Invalid hostname." << endl;
		return 1;

	}

	int serverSocketID = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocketID <= 0){

		cerr << "Error obtaining a socket." << endl;
		return 1;

	}

	struct sockaddr_in serverAddress;
	memset((void*) &serverAddress, 0, sizeof serverAddress);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr = *((struct in_addr *) serverHostname->h_addr);
	serverAddress.sin_port = htons(port);

	if (connect(serverSocketID, (struct sockaddr*) &serverAddress, sizeof serverAddress) < 0){

		cerr << "Error connecting to server." << endl;
		return 1;

	}

	cout << "Connection established." << endl;

	char choice = '\0';
	char buffer[BUFFER_LEN]; //allocate this now
	memset(buffer, '\0', BUFFER_LEN);

	while (choice != '3'){

		cout << endl;
		cout << "               OPTIONS               "	<< endl;
		cout << "-------------------------------------"	<< endl;
		cout << "1. Send a file"          				<< endl;
		cout << "2. Retrieve a file"      				<< endl;
		cout << "3. Quit"                 				<< endl;
		cout << "-------------------------------------" << endl;
		cout << "What would you like to do? (1-3) >";
		cin >> choice;

		//do the appropriate action
		if (choice == '1'){ 

			//ask for a file, verify it exists
			char filename[FILENAME_LEN];
			memset(filename, '\0', FILENAME_LEN);

			cout << "What file would you like to send? >";
			cin >> filename;

			fstream inFile(filename, ios::in);

			if (!inFile){

				cout << "Error opening file." << endl;
				continue;

			}

			//send the request to the server
			strcpy(buffer, "STOR:");
			strcat(buffer + 5, filename);

			send(serverSocketID, buffer, strlen(buffer) + 1, 0);

			//there will be one of two responses: CTS:<filename> or ERR:<message>
			recv(serverSocketID, buffer, BUFFER_LEN - 1, 0);

			if (strncmp(buffer, "CTS:", 4) == 0){

				//start sending
				inFile.seekg(0, ios::end);
				char fileData[(int) inFile.tellg() + 1];
				memset(fileData, '\0', sizeof fileData);

				inFile.seekg(0, ios::beg);
				inFile.read(&fileData[0], sizeof fileData);
				inFile.close();

				char message[sizeof fileData + 5];
				strcpy(message, "CONT:");
				strncpy(message + 5, fileData, sizeof fileData);

				send(serverSocketID, message, strlen(message) + 1, 0);

			} else if (strncmp(buffer, "ERR:", 4) == 0){

				cout << "Error: " << buffer + 4 << endl;

			} else {

				cerr << buffer << endl;
				cout << "Error: Bad message recieved by server." << endl;
				send(serverSocketID, "ERR:Bad message recieved\0", 25, 0);

			}

			inFile.close();

		} else if (choice == '2'){

			//ask the user for a filename to get, then ask the server
			memset(buffer, '\0', sizeof buffer);
			strcpy(buffer, "RTRV:");
			cout << "What file would you like to get? ";
			cin >> (buffer + 5);

			send(serverSocketID, buffer, strlen(buffer), 0);

			//wait for answer
			int messageSize = recv(serverSocketID, buffer, sizeof buffer, 0);

			if (messageSize < 0){

				cerr << "Error: Bad message recieved." << endl;

			}

			//now check the message
			if (strncmp(buffer, "RTS:", 4) == 0){

				ofstream outfile(buffer + 4, ios::out);

				recv(serverSocketID, buffer, sizeof buffer, 0);

				outfile << (buffer + 5);
				outfile.close();

			} else if (strncmp(buffer, "ERR:", 4) == 0){

				cout << "Error: " << buffer + 4 << endl;

			} else {

				cout << "Error: Bad message recieved from server." << endl;
				send(serverSocketID, "ERR:Bad message recieved.\0", 25, 0);

			}

		} else if (choice == '3') {

			cout << "Exiting..." << endl;

			shutdown(serverSocketID, SHUT_RDWR);

			return 0;

		} else {

			cout << "Invalid option. Please try again." << endl;

		}

	}

}
