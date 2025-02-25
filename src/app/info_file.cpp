/*
 * Copyright 2025 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <cstdint>

#include <cereal/archives/binary.hpp>
#include <cereal/details/helpers.hpp>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Io/GamestateBool0.hpp"
#include "hex-ai/Io/io_enums.hpp"

/**
 *
 * @param filename path to the file to analyze.
 */
void info_file(const std::string &filename) {
    std::ifstream infile(filename);
    if (infile.good()) {
        try {
            uint8_t filetype, version, board_size;

            cereal::BinaryInputArchive arc(infile);
            arc(filetype);
            arc(version);
            arc(board_size);
            switch (filetype) {
                case Io::GAMESTATE_BOOL:
                    switch (version) {
                        case 0:
                            {
                                int acc = 0;
                                GameState::HexState<5> state;
                                bool b;
                                // TODO: this is disgusting - fix it
                                if (board_size == 5) {
                                    Io::GamestateBool0Reader<5> r(infile);
                                    while (r.pop(state, b) == Io::GamestateBool0Reader<5>::CLEAR) {
                                        ++acc;
                                    }
                                    if (r.read_err() != Io::GamestateBool0Reader<5>::EMPTY) {
                                        std::cerr << "hex-ai: "
                                                  << filename
                                                  << " contained an error.\n";
                                    } else {
                                        std::cout << filename 
                                                << ": GAMESTATE_BOOL version 0, "
                                                << acc << std::endl;
                                    }
                                } else {
                                    std::cerr << "hex-ai: "
                                              << filename
                                              << " has unreadable board size "
                                              << board_size << std::endl;
                                }
                            }
                            break;
                        default:
                            // couldn't find version
                            std::cerr << "hex-ai: " << filename << " has bad version\n";
                            break;
                    }
                    break;
                default:
                    // couldn't find file type
                    std::cerr << "hex-ai: " << filename << " has bad file type\n";
                    break;
            }
        }
        catch (cereal::Exception &) {
            // cereal error
            std::cerr << "hex-ai: error reading file " << filename << std::endl;
        }
    } else {
        // reading file error
        std::cerr << "hex-ai: error reading file " << filename << std::endl;
    }
}

int main (int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (std::filesystem::is_regular_file(argv[i])) {
            info_file(argv[i]);
        } else {
            std::cerr << "hex-ai: " << argv[i] << " is not a file\n";
        }
    }
    
    return 0;
}

