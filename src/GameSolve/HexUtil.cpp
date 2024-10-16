/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <random>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameSolve/HexUtil.hpp"

int GameSolve::hex_rand_moves(GameState::HexState &state, int turns) {
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

