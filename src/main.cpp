#include <cassert>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <thread>
#include <vector>

#include "Games/Hex/HexState.hpp"
#include "GameSolve/Maximin.hpp"
#include "GameSolve/DeepNodeSolve.hpp"

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

void run_game() {
    std::cout << "sizeof action: " << sizeof(Action) << ", sizeof state: " << sizeof(State) << "\n";
    State *test = new State();

    std::cout << "Current Board:\n" << *test;

    GameSolve::Minimax2PlayersCached<State, Action, Action> maximin(5000);

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

}

void generate_and_solve(GameSolve::Minimax2PlayersCached<State, Action, Action> &cache) {
    State s;
    GameSolve::hex_rand_moves(s, 108);
    while (s.who_won() != -1) {
        s = State();
        GameSolve::hex_rand_moves(s, 108);
    }
    delete cache.maximin_no_prune(s);
}

void work(int t) {
    GameSolve::Minimax2PlayersCached<State, Action, Action> cache(1000000);
    while (t-->0) {
        generate_and_solve(cache);
    }
    std::cout << "Nodes expanded: " << cache.nodes_hit_total << "\n";
    std::cout << "Terms expanded: " << cache.terms_hit_total << "\n";
}



int main() {
    assert(sizeof(char) == 1);
    // run_game();
    
    // State s;
    // for (int x = 10000; x-->0;) {
    //     GameSolve::hex_rand_moves(s, 110);
    //     s = State();
    // }
    State s, copies[5];
    std::ofstream fileout("test.txt");
    for (int x = 0; x < 5; x++) {
        GameSolve::hex_rand_moves(s, 21);
        copies[x] = s;
        std::cout << s;
        std::cout << s.whose_turn() << '\n';
        s.to_stream(fileout);
    }
    fileout.close();

    std::cout << "-------------------------------------------\n";

    std::ifstream filein("test.txt", std::ios_base::binary);
    for (int x = 0; x < 5; x++) {
        s.from_stream(filein);
        if (s != copies[x]) {
            std::cout << "UH NO NO BUENO\n";
        }
        std::cout << s;
        std::cout << s.whose_turn() << '\n';
    }
    filein.close();

    /*
     * This is an example of how multithreading would work
    std::thread *threads[8];
    
    for (int x = 0; x < 8; x++) {
        std::thread *t = new std::thread(work, 10);
        threads[x] = t;
    }

    for (int x = 0; x < 8; x++) {
        threads[x]->join();
    }
    */

    // GameSolve::Minimax2PlayersCached<State, Action, Action> minimax(100000);
    // delete minimax.maximin_no_prune(s);
    // std::cout << minimax.nodes_hit_total << '\n';

    return 0;
}

