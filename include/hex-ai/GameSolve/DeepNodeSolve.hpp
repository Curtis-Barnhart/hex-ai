#ifndef GAMESOLVE_DEEPNODESOLVE_HPP
#define GAMESOLVE_DEEPNODESOLVE_HPP

#include <random>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

namespace GameSolve {

/**
*
*/
[[nodiscard("Return value is an error code of sorts - do not discard")]]
int hex_rand_moves(GameState::HexState &state, int turns) {
    static std::minstd_rand0 rand(0);

    std::vector<GameState::HexState::Action> a_list;
    while (turns-->0) {
        state.get_actions(a_list);
        if (a_list.empty()) {
            return 1;
        }
        state.succeed_in_place(a_list.at(rand() % a_list.size()));
        a_list.clear();
    }

    return 0;
}

}

#endif // !GAMESOLVE_DEEPNODESOLVE_HPP

