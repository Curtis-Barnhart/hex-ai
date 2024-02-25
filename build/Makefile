PROJ_ROOT := ../src
CFLAGS    := 

main: $(PROJ_ROOT)/main.cpp $(PROJ_ROOT)/Games/Hex/HexState.hpp
	g++ -o main $(PROJ_ROOT)/main.cpp

TTT_State.o: $(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp
	g++ -c $(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp

$(PROJ_ROOT)/Games/TicTacToe/TTT_StateBase.hpp: $(PROJ_ROOT)/GameState/GameState.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_StateBase.hpp

$(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp: $(PROJ_ROOT)/Games/TicTacToe/TTT_StateBase.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp

clean:
	rm *.o

