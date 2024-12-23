/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
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
#include "hex-ai/Util/FileIO/GamestateBool_1.hpp"

using State = GameState::HexState;
using Action = GameState::HexState::Action;

int generate_examples(
    GameSolve::AlphaBeta2PlayersCached &ab,
    int n,
    const std::string &output_path
) {
    std::ofstream outfile(output_path);
    Util::FileIO::GamestateBool1Writer writer(outfile);
    State s;
    bool b;
    
    // N times, create board state, solve it, and write down result.
    for (int x = 0; x < n; x++) {
        // make new game states until they don't have a winner
        do {
            s = State();
            GameSolve::hex_rand_moves(s, 106);
        } while (s.who_won() != GameState::HexState::PLAYERS::PLAYER_NONE);
        
        // calculate outcome and write it down
        b = ab.one_wins_one_turn(s);
        if (writer.push(s, b)) {
            throw std::exception();
        }
    }

    return 0;
}

void generate_loop(std::atomic<int> &count, int bundle, const std::string &filebase) {
    while (true) {
        int my_count = count.fetch_sub(1);
        if (my_count <= 0) {
            return;
        }

        GameSolve::AlphaBeta2PlayersCached ab(1500000);
        std::string hex_outs, bool_outs;

        std::stringstream s;
        s << filebase << "_hex" << std::setfill('0') << std::setw(5) << my_count;
        s >> hex_outs;
        s.str("");
        s.clear();
        s << filebase << "_bool" << std::setfill('0') << std::setw(5) << my_count;
        s >> bool_outs;

        if (generate_examples(ab, bundle, hex_outs)) {
            std::cout << "AAAAAAAAAAAAAAAAAA\n";
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

