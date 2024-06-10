#include <thread>
#include <vector>

#include "GameState/HexState.hpp"
#include "GameSolve/AlphaBeta.hpp"
#include "GameSolve/DeepNodeSolve.hpp"

using State = GameState::HexState;
using Action = GameState::HexState::Action;

void write_100_examples(
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

void read_100_examples(
    std::string input_filename,
    std::vector<State> games,
    std::vector<bool> winner
) {
    State s;
    char one_wins_char;
    bool one_wins;
    std::ifstream filein(input_filename);

    // N times, read in board state and solution, and record into vector
    for (int x = 0; x < 100; x++) {
        s.from_stream(filein);
        filein.get(one_wins_char);
        one_wins = one_wins_char == '1';
        games.push_back(s);
        winner.push_back(one_wins);
    }

    // close file
    filein.close();
}

int main (int argc, char *argv[]) {
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
            std::thread *t = new std::thread(write_100_examples, std::ref(*(ab[x])), record_name);
            threads[x] = t;
        }

        for (int x = 0; x < THREADS; x++) {
            threads[x]->join();
        }
    }

    return 0;
}

