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

TEST(HexState_4_Constructor, EmptyWin) {
    HexState<4> empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "Newly constructed board had a winner.";
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

TEST(HexState_9_Constructor, EmptyWin) {
    HexState<9> empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "Newly constructed board had a winner.";
}

/*************************************************
* Making Moves                                   *
*************************************************/

TEST(HexState_1_Moves, NormalMovesVector) {

}

TEST(HexState_1_Moves, SillyMovesVector) {

}

TEST(HexState_1_Moves, NormalMovesIterator) {

}

TEST(HexState_1_Moves, SillyMovesIterator) {

}

TEST(HexState_4_Moves, NormalMovesVector) {

}

TEST(HexState_4_Moves, SillyMovesVector) {

}

TEST(HexState_4_Moves, NormalMovesIterator) {

}

TEST(HexState_4_Moves, SillyMovesIterator) {

}

TEST(HexState_9_Moves, NormalMovesVector) {

}

TEST(HexState_9_Moves, SillyMovesVector) {

}

TEST(HexState_9_Moves, NormalMovesIterator) {

}

TEST(HexState_9_Moves, SillyMovesIterator) {

}

/*************************************************
* Checking Wins                                  *
*************************************************/

