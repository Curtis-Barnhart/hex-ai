#ifndef UTIL_FILE_IO_HPP
#define UTIL_FILE_IO_HPP

#include <string>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

namespace Util::FileIO {

enum HEX_FILE_TYPE {
    UNRECOGNIZED,
    GAMESTATE_00,
    BOOLS_00,
};

unsigned int read_gamestate_00(
    const std::string &filename,
    std::vector<GameState::HexState> states
);

unsigned int write_gamestate_00(
    const std::string &filename,
    const std::vector<GameState::HexState> states
);

}

#endif // !UTIL_FILE_IO_HPP

