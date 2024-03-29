CXX            := g++
PROJ_ROOT      := ../src
CFLAGS         := -g -Wall -std=c++11

main: $(PROJ_ROOT)/main.cpp                      \
	  $(PROJ_ROOT)/GameState/GameState.hpp       \
	  $(PROJ_ROOT)/GameSolve/AlphaBeta.hpp       \
	  $(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp
	$(CXX) $(CFLAGS) -o main $(PROJ_ROOT)/main.cpp

##### tic-tac-toe related recipies

$(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp: $(PROJ_ROOT)/GameState/GameState.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp

##### GameSolve related recipies

$(PROJ_ROOT)/GameSolve/AlphaBeta.hpp: $(PROJ_ROOT)/GameSolve/LRUCache.hpp
	touch $(PROJ_ROOT)/GameSolve/AlphaBeta.hpp

clean:
	rm *.o

