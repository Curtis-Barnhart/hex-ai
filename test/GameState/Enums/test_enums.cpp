/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtest/gtest.h>

#include "hex-ai/GameState/enums.hpp"

using GameState::PLAYERS::PLAYER_ONE;
using GameState::PLAYERS::PLAYER_TWO;
using GameState::PLAYERS::PLAYER_NONE;

/*************************************************
* Definitions                                    *
*************************************************/

/**
 * I think there are places (maybe when converting to files for Python to use)
 * where I rely on the numeric value of this enum??
 * It's actually when I'm writing them out to hex-ai files,
 * but that may change in the future.
 */
TEST(HexState_Definitions, PLAYERS_Value) {
    EXPECT_EQ(PLAYER_NONE, 0);
    EXPECT_EQ(PLAYER_ONE, 1);
    EXPECT_EQ(PLAYER_TWO, 2);
}

