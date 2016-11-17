README

CSC 4200 Program #3
Philip Westrich

1. Compiling

There is a makefile included. All you should have to do is type:

	./make
	./make run

2. Using

To use the program, first follow above build instructions, then follow the 
given prompts.

You may start a server, connect to a server, or play locally.

3. Protocol

The program uses a very simple protocol. Each program keeps its own local board.

The server is always player X, and X always goes first. Upon connection, the client waits for 
the first message from the server.

MESSAGE 		DESCRIPTION
---------------------------------------------------------------
MOVE:[0-8] 		The server or client sends the move that they have made. There shall be no invalid moves sent; moves should be verified before sending. The client will respond with another MOVE upon receipt, and the server will respond with either a WIN or MOVE.
WIN:[XOT] 		The player specified has won (O or X), or T if it is a tie. The server checks every turn for a winning move and sends it instead of a MOVE. When the client receives this, it will respond with either an AGAIN or CLOSE. The client will never send this.
AGAIN 			The server and client send this message to each other when determining to play again. The client will restart its game upon receipt if it wishes to continue, or respond with a CLOSE otherwise. The server will respond with either an AGAIN or CLOSE after receipt.
CLOSE 			Send in lieu of the AGAIN message when they do not wish to continue. The server and client will quit upon receipt.
ERROR:<message>	Sent when something bad happens, such as an invalid message. The server and client will print the message and quit upon receipt.
