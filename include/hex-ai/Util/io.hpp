#ifndef HEX_AI_UTIL_IO_HPP
#define HEX_AI_UTIL_IO_HPP

#include <string>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

void write_100_examples(
    GameSolve::AlphaBeta2PlayersCached &ab,
    std::string output_name
);

void read_100_examples(
    std::string input_filename,
    std::vector<GameState::HexState> games,
    std::vector<bool> winner
);

#endif // !HEX_AI_UTIL_IO_HPP

