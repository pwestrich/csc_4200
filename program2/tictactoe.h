
#include <cstdint>
#include <iostream>

using namespace std;

enum WinState : uint8_t { NO_WIN, X_WINS, O_WINS, TIE };

class TicTacToe {

private:

	char *board;
	int turnCount;

public:

	TicTacToe();
	~TicTacToe();

	//make a move on the board 
	//returns false if the space is taken
	bool xMove(const int index);
	bool oMove(const int index);

	//checks for a win, return the current state of the game
	WinState isWin();

	void printBoard();

};
