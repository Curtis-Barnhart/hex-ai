#include <iostream>
#include <vector>

#include "GameState/GameState.hpp"
#include "Games/TicTacToe/TTT_Actions.hpp"
#include "Games/TicTacToe/TTT_State.hpp"
#include "GameSolve/AlphaBeta.hpp"

using GameState::TA;
using GameState::State;

void get_p_action(TA &action) {
    signed char x, y;
    std::cout << "enter x coord: ";
    std::cin >> x;
    std::cout << "enter y coord: ";
    std::cin >> y;
    action.x = x - '0';
    action.y = y - '0';
    action.whose = 0;
}

int main(int argc, char *argv[]) {
    State<TA, TA, 2> *test = new State<TA, TA, 2>();
    std::cout << "Current Board:\n" << *test;

    GameSolve::MaximinCached<TA, TA, 2> maximin(50000);

    TA player_action;
    std::vector<TA> actions;
    while (true) {
        get_p_action(player_action);
        test->succeed_in_place(player_action);
        
        std::cout << "Player moves\n" << "Current Board:\n" << *test << "\n";
        if (test->who_won() != -1) {
            std::cout << "Player wins!\n";
            break;
        }

        test->get_actions(actions);
        if (actions.empty()) {
            std::cout << "Game over: no one wins!\n";
            break;
        }
        actions.clear();

        TA *computer_action = maximin.maximin_no_prune(*test);
        test->succeed_in_place(*computer_action);
        std::cout << "Computer moves\n" << "Current Board:\n" << *test << "\n";
        if (test->who_won() != -1) {
            std::cout << "Computer wins!\n";
            break;
        }
        delete computer_action;
    }

    delete test;
    return 0;
}

