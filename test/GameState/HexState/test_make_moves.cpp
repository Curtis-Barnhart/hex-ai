/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtest/gtest.h>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/enums.hpp"
#include "hex-ai/GameState/Action.hpp"

using GameState::HexState;
using GameState::PLAYERS::PLAYER_NONE;
using GameState::PLAYERS::PLAYER_ONE;
using GameState::PLAYERS::PLAYER_TWO;

TEST(HexState_1_moves, none) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_NONE});

    EXPECT_EQ(state[0][0], PLAYER_NONE);
}

TEST(HexState_1_moves, one) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_ONE});

    EXPECT_EQ(state[0][0], PLAYER_ONE);
}

TEST(HexState_1_moves, two) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_TWO});

    EXPECT_EQ(state[0][0], PLAYER_TWO);
}

TEST(HexState_1_moves, multiple_two) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_ONE});
    state.succeed({0, 0, PLAYER_TWO});

    EXPECT_EQ(state[0][0], PLAYER_TWO);
}

TEST(HexState_1_moves, multiple_one) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_TWO});
    state.succeed({0, 0, PLAYER_ONE});

    EXPECT_EQ(state[0][0], PLAYER_ONE);
}

TEST(HexState_4_moves, many_moves) {
    HexState<4> state;
    state.succeed({0, 1, PLAYER_NONE});
    state.succeed({2, 3, PLAYER_TWO});
    state.succeed({0, 1, PLAYER_TWO});
    state.succeed({3, 2, PLAYER_TWO});
    state.succeed({2, 2, PLAYER_NONE});
    state.succeed({3, 1, PLAYER_ONE});
    state.succeed({0, 3, PLAYER_ONE});
    state.succeed({2, 3, PLAYER_NONE});
    state.succeed({1, 1, PLAYER_NONE});
    state.succeed({0, 0, PLAYER_NONE});
    state.succeed({3, 3, PLAYER_TWO});
    state.succeed({2, 1, PLAYER_ONE});
    state.succeed({0, 2, PLAYER_TWO});
    state.succeed({3, 2, PLAYER_TWO});
    state.succeed({2, 2, PLAYER_ONE});
    state.succeed({0, 3, PLAYER_TWO});
    state.succeed({0, 3, PLAYER_TWO});
    state.succeed({0, 1, PLAYER_TWO});
    state.succeed({3, 3, PLAYER_NONE});
    state.succeed({0, 0, PLAYER_ONE});
    state.succeed({0, 0, PLAYER_ONE});
    state.succeed({1, 3, PLAYER_NONE});
    state.succeed({1, 0, PLAYER_NONE});
    state.succeed({2, 1, PLAYER_ONE});
    state.succeed({1, 0, PLAYER_TWO});
    state.succeed({3, 2, PLAYER_NONE});
    state.succeed({3, 2, PLAYER_TWO});
    state.succeed({0, 3, PLAYER_ONE});
    state.succeed({2, 2, PLAYER_NONE});
    state.succeed({2, 0, PLAYER_TWO});
    state.succeed({3, 3, PLAYER_ONE});
    state.succeed({1, 1, PLAYER_NONE});

    EXPECT_EQ(state[0][0], PLAYER_ONE);
    EXPECT_EQ(state[0][1], PLAYER_TWO);
    EXPECT_EQ(state[0][2], PLAYER_TWO);
    EXPECT_EQ(state[0][3], PLAYER_ONE);
    EXPECT_EQ(state[1][0], PLAYER_TWO);
    EXPECT_EQ(state[1][1], PLAYER_NONE);
    EXPECT_EQ(state[1][2], PLAYER_NONE);
    EXPECT_EQ(state[1][3], PLAYER_NONE);
    EXPECT_EQ(state[2][0], PLAYER_TWO);
    EXPECT_EQ(state[2][1], PLAYER_ONE);
    EXPECT_EQ(state[2][2], PLAYER_NONE);
    EXPECT_EQ(state[2][3], PLAYER_NONE);
    EXPECT_EQ(state[3][0], PLAYER_NONE);
    EXPECT_EQ(state[3][1], PLAYER_ONE);
    EXPECT_EQ(state[3][2], PLAYER_TWO);
    EXPECT_EQ(state[3][3], PLAYER_ONE);
}

