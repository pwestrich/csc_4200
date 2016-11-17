
CSC 4200 Program #1 README

by Philip Westrich


1. Contents

In the zip file, there should be four files:

	1. This readme
	2. A script to build both programs
	3. The server source
	4. The client source

2. Compiling

To compile the programs, simply run the build script.

3. Usage

First, you must start the server. You can do so by typing:

	./server <port>

where <port is the port to listen on. The server will print messages
every once in a while. They can be ignored, probably.

Once the server is running, you may start the client by typing:

	./client <host> <port>

where <host> is the host the server is running on
and <port> is the port said server is listening on.

The client is pretty self-explanatory. You have three options:

	1. Send a file
	2. Retrieve a file
	3. Quit

To use a command, type its number and hit return. The first two commands
will prompt you for a file to send or retrieve, respectivley.

4. Assumptions

Several assumptions were made while writing the program, as the assignment
specification allows for it on line three. They are as follows:

	1. Transmission of text files only
	2. The contents of said file will fit in one packet
	3. The server and client run in pairs; do not mix-and-match, for correct
		function cannot be guranteed

5. Errors

Errors should be self-explanitory; read what the program tells you!
