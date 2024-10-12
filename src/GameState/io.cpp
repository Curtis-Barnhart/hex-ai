#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/io.hpp"

using GameState::HexState;

[[nodiscard("Return value is an error code - do not discard.")]]
unsigned int GameState::write_hexstates(
    const std::string &output_name,
    const std::vector<HexState> &games
) {
    unsigned int count = games.size();

    std::ofstream fileout(output_name);

    if (!fileout.good()) {
        return 2;
    }

    fileout.write((const char *) &count, sizeof(unsigned int) / sizeof(char));

    for (const HexState &h : games) {
        if (h.pack_to_stream(fileout)) {
            return 1;
        }
    }
    fileout.close();

    return 0;
}

[[nodiscard("Return value is an error code - do not discard.")]]
unsigned int GameState::read_hexstates(
    const std::string &input_filename,
    std::vector<HexState> &games
) {
    unsigned int count;
    int status;

    std::ifstream filein(input_filename);
    if (!filein.good()) {
        filein.close(); // TODO: Do I even need to do this?
        return 4;
    }

    filein.read((char *) &count, sizeof(int) / sizeof(char));
    if (filein.eof()) {
        return 1;
    }

    // N times, read in board state and solution, and record into vector
    for (unsigned int x = 0; x < count; x++) {
        games.emplace_back();
        switch (status = games.back().unpack_from_stream(filein)) {
            case 0:
                break;
            case 1:
            case 2:
            case 3:
                return status;
            default:
                std::cerr << "hex-ai: unhandled error code " << status << ".\n";
                assert(false);
        }
    }

    filein.close();
    return 0;
}

[[nodiscard("Return value is an error code - do not discard.")]]
unsigned int GameState::write_bools(
    const std::string &output_name,
    const std::vector<bool> &bools
) {

    std::ofstream fileout(output_name);

    if (!fileout.good()) {
        return 1;
    }

    for (const bool &b : bools) {
        fileout.put(b ? '1' : '0');
    }
    fileout.close();

    return 0;
}

void GameState::read_bools(
    const std::string &input_filename,
    std::vector<bool> &bools
) {
    char val;
    std::ifstream filein(input_filename);
    while (filein) {
        filein.get(val);
        bools.push_back(val == '1');
    }

    filein.close();
}

