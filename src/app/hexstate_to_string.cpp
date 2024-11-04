/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Util/FileIO/file_types.hpp"
#include "hex-ai/GameSolve/AlphaBeta.hpp"

using std::vector;
using std::string;
using GameState::HexState;

int main (int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "hex-ai: hexstate_to_string takes exactly 3 arguments.\n";
    }
    vector<HexState> states;
    vector<bool> bools;

    std::ifstream infile(argv[1]);
    std::ofstream py_states(argv[2]), py_bools(argv[3]);

    // make sure all three files were opened correctly
    if (!infile) {
        std::cerr << "hex-ai: File " << argv[1] << " could not be opened for reading.\n";
        return 1;
    }
    if (!py_states) {
        std::cerr << "hex-ai: File " << argv[1] << " could not be opened for writing.\n";
        return 1;
    }
    if (!py_bools) {
        std::cerr << "hex-ai: File " << argv[1] << " could not be opened for writing.\n";
        return 1;
    }

    // Read in from infile
    // right now we're just going to assume that the file is a
    // gamestate_bools version 0 file... we can fix this later
    infile.seekg(2);
    if (Util::FileIO::read_gamestate_bools_00(infile, states, bools)) {
        std::cerr << "hex-ai: File " << argv[1] << " was corrupted and could not be read.\n";
        return 1;
    }
    
    for (HexState &h : states) {
        h.simple_string(py_states);
    }
    for (bool b: bools) {
        py_bools << (b ? '1' : '0');
        py_bools << '\n';
    }

    return 0;
}

