#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "hex-ai/Util/FileIO/file_types.hpp"
#include "hex-ai/GameState/HexState.hpp"

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using GameState::HexState;

unsigned int Util::FileIO::read_gamestate_00(
    const string &filename,
    vector<HexState> states
) {
    ifstream ifile(filename);

    if (!ifile.good()) {
        return 1;
    }

    uint32_t count = states.size();
    uint8_t read;
}

unsigned int Util::FileIO::write_gamestate_00(
    const string &filename,
    const vector<HexState> states
) {
    ofstream ofile(filename);

    if (!ofile.good()) {
        return 1;
    }

    uint32_t count = states.size();
    uint8_t read, temp;
    int read_used = 0;
    for (int t = 4; t-->0;) {
        read = (count >> (8 * t)) & 0xff;
        ofile.put(read);
    }

    for (const HexState &s: states) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                temp = s.at(x, y);
                if (
                    temp != HexState::PLAYERS::PLAYER_ONE &&
                    temp != HexState::PLAYERS::PLAYER_TWO &&
                    temp != HexState::PLAYERS::PLAYER_NONE
                ) {
                    return 2;
                }

            }
        }
    }
}

