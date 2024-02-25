#include <cstddef>
#include <iostream>
#include <vector>

#include "Games/TicTacToe/TTT_State.hpp"

int main(int argc, char *argv[]) {
    auto *test = new GameState::TTT_State();
    GameState::TA action(0, 0, 0);
    test->succeed_in_place(action);
    std::cout << *test << std::endl;
    return 0;
}

