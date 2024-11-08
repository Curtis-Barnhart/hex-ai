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

#include "cereal/archives/binary.hpp"
#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Util/FileIO/file_type_enum.hpp"
#include "hex-ai/Util/FileIO/file_types.hpp"
#include "hex-ai/Util/FileIO/GamestateBool_1.hpp"
#include "hex-ai/GameSolve/AlphaBeta.hpp"

using std::vector;
using std::string;
using GameState::HexState;
using Util::FileIO::GamestateBool1Reader;
using Util::FileIO::GamestateBool1Writer;
using Util::FileIO::read_gamestate_bools_00;
using Util::FileIO::write_gamestate_bools_00;

int combine_gamestate_bool_00(const vector<string> &in_paths, string &out_path) {
    bool file_errors = false;
    vector<std::ifstream> in_files;
    std::ofstream out_file(out_path);

    // Open all files and print out errors if any couldn't be opened and return 1
    for (const string &s : in_paths) {
        in_files.emplace_back(s);
        if (!in_files.back().good()) {
            std::cerr << "hex-ai: File " << s << " could not be opened for reading.\n";
            file_errors = true;
        }
    }
    if (!out_file.good()) {
        std::cerr << "hex-ai: File " << out_path << " could not be opened for writing.\n";
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
        if (read_gamestate_bools_00(in_files.at(x), states_buff, bools_buff)) {
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
    if (write_gamestate_bools_00(out_file, states, bools)) {
        std::cerr << "hex-ai: File " << out_path << " was corrupted and could not be read.\n";
        return 1;
    }

    return 0;
}

/**
* @return 0 everything worked
*         1 recoverable error (input file could not be read)
*         2 fatal error (output file could not be written to)
*/
unsigned int gb0_to_gb1(
    std::istream &reader,
    GamestateBool1Writer &writer,
    const string &in_fname,
    const string &out_fname
) {
    vector<HexState> states;
    vector<bool> bools;
    if (read_gamestate_bools_00(reader, states, bools)) {
        std::cerr << "hex-ai: File " << in_fname << " was corrupted and could not be read.\n";
        return 1;
    }
    if (states.size() != bools.size()) {
        std::cerr << "hex-ai: File " << in_fname << " had differing amounts of states and bools.\n";
        return 1;
    }
    for (size_t x = 0; x < states.size(); x++) {
        if (writer.push(states.at(x), bools.at(x))) {
            std::cerr << "hex-ai: File " << out_fname << " could not be written to.\n";
            return 2;
        }
    }
    return 0;
}

/**
* @return 0 everything worked
*         1 recoverable error (input file could not be read)
*         2 fatal error (output file could not be written to)
*/
unsigned int gb1_to_gb1(
    std::istream &stream,
    GamestateBool1Writer &writer,
    const string &in_fname,
    const string &out_fname
) {
    GamestateBool1Reader reader(stream);
    HexState h;
    bool b;
    while (reader.read_err() != GamestateBool1Reader::EMPTY) {
        if (reader.pop(h, b) != GamestateBool1Reader::CLEAR) {
            std::cerr << "hex-ai: File " << in_fname << " was corrupted and could not be read.\n";
            return 1;
        }
        if (writer.push(h, b) != GamestateBool1Writer::CLEAR) {
            std::cerr << "hex-ai: File " << out_fname << " could not be written to.\n";
            return 2;
        }
    }
    return 0;
}

unsigned int combine_to_gamestate_bool_1(
    const vector<string> &in_paths,
    const string &out_path
) {
    int file_errors = 0;
    vector<std::ifstream> in_files;
    std::ofstream out_file(out_path);

    if (!out_file.good()) {
        std::cerr << "hex-ai: File " << out_path << " could not be opened for writing.\n";
        return 1;
    }
    GamestateBool1Writer writer(out_file);

    // Open all files and print out errors if any couldn't be opened and return 1
    for (const string &s : in_paths) {
        in_files.emplace_back(s);
        if (!in_files.back().good()) {
            std::cerr << "hex-ai: File " << s << " could not be opened for reading.\n";
            file_errors = 1;
        }
    }
    if (!out_file.good()) {
        std::cerr << "hex-ai: File " << out_path << " could not be opened for writing.\n";
        file_errors = 1;
    }
    if (file_errors) {
        return file_errors;
    }

    // read in all files one by one
    // only throw an error after we've tried to read in as many as possible
    // to try and give as many errors as can be given at once
    std::vector<bool> bools;
    std::vector<GameState::HexState> states;
    uint8_t file_type, file_version;
    for (size_t x = 0; x < in_files.size(); x++) {
        bools.clear();
        states.clear();
        try {
            cereal::BinaryInputArchive archive(in_files.at(x));
            archive(file_type);
            archive(file_version);
        } catch (cereal::Exception &) {
            std::cerr << "hex-ai: File " << in_paths.at(x) << " was corrupted and could not be read.\n";
            file_errors = 1;
            continue;
        }
        if (file_type != Util::FileIO::GAMESTATE_BOOL) {
            std::cerr << "hex-ai: File " << in_paths.at(x) << " is not of type GAMESTATE_BOOL.\n";
            file_errors = 1;
            continue;
        }

        switch (file_version) {
            case 0:
                if (gb0_to_gb1(in_files.at(x), writer, in_paths.at(x), out_path) == 2) {
                    return 1;
                }
                break;
            case 1:
                if (gb1_to_gb1(in_files.at(x), writer, in_paths.at(x), out_path) == 2) {
                    return 1;
                }
                break;
            default:
                std::cerr << "hex-ai: File " << in_paths.at(x) << " has unrecognized version " << file_version << ".\n";
                file_errors = 1;
                continue;
        }
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
                // last argument is the output filename
                // all other arguments are input files
                if (argc < 4) {
                    std::cerr << "hex-ai: file_combine combining requires at least 2 additional arguments.\n";
                    return 1;
                }
                vector<string> in_paths;
                for (int x = 2; x < argc - 1; x++) {
                    in_paths.emplace_back(argv[x]);
                }
                string out_path(argv[argc - 1]);
                return combine_to_gamestate_bool_1(in_paths, out_path);
            }
        default:
            std::cerr << "hex-ai: file_combine does not recognize flag.\n";
            return 1;
    }
}

