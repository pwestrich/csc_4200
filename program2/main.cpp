
#include <cstdlib>
#include <iostream>
#include <string>

#include "tictactoe.h"
#include "NetworkServer.h"
#include "NetworkClient.h"

using namespace std;

void playServer();
void playClient();
void playLocal();
const char *buildMoveMessage(const int move);

int main(const int argc, const char **argv){

	char input = '\0';

	menu:

	cout << "Menu                " << endl;
	cout << "--------------------" << endl;
	cout << "1. Start server     " << endl;
	cout << "2. Connect to server" << endl;
	cout << "3. Play locally     " << endl;
	cout << "Your choice:        ";
	cin >> input;
	cin.ignore(1000, '\n');

	switch (input){

		case '1': {

			playServer();
			break;

		} case '2': {

			playClient();
			break;

		} case '3': {

			playLocal();
			break;

		} default: {

			cout << "Invalid option: " << input << endl;
			goto menu;

		}

	}

	return 0;

}

void playServer(){

	//ask the user what port to listen on
	int port = 0;

	while (port == 0){

		cout << "What port would you like to listen on? ";
		cin >> port;

		if (port < 1024 || port > 32000){

			cout << "Invalid port number: " << port << endl;
			port = 0;

		}

	}

	try {

		NetworkServer *server = new NetworkServer(port);

		cout << "Awaiting connection..." << endl;

		//wait for a connection
		server->accept();

		cout << "Connection established." << endl;

		bool play = true;
		int input = -1;

		while (play){

			TicTacToe *game = new TicTacToe();

			while (true){

				game->printBoard();
				input = -1;

				//the server is always X and always goes first.
				while (input < 0 || input > 9){

					cout << "X, enter a play (1 - 9): ";
					cin >> input;

					if (!game->xMove(input - 1)){

						cout << "Invalid move. Try again." << endl;
						input = -1;

					}

				}

				//check for a win
				WinState win = game->isWin();

				if (win == X_WINS){

					server->sendMessage("WIN:X");
					game->printBoard();
					cout << "X wins!" << endl;
					break;

				} else if (win == O_WINS){

					server->sendMessage("WIN:O");
					game->printBoard();
					cout << "O wins!" << endl;
					break;

				} else if (win == TIE){

					server->sendMessage("WIN:T");
					game->printBoard();
					cout << "It's a tie!" << endl;
					break;

				} else {

					server->sendMessage(buildMoveMessage(input - 1));

				}

				cout << "Waiting on client..." << endl;

				//wait on client to send its move
				const char *message = server->recieveMessage();

				if (strncmp(message, "MOVE:", 5) == 0){

					//get the move from it, and make the move
					int move = atoi(message + 5);
					if (!game->oMove(move)){

						server->sendMessage("ERROR:Invalid move.");
						cerr << "Invalid move from the client: " << message << endl;
						exit(EXIT_FAILURE);

					}

				} else {

					server->sendMessage("ERROR:Invalid message.");
					cerr << "Invalid message: " << message << endl;
					exit(EXIT_FAILURE);

				}

				//check a win again
				win = game->isWin();

				if (win == X_WINS){

					server->sendMessage("WIN:X");
					game->printBoard();
					cout << "X wins!" << endl;
					break;

				} else if (win == O_WINS){

					server->sendMessage("WIN:O");
					game->printBoard();
					cout << "O wins!" << endl;
					break;

				} else if (win == TIE){

					server->sendMessage("WIN:T");
					game->printBoard();
					cout << "It's a tie!" << endl;
					break;

				}

			}

			delete game;

			char again;
			cout << "Would you like to play again? (y/n): ";
			cin >> again;

			//get the client message
			const char *message = server->recieveMessage();
			if ((strncmp(message, "AGAIN", 5) == 0) && (again == 'y' || again == 'Y')){

				server->sendMessage("AGAIN");

			} else {

				//close connection
				server->sendMessage("CLOSE");
				server->close();
				play = false;

			}

		}

		delete server;

	} catch (runtime_error *it){

		cerr << it->what() << endl;

	}

}

void playClient(){

	try {

		//ask the user for a port and host to connect to
		int port = 0;
		char *hostname = new char[64];
		memset(hostname, '\0', 64);

		cout << "What host would you like to connect to? ";
		cin.getline(hostname, '\n');
	
		while (port == 0){

			cout << "What port is the server listening on? ";
			cin >> port;

			if (port < 1024 || port > 32000){

				cout << "Invalid port number: " << port << endl;
				port = 0;

			}

		}

		NetworkClient *client = new NetworkClient(port, hostname);
		client->connect();

		bool play = true;
		int input = -1;

		while (play){

			TicTacToe *game = new TicTacToe();

			while (true){

				cout << "Waiting on server..." << endl;

				//wait for the server to make a move
				const char *message = client->recieveMessage();
				cerr << message << endl;
				if (strncmp(message, "MOVE:", 5) == 0){

					//get the move and make it
					int move = atoi(message + 5);
					game->xMove(move);

				} else if (strncmp(message, "WIN:T", 5) == 0) {

					game->printBoard();
					cout << "It's a tie!" << endl;
					break;

				} else if (strncmp(message, "WIN:", 4) == 0){

					game->printBoard();
					cout << message[4] << " wins!" << endl;
					break;

				} else {

					cerr << "Error: Invalid message received: " << message << endl;
					client->sendMessage("ERROR:Invalid command");
					exit(EXIT_FAILURE);

				}

				//now make your move
				game->printBoard();

				//the server is always X and always goes first.
				//the client is always O and goes second.
				while (input < 1 || input > 9){

					cout << "O, enter a play (1 - 9): ";
					cin >> input;

					if (!game->oMove(input - 1)){

						cout << "Invalid move. Try again." << endl;
						input = -1;

					}

				}

				//send it to the server
				client->sendMessage(buildMoveMessage(input - 1));
				input = -1;

			}

			delete game;

			//now ask to play again
			char again;
			cout << "Would you like to play again? (y/n): ";
			cin >> again;

			if (again == 'y' || again == 'Y'){

				client->sendMessage("AGAIN");

			} else {

				//close connection
				client->sendMessage("CLOSE");
				client->close();
				play = false;

			}

			//get the server response
			const char *message = client->recieveMessage();
			if (strncmp(message, "AGAIN", 5) != 0){

				play = false;

			}

		}

		delete client;
		delete hostname;

	} catch (runtime_error *it){

		cerr << it->what() << endl;

	} catch (invalid_argument *it){

		cerr << it->what() << endl;

	}

}

//play a local game
void playLocal(){

	TicTacToe *game = new TicTacToe();
	int input = -1;

	while (true){

		//print board and ask X to play
		game->printBoard();

		while (input < 0 || input > 9){

			cout << "X, enter a play (1 - 9): ";
			cin >> input;

			if (!game->xMove(input - 1)){

				cout << "Invalid move. Try again." << endl;
				input = -1;

			}

		}

		input = -1;
		//check for a win
		WinState win = game->isWin();

		if (win == X_WINS){

			game->printBoard();
			cout << "X wins!" << endl;
			break;

		} else if (win == O_WINS){

			game->printBoard();
			cout << "O wins!" << endl;
			break;

		} else if (win == TIE){

			game->printBoard();
			cout << "It's a tie!" << endl;
			break;

		}

		//print board and ask O to move
		game->printBoard();

		while (input < 0 || input > 9){

			cout << "O, enter a play (1 - 9): ";
			cin >> input;

			if (!game->oMove(input - 1)){

				cout << "Invalid move. Try again." << endl;
				input = -1;

			}

		}

		input = -1;
		//check for a winin again
		win = game->isWin();

		if (win == X_WINS){

			game->printBoard();
			cout << "X wins!" << endl;
			break;

		} else if (win == O_WINS){

			game->printBoard();
			cout << "O wins!" << endl;
			break;

		} else if (win == TIE){

			game->printBoard();
			cout << "It's a tie!" << endl;
			break;

		}		

	}

	//done
	delete game;

}

const char *buildMoveMessage(const int move){

	char *value = new char[8];

	strncpy(value, "MOVE:", 5);
	value[5] = '0' + move;
	value[6] = value[7] = '\0';

	return value;

}
