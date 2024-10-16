/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Players/util.hpp"
#include <cstdio>
#include <iostream>

using GameState::HexState::PLAYERS::PLAYER_ONE;
using GameState::HexState::PLAYERS::PLAYER_TWO;
using GameState::HexState::PLAYERS::PLAYER_NONE;

int main (int argc, char *argv[]) {
    GameState::HexState state;
    GameState::HexState::Action a;

    game_loop:
    Players::get_action(a);
    a.whose = a.whose == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
    state.succeed_in_place(a);
    std::cout << state;
    switch (state.who_won()) {
        case PLAYER_NONE:
            goto game_loop;
        case PLAYER_ONE:
            std::cout << "Player 1 wins.\n";
            break;
        case PLAYER_TWO:
            std::cout << "Player 2 wins.\n";
    }
    return 0;
}

