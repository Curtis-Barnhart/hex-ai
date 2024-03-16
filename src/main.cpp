#include <cstdio>

#include "GameState/GameState.hpp"
#include "Games/TicTacToe/TTT_Actions.hpp"
#include "Games/TicTacToe/TTT_State.hpp"
#include "GameSolve/AlphaBeta.hpp"

using GameState::TA;
using GameState::State;

int main(int argc, char *argv[]) {
    State<TA, TA, 2> *test = new State<TA, TA, 2>();

    // TA action(0, 0, 0);
    // test->succeed_in_place(action);
    // action = TA(0, 1, 0);
    // test->succeed_in_place(action);
    // action = TA(0, 2, 0);
    // test->succeed_in_place(action);

    // std::cout << *test << std::endl;
    // std::cout << test->who_won() << std::endl;
    
    GameSolve::MaximinCached<TA, TA, 2> maximin(50000);
    delete maximin.maximin_no_prune(*test);
    std::printf("Total nodes expanded: %ld\nTotal leaves examined: %ld\n", maximin.nodes_hit, maximin.terms_hit);
    delete test;

    return 0;
}

