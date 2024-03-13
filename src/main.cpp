#include <iostream>

#include "GameState/GameState.hpp"
#include "Games/TicTacToe/TTT_Actions.hpp"

using GameState::TA;
using GameState::State;

extern template class GameState::State<TA, TA>;

int main(int argc, char *argv[]) {
    auto *test = new State<TA, TA>();
    TA action(0, 0, 0);
    test->succeed_in_place(action);
    std::cout << *test << std::endl;
    return 0;
}

