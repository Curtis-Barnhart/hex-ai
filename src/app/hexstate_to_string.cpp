/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Util/FileIO/file_type_enum.hpp"
#include "hex-ai/Util/FileIO/file_types.hpp"
#include "hex-ai/Util/FileIO/GamestateBool_1.hpp"
#include "hex-ai/GameSolve/AlphaBeta.hpp"

using std::vector;
using std::string;
using GameState::HexState;
using Util::FileIO::GamestateBool1Reader;

int main (int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "hex-ai: hexstate_to_string takes exactly 3 arguments.\n";
    }

    std::ifstream infile(argv[1], std::ifstream::binary);
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

    uint8_t file_type, file_version;
    try {
        cereal::BinaryInputArchive archive(infile);
        archive(file_type);
        archive(file_version);
    } catch (cereal::Exception &) {
        std::cerr << "hex-ai: File " << argv[1] << " was corrupted and could not be read.\n";
        return 1;
    }
    if (file_type != Util::FileIO::GAMESTATE_BOOL) {
        std::cerr << "hex-ai: File " << argv[1] << " is not of type GAMESTATE_BOOL.\n";
        return 1;
    }
    if (file_version != 1) {
        std::cerr << "hex-ai: File " << argv[1] << " is not of GAMESTATE_BOOL version equal to 1.\n";
        return 1;
    }

    HexState h;
    bool b;
    GamestateBool1Reader reader(infile);

    while (reader.read_err() != GamestateBool1Reader::EMPTY) {
        if (reader.pop(h, b) != GamestateBool1Reader::CLEAR) {
            std::cerr << "hex-ai: File " << argv[1] << " was corrupted and could not be read.\n";
            return 1;
        }
        h.simple_string(py_states);
        py_bools << (b ? '1' : '0') << '\n';
    }
    return 0;
}

