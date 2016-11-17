
#include "tictactoe.h"

using namespace std;

TicTacToe::TicTacToe(){

	board = new char[10];
	memset(board, '-', 9);
	board[9] = '\0';

	turnCount = 0;

}

TicTacToe::~TicTacToe(){

	delete [] board;

}

//make a move on the board 
bool TicTacToe::xMove(const int index){

	if (index < 0 || index > 8){

		return false;

	} else if (board[index] == '-'){

		board[index] = 'X';
		turnCount++;
		return true;

	} else return false;

}

bool TicTacToe::oMove(const int index){

	if (index < 0 || index > 8){

		return false;

	} else if (board[index] == '-'){

		board[index] = 'O';
		turnCount++;
		return true;

	} else return false;

}

//checks for a win, return the current state of the game
WinState TicTacToe::isWin(){

	if ((board[0] == 'X' && board[0] == board[1] && board[1] == board[2])	//row 1
			|| (board[3] == 'X' && board[3] == board[4] && board[4] == board[5])	//row 2
			|| (board[6] == 'X' && board[6] == board[7] && board[7] == board[8])	//row 3
			|| (board[0] == 'X' && board[0] == board[3] && board[3] == board[6])	//col 1
			|| (board[1] == 'X' && board[1] == board[4] && board[4] == board[7])	//col 2
			|| (board[2] == 'X' && board[2] == board[5] && board[5] == board[8])	//col 3
			|| (board[0] == 'X' && board[0] == board[4] && board[4] == board[8])	//diagonal top left
			|| (board[2] == 'X' && board[2] == board[4] && board[4] == board[6])){  //diagonal top right

		return X_WINS;

	} else if ((board[0] == 'O' && board[0] == board[1] && board[1] == board[2])	//row 1
			|| (board[3] == 'O' && board[3] == board[4] && board[4] == board[5])	//row 2
			|| (board[6] == 'O' && board[6] == board[7] && board[7] == board[8])	//row 3
			|| (board[0] == 'O' && board[0] == board[3] && board[3] == board[6])	//col 1
			|| (board[1] == 'O' && board[1] == board[4] && board[4] == board[7])	//col 2
			|| (board[2] == 'O' && board[2] == board[5] && board[5] == board[8])	//col 3
			|| (board[0] == 'O' && board[0] == board[4] && board[4] == board[8])	//diagonal top left
			|| (board[2] == 'O' && board[2] == board[4] && board[4] == board[6])){  //diagonal top right

		return O_WINS;

	} else if (turnCount == 9){

		return TIE;

	} else return NO_WIN; //no one has won yet otherwise

}

void TicTacToe::printBoard(){

	cout << "  " << board[0] << " | " << board[1] << " | " << board[2] << endl;
	cout << "-------------" << endl;
	cout << "  " << board[3] << " | " << board[4] << " | " << board[5] << endl;
	cout << "-------------" << endl;
	cout << "  " << board[6] << " | " << board[7] << " | " << board[8] << endl;
	cout << endl;

}
