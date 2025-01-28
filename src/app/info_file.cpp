/*
 * Copyright 2025 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <cstdint>

#include <cereal/archives/binary.hpp>
#include <cereal/details/helpers.hpp>

#include "hex-ai/GameState/HexState.hpp"
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
                            std::cout << filename 
                                      << ": GAMESTATE_BOOL version 0\n";
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
        info_file(std::string(argv[1]));
    }
    
    return 0;
}

