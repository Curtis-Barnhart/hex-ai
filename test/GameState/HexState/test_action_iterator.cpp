/*
 * Copyright 2025 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdio>
#include <iterator>

#include <gtest/gtest.h>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/enums.hpp"
#include "hex-ai/GameState/Action.hpp"

using GameState::HexState;
using GameState::Action;
using GameState::PLAYERS::PLAYER_NONE;
using GameState::PLAYERS::PLAYER_ONE;
using GameState::PLAYERS::PLAYER_TWO;

TEST(HexState1_ActionIterator, DefaultPlayer) {
    HexState<1> state;
    HexState<1>::ActionIterator start = state.begin(),
                                end = state.end();
    
    EXPECT_GT(std::distance(start, end), 0)
        << "Empty 1x1 did not have any actions.\n";
    EXPECT_EQ(*start, Action(0, 0, PLAYER_NONE))
        << "Empty 1x1 did not have (0, 0, NONE) as first available move.\n";
    EXPECT_EQ(std::distance(start, end), 1)
        << "Empty 1x1 did not have 1 move.\n";
}

TEST(HexState1_GetActions, ExplicitPlayer) {
    HexState<1> state;
    state.default_iter_whose = PLAYER_NONE;
    HexState<1>::ActionIterator start_none = state.begin();
    state.default_iter_whose = PLAYER_ONE;
    HexState<1>::ActionIterator start_one = state.begin();
    state.default_iter_whose = PLAYER_TWO;
    HexState<1>::ActionIterator start_two = state.begin();

    HexState<1>::ActionIterator end = state.end();
    
    EXPECT_GT(std::distance(start_none, end), 0)
        << "Empty 1x1 did not have any actions.\n";
    EXPECT_GT(std::distance(start_one, end), 0)
        << "Empty 1x1 did not have any actions.\n";
    EXPECT_GT(std::distance(start_two, end), 0)
        << "Empty 1x1 did not have any actions.\n";

    EXPECT_EQ(*start_none, Action(0, 0, PLAYER_NONE))
        << "Empty 1x1 did not have (0, 0, NONE) as first available move.\n";
    EXPECT_EQ(*start_one, Action(0, 0, PLAYER_ONE))
        << "Empty 1x1 did not have (0, 0, ONE) as first available move.\n";
    EXPECT_EQ(*start_two, Action(0, 0, PLAYER_TWO))
        << "Empty 1x1 did not have (0, 0, TWO) as first available move.\n";

    EXPECT_EQ(std::distance(start_none, end), 1)
        << "Empty 1x1 had more than 1 move.\n";
    EXPECT_EQ(std::distance(start_one, end), 1)
        << "Empty 1x1 had more than 1 move.\n";
    EXPECT_EQ(std::distance(start_two, end), 1)
        << "Empty 1x1 had more than 1 move.\n";
}

TEST(HexState1_ActionIterator, NoMoves) {
    HexState<1> state;
    state.succeed({0, 0, PLAYER_ONE});
    HexState<1>::ActionIterator start = state.begin(),
                                end = state.end();
    
    EXPECT_EQ(std::distance(start, end), 0)
        << "Full 1x1 had moves available.\n";
}

TEST(HexState4_GetActions, EmptyBoard) {
    HexState<4> state;
    HexState<4>::ActionIterator start = state.begin(),
                                end = state.end();

    EXPECT_LT(start, end)
        << "Empty 4x4 did not have any actions.\n";
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            EXPECT_EQ(*start, Action(x, y, PLAYER_NONE))
                << "Action at (" << x << ", " << y << ") had incorrect value.\n";
            ++start;
        }
    }
    EXPECT_GE(start, end)
        << "Empty 4x4 had more than 16 moves.\n";
}

TEST(HexState4_GetActions, NonemptyBoard) {
    HexState<4> state;

    state.succeed(Action(0, 1, PLAYER_ONE));
    state.succeed(Action(0, 2, PLAYER_TWO));
    state.succeed(Action(1, 2, PLAYER_ONE));
    state.succeed(Action(1, 3, PLAYER_ONE));
    state.succeed(Action(2, 0, PLAYER_TWO));
    state.succeed(Action(2, 1, PLAYER_ONE));
    state.succeed(Action(2, 2, PLAYER_TWO));
    state.succeed(Action(2, 3, PLAYER_TWO));
    Action expected[8] = {
        {0, 0, PLAYER_NONE},
        {0, 3, PLAYER_NONE},
        {1, 0, PLAYER_NONE},
        {1, 1, PLAYER_NONE},
        {3, 0, PLAYER_NONE},
        {3, 1, PLAYER_NONE},
        {3, 2, PLAYER_NONE},
        {3, 3, PLAYER_NONE},
    };

    HexState<4>::ActionIterator start = state.begin(),
                                end = state.end();
    EXPECT_LT(start, end)
        << "Half empty 4x4 did not have any actions.\n";
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(*start, expected[i])
            << "Action at " << i << " had bad value.\n";
        ++start;
    }
    EXPECT_GE(start, end)
        << "Half empty 4x4 had more than 8 moves.\n";
}

TEST(HexState4_GetActions, WonBoard) {
    HexState<4> state;
    state.succeed({ 0, 0, PLAYER_ONE });
    state.succeed({ 1, 0, PLAYER_ONE });
    state.succeed({ 2, 0, PLAYER_ONE });
    state.succeed({ 3, 0, PLAYER_ONE });

    HexState<4>::ActionIterator start = state.begin(),
                                end = state.end();
    EXPECT_LT(start, end)
        << "3/4 empty 4x4 did not have any actions.\n";

    for (int x = 0; x < 4; ++x) {
        for (int y = 1; y < 4; ++y) {
            EXPECT_EQ(*start, Action(x, y, PLAYER_NONE))
                << "Action at (" << x << ", " << y << ") had bad value.\n";
            ++start;
        }
    }

    EXPECT_GE(start, end)
        << "3/4 empty 4x4 had more than 12 moves.\n";
}

TEST(HexState4_GetActions, SetPlayerByState) {
    HexState<4> state;
    for (const Action &a : state) {
        EXPECT_EQ(a.whose, PLAYER_NONE);
    }

    state.default_iter_whose = PLAYER_ONE;
    for (const Action &a : state) {
        EXPECT_EQ(a.whose, PLAYER_ONE);
    }

    state.default_iter_whose = PLAYER_TWO;
    for (const Action &a : state) {
        EXPECT_EQ(a.whose, PLAYER_TWO);
    }
}

