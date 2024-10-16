/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PLAYERS_UTIL_HPP
#define PLAYERS_UTIL_HPP

#include "hex-ai/GameState/HexState.hpp"

namespace Players {

void get_action(GameState::HexState::Action &a);

}

#endif // !PLAYERS_UTIL_HPP

