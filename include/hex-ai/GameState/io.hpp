#ifndef HEX_AI_GAMESTATE_IO_HPP
#define HEX_AI_GAMESTATE_IO_HPP

#include <string>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

namespace GameState {

/**
 * Writes a vector of HexStates to a file.
 *
 * @return 0 if successful,
 *         1 if an error was encountered while writing a HexState to file
 *         2 if the file could not be opened to write to.
 */
[[nodiscard("Return value is an error code - do not discard.")]]
unsigned int write_hexstates(
    const std::string &output_name,
    const std::vector<GameState::HexState> &games
);

/**
 * Appends all HexStates stored in a file into a given vector.
 *
 * @return 0 if successful,
 *         1 if the file ended unexpectedly,
 *         2 if the file contained a value that should not exist in a HexState,
 *         3 if an error was encountered between reading in the number of HexStates
 *           and reading in the HexStates.
 *         4 if the file could not be opened for reading.
 */
[[nodiscard("Return value is an error code - do not discard.")]]
unsigned int read_hexstates(
    const std::string &input_filename,
    std::vector<GameState::HexState> &games
);

/**
 * Writes a vector of booleans to a file.
 * '1' will be written for true, '0' for false.
 *
 * @return 0 if successful,
 *         1 if an error occured while opening the file.
 */
[[nodiscard("Return value is an error code - do not discard.")]]
unsigned int write_bools(
    const std::string &output_name,
    const std::vector<bool> &bools
);

/**
 * Appends all bools stored in a file into a given vector.
 */
void read_bools(
    const std::string &input_filename,
    std::vector<bool> &bools
);

}

#endif // !HEX_AI_GAMESTATE_IO_HPP

