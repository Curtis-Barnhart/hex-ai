/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtest/gtest.h>

#include "hex-ai/GameSolve/AlphaBeta.hpp"
#include "hex-ai/GameState/enums.hpp"

using GameState::PLAYER_NONE;
using GameState::PLAYER_ONE;
using GameState::PLAYER_TWO;

TEST(test_AlphaBeta, already_won_one) {
    GameSolve::AlphaBeta2PlayersCached<1> ab1 {5};
    GameSolve::AlphaBeta2PlayersCached<1> ab2 {5};
    GameState::HexState<1> state;
    state.succeed({0, 0, PLAYER_ONE});

    EXPECT_TRUE(ab1.one_wins_one_turn(state))
        << "P1 was not reported to be able to win a game they already won.\n";
    EXPECT_EQ(state[0][0], PLAYER_ONE)
        << "State was changed by AlphaBeta.\n";

    EXPECT_TRUE(ab2.one_wins_two_turn(state))
        << "P1 was not reported to be able to win a game they already won.\n";
    EXPECT_EQ(state[0][0], PLAYER_ONE)
        << "State was changed by AlphaBeta.\n";
}

TEST(test_AlphaBeta, already_won_two) {
    GameSolve::AlphaBeta2PlayersCached<1> ab1 {5};
    GameSolve::AlphaBeta2PlayersCached<1> ab2 {5};
    GameState::HexState<1> state;
    state.succeed({0, 0, PLAYER_TWO});

    EXPECT_FALSE(ab1.one_wins_one_turn(state))
        << "P1 was reported to be able to win a game they already lost.\n";
    EXPECT_EQ(state[0][0], PLAYER_TWO)
        << "State was changed by AlphaBeta.\n";

    EXPECT_FALSE(ab2.one_wins_two_turn(state))
        << "P1 was reported to be able to win a game they already lost.\n";
    EXPECT_EQ(state[0][0], PLAYER_TWO)
        << "State was changed by AlphaBeta.\n";
}

TEST(test_AlphaBeta, not_won) {
    GameSolve::AlphaBeta2PlayersCached<1> ab1 {5};
    GameSolve::AlphaBeta2PlayersCached<1> ab2 {5};
    GameState::HexState<1> state;

    EXPECT_TRUE(ab1.one_wins_one_turn(state))
        << "P1 was not reported to be able to win a game they can.\n";
    EXPECT_EQ(state[0][0], PLAYER_NONE)
        << "State was changed by AlphaBeta.\n";

    EXPECT_FALSE(ab2.one_wins_two_turn(state))
        << "P1 was reported to be able to win a game they cannot.\n";
    EXPECT_EQ(state[0][0], PLAYER_NONE)
        << "State was changed by AlphaBeta.\n";
}

