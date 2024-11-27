/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdio>

#include <gtest/gtest.h>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/enums.hpp"
#include "hex-ai/GameState/Action.hpp"

using GameState::HexState;
using GameState::PLAYERS::PLAYER_ONE;
using GameState::PLAYERS::PLAYER_TWO;
using GameState::PLAYERS::PLAYER_NONE;

/*************************************************
* Definitions                                    *
*************************************************/

/**
 * I think there are places (maybe when converting to files for Python to use)
 * where I rely on the numeric value of this enum??
 */
TEST(HexState_Definitions, PLAYERS_Value) {
    EXPECT_EQ(PLAYER_NONE, 0);
    EXPECT_EQ(PLAYER_ONE, 1);
    EXPECT_EQ(PLAYER_TWO, 2);
}

/*************************************************
* Constructing                                   *
*************************************************/

TEST(HexState_1_Constructor, EmptyConstructor) {
    HexState<1> state;

    EXPECT_EQ(state[0][0], PLAYER_NONE)
        << "Newly constructed board was not empty.";
}

TEST(HexState_1_Constructor, EmptyWin) {
    HexState<1> empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "Newly constructed board had a winner.";
}

TEST(HexState_4_Constructor, EmptyConstructor) {
    HexState<4> state;

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            EXPECT_EQ(state[x][y], PLAYER_NONE)
                << "Newly constructed board was not empty.";
        }
    }
}

TEST(HexState_9_Constructor, EmptyConstructor) {
    HexState<9> state;

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            EXPECT_EQ(state[x][y], PLAYER_NONE)
                << "Newly constructed board was not empty.";
        }
    }
}

TEST(HexState_4_Constructor, EmptyWin) {
    HexState<4> empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "Newly constructed board had a winner.";
}

TEST(HexState_9_Constructor, EmptyWin) {
    HexState<9> empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "Newly constructed board had a winner.";
}

/*************************************************
* Making Moves                                   *
*************************************************/

/*************************************************
* Checking Wins                                  *
*************************************************/

