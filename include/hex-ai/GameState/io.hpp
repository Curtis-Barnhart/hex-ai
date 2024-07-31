#ifndef HEX_AI_UTIL_IO_HPP
#define HEX_AI_UTIL_IO_HPP

#include <string>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

namespace GameState {

/**
*
* @return 0 if successful,
*         1 if an error was encountered while writing a HexState to disk.
*/
[[nodiscard("Return value is an error code - do not discard.")]]
int write_hexstates(
    const std::string &output_name,
    const std::vector<GameState::HexState> &games
);

/**
*
* @return 0 if successful,
*         1 if the file ended unexpectedly,
*         2 if the file contained a value that should not exist in a HexState,
*         3 if an error was encountered between reading in the number of HexStates
*           and reading in the HexStates.
*/
[[nodiscard("Return value is an error code - do not discard.")]]
int read_hexstates(
    const std::string &input_filename,
    std::vector<GameState::HexState> &games
);

}

#endif // !HEX_AI_UTIL_IO_HPP

