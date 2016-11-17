
# the compiler is g++
CXX      = g++
CXXFLAGS = -std=c++11 -g -c
LDFLAGS  = -std=c++11 -g -o

# include and library directories
INC_DIRS =
LIB_DIRS =
LIBS     =

# shortcuts to compile and link
COMPILE = $(CXX) $(INC_DIRS) $(CXXFLAGS)
LINK = $(CXX) $(LIB_DIRS) $(LDFLAGS) $(LIBS)

# removal of things for clean
RM = rm

# project details
PROJECT_NAME = program3
PROJECT_SOURCE = main.cpp tictactoe.cpp NetworkConnectionBase.cpp NetworkServer.cpp NetworkClient.cpp
PROJECT_OBJECT = main.o tictactoe.o NetworkConnectionBase.o NetworkServer.o NetworkClient.o
PROJECT_HEADER = tictactoe.h NetworkConnectionBase.h NetworkServer.h NetworkClient.h

#make rules
all: $(PROJECT_NAME)

$(PROJECT_NAME): $(PROJECT_OBJECT)

	$(LINK) $(PROJECT_NAME) $(PROJECT_OBJECT)

main.o: main.cpp $(PROJECT_HEADER)

	$(COMPILE) main.cpp

tictactoe.o: tictactoe.cpp tictactoe.h

	$(COMPILE) tictactoe.cpp

NetworkConnectionBase.o: NetworkConnectionBase.cpp NetworkConnectionBase.h

	$(COMPILE) NetworkConnectionBase.cpp

NetworkServer.o: NetworkServer.cpp NetworkServer.h NetworkConnectionBase.h

	$(COMPILE) NetworkServer.cpp

NetworkClient.o: NetworkClient.cpp NetworkClient.h NetworkConnectionBase.h

	$(COMPILE) NetworkClient.cpp

clean:

	$(RM) $(PROJECT_OBJECT)
	$(RM) $(PROJECT_NAME)

run: all

	./$(PROJECT_NAME)
