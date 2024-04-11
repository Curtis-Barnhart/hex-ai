#include <iostream>
#include <vector>

// #include "Games/TicTacToe/TTT_State.hpp"
#include "Games/Hex/HexState.hpp"
#include "GameSolve/AlphaBeta.hpp"

using State = GameState::HexState;
using Action = GameState::HexState::Action;

void get_p_action(Action &action) {
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
    std::cout << "sizeof action: " << sizeof(Action) << ", sizeof state: " << sizeof(State) << "\n";
    State *test = new State();

    // Action a1 = Action(0, 0, 0);
    // Action a2 = Action(0, 1, 0);
    // Action a3 = Action(0, 2, 0);
    // Action a4 = Action(1, 0, 1);
    // Action a5 = Action(2, 0, 1);

    // test->succeed_in_place(a1);
    // test->succeed_in_place(a2);
    // test->succeed_in_place(a3);
    // test->succeed_in_place(a4);
    // test->succeed_in_place(a5);
    // std::cout << *test << "\n";
    // std::cout << test->who_won() << "\n";

    // delete test;


    // std::cout << "sizeof action: " << sizeof(Action) << ", sizeof state: " << sizeof(State) << "\n";
    // GameState::TTT_State *test = new GameState::TTT_State();
    std::cout << "Current Board:\n" << *test;

    GameSolve::MaximinCached<State, Action, Action> maximin(20000000);

    Action player_action;
    std::vector<Action> actions;
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

        std::cout << "Computer thinking...\n";
        Action *computer_action = maximin.maximin_no_prune(*test);
        test->succeed_in_place(*computer_action);
        std::cout << "Computer moves\n" << "Current Board:\n" << *test << "\n";
        std::cout << "Game state nodes evaluated this turn: " << maximin.nodes_hit << "\n"
                  << "Game state nodes evaluated total    : " << maximin.nodes_hit_total << "\n";
        delete computer_action;

        if (test->who_won() != -1) {
            std::cout << "Computer wins!\n";
            break;
        }
    }

    delete test;
    return 0;
}

