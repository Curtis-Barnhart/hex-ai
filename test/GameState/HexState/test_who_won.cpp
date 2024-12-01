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

TEST(HexState_1_winning, no_win) {
    HexState<1> state;

    EXPECT_EQ(state.who_won(), PLAYER_NONE)
        << "Empty 1x1 was won by someone.\n";
}

TEST(HexState_1_winning, one_win) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_ONE});

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "1x1 won by ONE was not detected as won by ONE.\n";
}

TEST(HexState_1_winning, two_win) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_TWO});

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "1x1 won by TWO was not detected as won by TWO.\n";
}

TEST(HexState_4_winning, no_win) {
    HexState<4> state;

    EXPECT_EQ(state.who_won(), PLAYER_NONE)
        << "Empty 4x4 was won by someone.\n";
}

TEST(HexState_4_winning, one_win_straight_x_0) {
    HexState<4> state;
    state.succeed({0, 0, PLAYER_ONE});
    state.succeed({0, 1, PLAYER_ONE});
    state.succeed({0, 2, PLAYER_ONE});
    state.succeed({0, 3, PLAYER_ONE});

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "4x4 won by ONE was not detected as won by ONE.\n";
}

TEST(HexState_4_winning, one_win_straight_x_1) {
    HexState<4> state;
    state.succeed({1, 0, PLAYER_ONE});
    state.succeed({1, 1, PLAYER_ONE});
    state.succeed({1, 2, PLAYER_ONE});
    state.succeed({1, 3, PLAYER_ONE});

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "4x4 won by ONE was not detected as won by ONE.\n";
}

TEST(HexState_4_winning, one_win_straight_x_2) {
    HexState<4> state;
    state.succeed({2, 0, PLAYER_ONE});
    state.succeed({2, 1, PLAYER_ONE});
    state.succeed({2, 2, PLAYER_ONE});
    state.succeed({2, 3, PLAYER_ONE});

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "4x4 won by ONE was not detected as won by ONE.\n";
}

TEST(HexState_4_winning, one_win_straight_x_3) {
    HexState<4> state;
    state.succeed({3, 0, PLAYER_ONE});
    state.succeed({3, 1, PLAYER_ONE});
    state.succeed({3, 2, PLAYER_ONE});
    state.succeed({3, 3, PLAYER_ONE});

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "4x4 won by ONE was not detected as won by ONE.\n";
}

TEST(HexState_4_winning, two_win_straight_y_0) {
    HexState<4> state;
    state.succeed({0, 0, PLAYER_TWO});
    state.succeed({1, 0, PLAYER_TWO});
    state.succeed({2, 0, PLAYER_TWO});
    state.succeed({3, 0, PLAYER_TWO});

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "4x4 won by TWO was not detected as won by TWO.\n";
}

TEST(HexState_4_winning, two_win_straight_y_1) {
    HexState<4> state;
    state.succeed({0, 1, PLAYER_TWO});
    state.succeed({1, 1, PLAYER_TWO});
    state.succeed({2, 1, PLAYER_TWO});
    state.succeed({3, 1, PLAYER_TWO});

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "4x4 won by TWO was not detected as won by TWO.\n";
}

TEST(HexState_4_winning, two_win_straight_y_2) {
    HexState<4> state;
    state.succeed({0, 2, PLAYER_TWO});
    state.succeed({1, 2, PLAYER_TWO});
    state.succeed({2, 2, PLAYER_TWO});
    state.succeed({3, 2, PLAYER_TWO});

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "4x4 won by TWO was not detected as won by TWO.\n";
}

TEST(HexState_4_winning, two_win_straight_y_3) {
    HexState<4> state;
    state.succeed({0, 3, PLAYER_TWO});
    state.succeed({1, 3, PLAYER_TWO});
    state.succeed({2, 3, PLAYER_TWO});
    state.succeed({3, 3, PLAYER_TWO});

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "4x4 won by TWO was not detected as won by TWO.\n";
}

TEST(HexState_4_winning, one_win_curvy) {
    HexState<4> state;
    state.succeed({0, 0, PLAYER_ONE});
    state.succeed({0, 1, PLAYER_ONE});
    state.succeed({1, 1, PLAYER_ONE});
    state.succeed({2, 1, PLAYER_ONE});
    state.succeed({3, 1, PLAYER_ONE});
    state.succeed({3, 2, PLAYER_ONE});
    state.succeed({2, 3, PLAYER_ONE});

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "4x4 won by ONE was not detected as won by ONE.\n";
}

TEST(HexState_4_winning, one_win_curvy_two_filled) {
    HexState<4> state;
    for (unsigned char x = 0; x < 4; x++) {
        for (unsigned char y = 0; y < 4; y++) {
            state.succeed({x, y, PLAYER_TWO});
        }
    }
    state.succeed({0, 0, PLAYER_ONE});
    state.succeed({0, 1, PLAYER_ONE});
    state.succeed({1, 1, PLAYER_ONE});
    state.succeed({2, 1, PLAYER_ONE});
    state.succeed({3, 1, PLAYER_ONE});
    state.succeed({3, 2, PLAYER_ONE});
    state.succeed({2, 3, PLAYER_ONE});

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "4x4 won by ONE was not detected as won by ONE.\n";
}

TEST(HexState_4_winning, two_win_curvy) {
    HexState<4> state;
    state.succeed({0, 0, PLAYER_TWO});
    state.succeed({1, 0, PLAYER_TWO});
    state.succeed({1, 1, PLAYER_TWO});
    state.succeed({1, 2, PLAYER_TWO});
    state.succeed({1, 3, PLAYER_TWO});
    state.succeed({2, 3, PLAYER_TWO});
    state.succeed({3, 2, PLAYER_TWO});

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "4x4 won by TWO was not detected as won by TWO.\n";
}

TEST(HexState_4_winning, two_win_curvy_one_filled) {
    HexState<4> state;
    for (unsigned char x = 0; x < 4; x++) {
        for (unsigned char y = 0; y < 4; y++) {
            state.succeed({x, y, PLAYER_ONE});
        }
    }
    state.succeed({0, 0, PLAYER_TWO});
    state.succeed({1, 0, PLAYER_TWO});
    state.succeed({1, 1, PLAYER_TWO});
    state.succeed({1, 2, PLAYER_TWO});
    state.succeed({1, 3, PLAYER_TWO});
    state.succeed({2, 3, PLAYER_TWO});
    state.succeed({3, 2, PLAYER_TWO});

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "4x4 won by TWO was not detected as won by TWO.\n";
}

TEST(HexState_4_winning, no_wins_curvy) {
    HexState<4> state;
    state.succeed({0, 0, PLAYER_ONE});
    state.succeed({0, 1, PLAYER_ONE});
    state.succeed({1, 1, PLAYER_ONE});
    state.succeed({2, 1, PLAYER_ONE});
    state.succeed({3, 1, PLAYER_ONE});
    state.succeed({3, 2, PLAYER_ONE});
    state.succeed({2, 3, PLAYER_ONE});
    state.succeed({0, 0, PLAYER_TWO});
    state.succeed({1, 0, PLAYER_TWO});
    state.succeed({1, 1, PLAYER_TWO});
    state.succeed({1, 2, PLAYER_TWO});
    state.succeed({1, 3, PLAYER_TWO});
    state.succeed({2, 3, PLAYER_TWO});
    state.succeed({3, 2, PLAYER_TWO});
    state.succeed({1, 1, PLAYER_NONE});

    EXPECT_EQ(state.who_won(), PLAYER_NONE)
        << "4x4 won by NONE was not detected as won by NONE.\n";
}

