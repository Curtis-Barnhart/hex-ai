CXX            := g++
PROJ_ROOT      := ../src
DOCS_ROOT      := ../docs
CFLAGS         := -g -Wall

main: $(PROJ_ROOT)/main.cpp                      \
	  $(PROJ_ROOT)/GameState/GameState.hpp       \
	  TTT_Actions.o                              \
	  TTT_State.o
	$(CXX) $(CFLAGS) -o main $(PROJ_ROOT)/main.cpp TTT_Actions.o TTT_State.o

##### tic-tac-toe related recipies

TTT_Actions.o: $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp
	$(CXX) $(CFLAGS) -c $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp

$(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp: $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp

TTT_State.o: $(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp
	$(CXX) $(CFLAGS) -c $(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp

$(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp: $(PROJ_ROOT)/GameState/GameState.hpp          \
	                                        $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp

clean:
	rm *.o

