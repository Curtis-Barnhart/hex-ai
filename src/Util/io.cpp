#include <string>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameSolve/HexUtil.hpp"

#include "hex-ai/Util/io.hpp"

#ifndef HEX_AI_DATA_DIR
#define HEX_AI_DATA_DIR "./data/"
#endif // !HEX_AI_DATA_DIR

using GameState::HexState;

[[nodiscard("Return value is an error code - do not discard.")]]
int write_hexstates(
    const std::string &output_name,
    const std::vector<GameState::HexState> &games,
    const std::vector<bool> &winner
) {
    std::string filename(HEX_AI_DATA_DIR);
    filename.append(output_name);
    std::ofstream fileout(filename);
    
    // N times, create board state, solve it, and write down result.
    for (int x = 0; x < 100; x++) {

        // calculate outcome and write it down
        one_wins = ab.one_wins_one_turn(s);
        s.pack_to_stream(fileout);
        fileout.put(one_wins ? '1' : '0');
    }

    // close file
    fileout.close();
}

[[nodiscard("Return value is an error code - do not discard.")]]
void read_examples(
    const std::string &input_filename,
    std::vector<HexState> &games,
    std::vector<bool> &winner
) {
    HexState s;
    char one_wins_char;
    std::string abs_filename(HEX_AI_DATA_DIR"ex_games/");
    abs_filename.append(input_filename);
    std::ifstream filein(abs_filename);

    // N times, read in board state and solution, and record into vector
    for (int x = 0; x < 100; x++) {
        s.unpack_from_stream(filein);
        filein.get(one_wins_char);
        games.push_back(s);
        winner.push_back(one_wins_char == '1');
    }

    // close file
    filein.close();
}

