#include <cassert>
#include <string>
#include <vector>

#include "hex-ai/Util/generated_games_combiner.hpp"
#include "hex-ai/GameState/io.hpp"

int Util::combine_games(
    const std::vector<std::string> &filenames,
    const std::string &new_filename,
    std::string &error_filename
) {
    std::vector<GameState::HexState> all_states;

    unsigned int error;
    for (const std::string &f: filenames) {
        error = GameState::read_hexstates(f, all_states);
        assert(error < 4); // valid error code assert

        switch (error) {
            case 0:
                break;
            case 1:
            case 2:
            case 3:
                error_filename = f;
                return error;
        }
    }

    error = GameState::write_hexstates(new_filename, all_states);
    assert(error < 2); // valid error code assert
    switch (error) {
        case 0:
            break;
        case 1:
            return 4;
    }
    return 0;
}

int Util::combine_bools(
    const std::vector<std::string> &filenames,
    const std::string &new_filename
) {
    std::vector<bool> all_bools;

    for (const std::string &f: filenames) {
        GameState::read_bools(f, all_bools);
    }

    int error = GameState::write_bools(new_filename, all_bools);
    assert(error < 2); // valid error code assert

    switch (error) {
        case 0:
            break;
        case 1:
            return 1;
    }
    return 0;
}

