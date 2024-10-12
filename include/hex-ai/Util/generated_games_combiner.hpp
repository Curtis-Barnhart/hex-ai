#ifndef HEX_AI_UTIL_GENERATED_GAMES_COMBINER_HPP
#define HEX_AI_UTIL_GENERATED_GAMES_COMBINER_HPP

#include <string>
#include <vector>

namespace Util {

/**
* @return 0 if successful,
*         1 if a file ended unexpectly,
*         2 if a file contained a value that should not exist in a HexState,
*         3 if an error was encountered between reading in the number of HexStates
*           and reading in the HexStates.
*         4 if an error was encountered while writing a HexState to disk.
*/
[[nodiscard("Return value is an error code - do not discard.")]]
unsigned int combine_games(
    const std::vector<std::string> &filenames,
    const std::string &new_filename,
    std::string &error_filename
);

/**
* @return 0 if successful,
*         1 if the output file could not be opened.
*/
unsigned int combine_bools(
    const std::vector<std::string> &filenames,
    const std::string &new_filename
);

}

#endif // !HEX_AI_UTIL_GENERATED_GAMES_COMBINER_HP

