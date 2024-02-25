#include <cstddef>
#include <iostream>
#include <vector>

#include "Games/TicTacToe/TTT_State.hpp"

int main(int argc, char *argv[]) {
    auto *test = new GameState::TTT_State();
    test->get_actions();
    return 0;
}

