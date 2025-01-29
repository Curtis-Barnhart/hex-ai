/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtest/gtest.h>
#include <iterator>

#include "hex-ai/GameSolve/HexUtil.hpp"
#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/enums.hpp"

using GameState::PLAYER_ONE;
using GameState::PLAYER_TWO;

TEST(test_hex_rand_moves, take_no_moves) {
    GameState::HexState<4> state;
    EXPECT_EQ(GameSolve::hex_rand_moves(state, 0, PLAYER_ONE), 0)
        << "After not cutting the random moves short, hex_rand_moves did not return 0.\n";
    EXPECT_EQ(std::distance(state.begin(), state.end()), 16)
        << "After making 0 moves, there were not 16 moves left on an empty 4x4 board.\n";
}

TEST(test_hex_rand_moves, take_moves_no_win) {
    GameState::HexState<4> state;

    for (int x = 4096; x-->0;) {
        state = GameState::HexState<4>();
        ASSERT_EQ(GameSolve::hex_rand_moves(state, 6, PLAYER_ONE), 0)
            << "After not cutting the random moves short, hex_rand_moves did not return 0.\n";
        ASSERT_EQ(std::distance(state.begin(), state.end()), 10)
            << "After making 6 moves, there were not 10 moves left on an empty 4x4 board.\n";
    }
}

TEST(test_hex_rand_moves, take_moves_no_win_preexisting) {
    GameState::HexState<4> state;

    for (int x = 4096; x-->0;) {
        state = GameState::HexState<4>();
        state.succeed({1, 0, PLAYER_ONE});
        state.succeed({2, 0, PLAYER_ONE});
        state.succeed({3, 0, PLAYER_ONE});
        state.succeed({0, 0, PLAYER_TWO});
        state.succeed({0, 1, PLAYER_TWO});
        state.succeed({0, 2, PLAYER_TWO});
        state.succeed({0, 3, PLAYER_TWO});

        ASSERT_EQ(GameSolve::hex_rand_moves(state, 4, PLAYER_TWO), 0)
            << "After not cutting the random moves short, hex_rand_moves did not return 0.\n";
        ASSERT_EQ(std::distance(state.begin(), state.end()), 5)
            << "After making 11 moves total, there were not 5 moves left on an empty 4x4 board.\n";

        ASSERT_EQ(state[1][0], PLAYER_ONE);
        ASSERT_EQ(state[2][0], PLAYER_ONE);
        ASSERT_EQ(state[3][0], PLAYER_ONE);
        ASSERT_EQ(state[0][0], PLAYER_TWO);
        ASSERT_EQ(state[0][1], PLAYER_TWO);
        ASSERT_EQ(state[0][2], PLAYER_TWO);
        ASSERT_EQ(state[0][3], PLAYER_TWO);
    }
}

TEST(test_hex_rand_moves, take_moves_win_preexisting) {
    GameState::HexState<4> state, copy;

    for (int x = 4096; x-->0;) {
        state = GameState::HexState<4>();
        state.succeed({2, 1, PLAYER_ONE});
        state.succeed({2, 2, PLAYER_ONE});
        state.succeed({2, 3, PLAYER_ONE});
        state.succeed({0, 0, PLAYER_TWO});
        state.succeed({1, 0, PLAYER_TWO});
        state.succeed({2, 0, PLAYER_TWO});
        state.succeed({3, 0, PLAYER_TWO});
        copy = state;

        ASSERT_EQ(GameSolve::hex_rand_moves(state, 4, PLAYER_TWO), 1)
            << "After cutting the random moves short, hex_rand_moves did not return 1.\n";
        ASSERT_EQ(std::distance(state.begin(), state.end()), 0)
            << "After starting with a winning board with no moves, there are somehow moves.\n";

        ASSERT_EQ(state, copy)
            << "hex_rand_moves changed a board that had no moves.\n";
    }
}

