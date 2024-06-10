#include <fstream>
#include <functional>
#include <string>
#include <thread>

#include "GameState/HexState.hpp"
#include "GameSolve/AlphaBeta.hpp"
#include "GameSolve/DeepNodeSolve.hpp"

using State = GameState::HexState;
using Action = GameState::HexState::Action;

/*
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
*/

/*
void run_game() {
    std::cout << "sizeof action: " << sizeof(Action) << ", sizeof state: " << sizeof(State) << "\n";
    State *test = new State();

    std::cout << "Current Board:\n" << *test;

    GameSolve::Minimax2PlayersCached<State, Action, Action> maximin(100000);

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
*/

// template<int N>
void do_N_examples(
    GameSolve::AlphaBeta2PlayersCached &ab,
    std::string output_name
) {
    // make variables for a state and for the file to write to
    State s;
    bool one_wins;
    std::ofstream fileout(output_name);
    
    // N times, create board state, solve it, and write down result.
    for (int x = 0; x < 100; x++) {
        // make new game states until they don't have a winner
        do {
            s = State();
            GameSolve::hex_rand_moves(s, 106);
        } while (s.who_won() != GameState::HexState::PLAYERS::PLAYER_NONE);
        
        // calculate outcome and write it down
        one_wins = ab.one_wins_one_turn(s);
        s.to_stream(fileout);
        fileout.put(one_wins ? '1' : '0');
    }

    // close file
    fileout.close();
}

// template<int N>
void read_N_examples(
    std::string input_name
) {
    State s;
    char one_wins_char;
    bool one_wins;
    std::ifstream filein(input_name);

    // N times, read in board state and solution, and record into vector
    for (int x = 0; x < 1; x++) {
        s.from_stream(filein);
        filein.get(one_wins_char);
        one_wins = one_wins_char == '1';
    }

    // close file
    filein.close();
}

int main() {
    /* example of writing to and reading from file
    std::ofstream fileout("test.txt");
    State s;
    for (int x = 5; x-->0;) {
        s = State();
        GameSolve::hex_rand_moves(s, 3);
        s.to_stream(fileout);
        std::cout << s << '\n';
    }
    fileout.close();
    std::cout << "-----------------------------------------------------\n";

    std::ifstream filein("test.txt", std::ios_base::binary);
    for (int x = 5; x-->0;) {
        s.from_stream(filein);
        std::cout << s << '\n';
    }
    filein.close();
    */

    constexpr int THREADS = 8;

    GameSolve::AlphaBeta2PlayersCached *ab[THREADS];
    for (int x = 0; x < THREADS; x++) {
        ab[x] = new GameSolve::AlphaBeta2PlayersCached(1500000);
    }

    State s;

    int sample_number = 72;
    while (true) {
        std::thread *threads[THREADS];
        
        for (int x = 0; x < THREADS; x++) {
            std::string record_name = "record" + std::to_string(sample_number++) + ".data";
            std::thread *t = new std::thread(do_N_examples, std::ref(*(ab[x])), record_name);
            threads[x] = t;
        }

        for (int x = 0; x < THREADS; x++) {
            threads[x]->join();
        }
    }

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

    return 0;
}

