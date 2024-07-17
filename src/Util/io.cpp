#include <string>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Util/io.hpp"

using GameState::HexState;

[[nodiscard("Return value is an error code - do not discard.")]]
int Util::write_hexstates(
    const std::string &output_name,
    const std::vector<GameState::HexState> &games
) {
    unsigned int count = games.size();
    int status;

    std::ofstream fileout(output_name);

    fileout.write((const char *) &count, sizeof(unsigned int) / sizeof(char));

    for (const HexState &h : games) {
        if (!(status = h.pack_to_stream(fileout))) {
            return 1;
        }
    }

    fileout.close();
    return 0;
}

[[nodiscard("Return value is an error code - do not discard.")]]
int read_hexstates(
    const std::string &input_filename,
    std::vector<HexState> &games
) {
    unsigned int count;

    std::ifstream filein(input_filename);

    filein.read((char *) &count, sizeof(int) / sizeof(char));
    if (filein.eof()) {
        return 1;
    }

    // N times, read in board state and solution, and record into vector
    for (unsigned int x = 0; x < count; x++) {
        games.emplace_back();
        if (int status = !games.end()->unpack_from_stream(filein)) {
            return status;
        }
    }

    filein.close();
    return 0;
}

