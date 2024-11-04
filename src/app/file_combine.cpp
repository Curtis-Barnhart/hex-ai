/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

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

int combine_gamestate_bool_00(const vector<string> &in_paths, string &out_path) {
    bool file_errors = false;
    vector<std::ifstream> in_files;
    std::ofstream out_file(out_path);

    // Open all files and print out errors if any couldn't be opened and return 1
    for (const string &s : in_paths) {
        in_files.emplace_back(s);
        if (!in_files.back()) {
            std::cerr << "hex-ai: File " << s << " could not be opened for reading.\n";
            file_errors = true;
        }
    }
    if (!out_file) {
        std::cerr << "hex-ai: File " << out_path << " could not be opened for reading.\n";
        file_errors = true;
    }
    if (file_errors) {
        return 1;
    }

    // read in all files one by one
    // only throw an error after we've tried to read in as many as possible
    // to try and give as many errors as can be given at once
    std::vector<bool> bools;
    std::vector<GameState::HexState> states;
    for (size_t x = 0; x < in_files.size(); x++) {
        std::vector<bool> bools_buff;
        std::vector<GameState::HexState> states_buff;
        // We assume the file given was actually a
        // gamestate_bool version 0 (we can fix later)
        in_files.at(x).seekg(2);
        if (Util::FileIO::read_gamestate_bools_00(
            in_files.at(x), states_buff, bools_buff
        )) {
            std::cerr << "hex-ai: File " << in_paths.at(x) << " was corrupted and could not be read.\n";
            file_errors = true;
        } else {
            bools.insert(bools.end(), bools_buff.begin(), bools_buff.end());
            states.insert(states.end(), states_buff.begin(), states_buff.end());
        }
    }
    if (file_errors) {
        return 1;
    }

    // attempt to write out all values
    if (Util::FileIO::write_gamestate_bools_00(out_file, states, bools)) {
        std::cerr << "hex-ai: File " << out_path << " was corrupted and could not be read.\n";
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "hex-ai: file_combine requires at least 1 argument.\n";
        return 1;
    }

    switch (*argv[1]) {
        case 'c':
            {
                if (argc < 5) {
                    std::cerr << "hex-ai: file_combine combining requires at least 3 additional arguments.\n";
                    return 1;
                }
                vector<string> in_paths;
                for (int x = 2; x < argc; x++) {
                    in_paths.emplace_back(argv[x]);
                }
                string out_path(argv[argc - 2]);
                return combine_gamestate_bool_00(in_paths, out_path);
            }
        default:
            std::cerr << "hex-ai: file_combine does not recognize flag.\n";
            return 1;
    }
}

