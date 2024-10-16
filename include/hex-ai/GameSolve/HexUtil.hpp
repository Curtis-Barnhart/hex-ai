/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef GAMESOLVE_HEXUTIL_HPP
#define GAMESOLVE_HEXUTIL_HPP

#include "hex-ai/GameState/HexState.hpp"

namespace GameSolve {

/**
* Attempts to play `turns` random turns on a HexState board.
* @param state HexState instance to make random moves on.
* @param turns amount of random moves to make.
* @return 0 if the exact amount of turns were made.
*         1 if the amount of turns that were made were less than `turns`
*         due to the game having been won by any player.
*/
int hex_rand_moves(GameState::HexState &state, int turns);

}

#endif // !GAMESOLVE_HEXUTIL_HPP

