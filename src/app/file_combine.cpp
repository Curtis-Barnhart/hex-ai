/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Util/FileIO/file_types.hpp"
#include "hex-ai/GameSolve/AlphaBeta.hpp"

using std::vector;
using std::string;
using GameState::HexState;

int main () {
    vector<HexState> states;
    vector<bool> bools;
    std::ifstream file("/home/curtisb/hex-type-test/all_data", std::ifstream::binary);
    if (!file.good()) {
        std::printf("double bruh\n");
    }

    file.seekg(2);

    if (Util::FileIO::read_gamestate_bools_00(file, states, bools)) {
        printf("bruh\n");
        return 1;
    }

    file.close();
    
    std::ofstream py_states("py_states.txt"), py_bools("py_bools.txt");

    if (!(py_states && py_bools)) {
        std::printf("Error opening writing files.\n");
    }

    for (HexState &h : states) {
        h.simple_string(py_states);
    }
    for (bool b: bools) {
        py_bools << (b ? '1' : '0');
    }

    return 0;
}

int not_main (int argc, char *argv[]) {
    std::vector<std::string> inputs;
    for (int x = 1; x < argc - 1; x++) {
        inputs.emplace_back(argv[x]);
    }
    std::string output(argv[argc - 1]);

    std::vector<bool> bools;
    std::vector<GameState::HexState> states;

    if (std::any_of(
        inputs.begin(),
        inputs.begin() + 3,
        [&states](std::string &fname) {
            std::ifstream s(fname, std::ifstream::binary);
            std::vector<GameState::HexState> local;
            s.seekg(2);
            unsigned int error = Util::FileIO::read_gamestate_01(s, local);
            states.insert(states.end(), local.begin(), local.end());
            return error;
        }
    )) {
        std::printf("ERROR READING (I'm not telling what though)\n");
        return 1;
    }

    if (std::any_of(
        inputs.begin() + 3,
        inputs.begin() + 6,
        [&bools](std::string &fname) {
            std::ifstream s(fname);
            std::vector<bool> local;
            s.seekg(2);
            unsigned int error = Util::FileIO::read_bools_01(s, local);
            bools.insert(bools.end(), local.begin(), local.end());
            return error;
        }
    )) {
        std::printf("ERROR READING (I'm not telling what though)\n");
        return 1;
    }

    std::ofstream s(output, std::ofstream::binary);
    if (Util::FileIO::write_gamestate_bools_00(s, states, bools)) {
        std::printf("ERROR WRITING (I'm not telling what though)\n");
        return 1;
    }

    return 0;
}

