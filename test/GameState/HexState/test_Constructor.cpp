/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdio>

#include <gtest/gtest.h>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/enums.hpp"
#include "hex-ai/GameState/Action.hpp"

using GameState::HexState;
using GameState::PLAYERS::PLAYER_NONE;


TEST(HexState_1_Constructor, IsEmtpy) {
    HexState<1> state;

    EXPECT_EQ(state[0][0], PLAYER_NONE)
        << "Newly constructed board was not empty.";
}

TEST(HexState_1_Constructor, EmptyWin) {
    HexState<1> empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "Newly constructed board had a winner.";
}

TEST(HexState_1_Constructor, Moves) {
    HexState<1> empty;
    std::vector<GameState::Action> v;

    empty.get_actions(v);

    EXPECT_EQ(v.size(), 1)
        << "Newly constructed 1x1 board did not have 1 available move.";
}

TEST(HexState_4_Constructor, IsEmtpy) {
    HexState<4> state;

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
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

TEST(HexState_4_Constructor, Moves) {
    HexState<4> empty;
    std::vector<GameState::Action> v;

    empty.get_actions(v);

    EXPECT_EQ(v.size(), 16)
        << "Newly constructed 4x4 board did not have 16 available moves.";
}

TEST(HexState_9_Constructor, IsEmtpy) {
    HexState<9> state;

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            EXPECT_EQ(state[x][y], PLAYER_NONE)
                << "Newly constructed board was not empty.";
        }
    }
}

TEST(HexState_9_Constructor, EmptyWin) {
    HexState<9> empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "Newly constructed board had a winner.";
}

TEST(HexState_9_Constructor, Moves) {
    HexState<9> empty;
    std::vector<GameState::Action> v;

    empty.get_actions(v);

    EXPECT_EQ(v.size(), 81)
        << "Newly constructed 9x9 board did not have 81 available moves.";
}

