PROJ_ROOT := ../src
DOCS_ROOT := ../docs
#CFLAGS    := 

main: $(PROJ_ROOT)/main.cpp                      \
	  $(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp \
	  TTT_Actions.o                              \
	  TTT_State.o
	g++ -o main $(PROJ_ROOT)/main.cpp TTT_Actions.o TTT_State.o

##### tic-tac-toe related recipies

TTT_Actions.o: $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp
	g++ -c $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp

$(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp: $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.cpp

TTT_State.o: $(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp
	g++ -c $(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp

$(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp: $(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_State.cpp

$(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp: $(PROJ_ROOT)/Games/TicTacToe/TTT_Actions.hpp \
	                                        $(PROJ_ROOT)/GameState/GameState.hpp
	touch $(PROJ_ROOT)/Games/TicTacToe/TTT_State.hpp

##### documentation related recipies

docs: $(DOCS_ROOT)/config/Doxyfile
	doxygen $(DOCS_ROOT)/config/Doxyfile

clean:
	rm *.o

