/*
 * Copyright 2025 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <atomic>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameSolve/AlphaBeta.hpp"
#include "hex-ai/GameSolve/HexUtil.hpp"
#include "hex-ai/GameState/enums.hpp"
#include "hex-ai/Io/GamestateBool0.hpp"

using State = GameState::HexState<5>;
using Action = GameState::Action;

int gen_winning(
    int n,
    const std::string &output_path
) {
    std::ofstream outfile(output_path);
    Io::GamestateBool0Writer<5> writer(outfile);
    State s;
    bool b;
    int err;

    // N times, create board state, solve it, and write down result.
    for (int x = 0; x < n; x++) {
        s = State();
        GameSolve::hex_rand_moves(s, 25, GameState::PLAYER_ONE);

        // calculate outcome and write it down
        b = s.who_won() == GameState::PLAYER_ONE;
        if ((err = writer.push(s, b))) {
            return err;
        }
    }

    return 0;
}

/*
int generate_examples(
    GameSolve::AlphaBeta2PlayersCached<5> &ab,
    int n,
    const std::string &output_path
) {
    std::ofstream outfile(output_path);
    Io::GamestateBool0Writer<5> writer(outfile);
    State s;
    bool b;
    
    // N times, create board state, solve it, and write down result.
    for (int x = 0; x < n; x++) {
        // make new game states until they don't have a winner
        do {
            s = State();
            GameSolve::hex_rand_moves(s, 25, GameState::PLAYER_ONE);
        } while (s.who_won() != GameState::PLAYER_NONE);
        
        // calculate outcome and write it down
        b = ab.one_wins_one_turn(s);
        if (writer.push(s, b)) {
            throw std::exception();
        }
    }

    return 0;
}
*/

void generate_loop(std::atomic<int> &count, int bundle, const std::string &filebase) {
    while (true) {
        int my_count = count.fetch_sub(1);
        if (my_count <= 0) {
            return;
        }

        std::string hex_outs, bool_outs;

        std::stringstream s;
        s << filebase << "_hex" << std::setfill('0') << std::setw(5) << my_count;
        s >> hex_outs;
        s.str("");
        s.clear();
        s << filebase << "_bool" << std::setfill('0') << std::setw(5) << my_count;
        s >> bool_outs;

        if (gen_winning(bundle, hex_outs)) {
            std::cerr << "AAAAAAAAAAAAAAAAAA\n";
        }
        std::cout << "finishing " << my_count << "\n";
    }
}

int main (int argc, char *argv[]) {
    if (argc < 5) {
        std::cout << "needs 4 args\n";
        return 0;
    }

    std::atomic<int> count;
    int thread_ct, bundle, int_count;
    std::string filebase;

    std::stringstream args;
    args << argv[1] << ' ' << argv[2] << ' ' << argv[3] << ' ' << argv[4];
    args >> thread_ct;
    args >> int_count;
    args >> bundle;
    args >> filebase;
    count = int_count;

    std::vector<std::thread *> threads;

    for (int x = 0; x < thread_ct; x++) {
        threads.push_back(new std::thread(generate_loop, std::ref(count), bundle, std::ref(filebase)));
    }

    for (int x = 0; x < thread_ct; x++) {
        threads[x]->join();
    }

    return 0;
}

