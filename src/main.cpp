#include <cstddef>
#include <iostream>
#include <vector>

#include "Games/TicTacToe/TicTacToe.hpp"

template<class A>
std::size_t count_moves(const GameState::State<A> &s) {
    std::vector<A> actions;
    s.get_actions(actions);
    return actions.size();
}

int main (int argc, char *argv[]) {
    GameState::TTT_State initial = GameState::TTT_State();
    std::cout << "The number of initial moves is "
              << count_moves<GameState::TTT_Action>(initial)
              << "\n";

    std::cout << "Works so far\n";
    //int winner = random_play_game(initial);
    //std::cout << "Player " << winner << " wins!\n";
    return 0;
}

