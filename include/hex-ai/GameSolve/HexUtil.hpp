/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HEX_AI_GAMESOLVE_HEXUTIL_HPP
#define HEX_AI_GAMESOLVE_HEXUTIL_HPP

#include <algorithm>
#include <cstddef>
#include <random>
#include <utility>

#include "hex-ai/GameState/Action.hpp"
#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/enums.hpp"

namespace GameSolve {

/**
* Attempts to play `turns` random turns on a HexState board.
* @param state HexState instance to make random moves on.
* @param turns amount of random moves to make.
* @return 0 if the exact amount of turns were made.
*         1 if the amount of turns that were made were less than `turns`
*         due to the game having been won by any player.
*/
template<int bsize>
int hex_rand_moves(
        GameState::HexState<bsize> &state,
        int turns,
        GameState::PLAYERS whose_turn
) {
    assert(whose_turn != GameState::PLAYER_NONE);
    static std::minstd_rand0 rand(0);
    GameState::Action actions[bsize * bsize];
    state.default_iter_whose = whose_turn;
    GameState::PLAYERS temp =
        state.default_iter_whose == GameState::PLAYER_ONE ? 
        GameState::PLAYER_TWO : GameState::PLAYER_ONE;
    
    while (turns-->0) {
        size_t count = std::copy(state.begin(), state.end(), &actions[0]) - &actions[0];
        if (count == 0) {
            return 1;
        }
        state.succeed(actions[rand() % count]);
        std::swap(state.default_iter_whose, temp);
    }
    return 0;

    // std::vector<GameState::HexState<bsize>> a_list;
    // while (turns-->0) {
    //     state.get_actions(a_list);
    //     if (a_list.empty()) {
    //         return 1;
    //     }
    //     state.succeed_in_place(a_list.at(rand() % a_list.size()));
    //     a_list.clear();
    // }

    // return 0;
}

}

#endif // !HEX_AI_GAMESOLVE_HEXUTIL_HPP

