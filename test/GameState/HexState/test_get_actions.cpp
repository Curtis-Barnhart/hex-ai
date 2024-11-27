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
using GameState::Action;
using GameState::PLAYERS::PLAYER_NONE;
using GameState::PLAYERS::PLAYER_ONE;
using GameState::PLAYERS::PLAYER_TWO;

TEST(HexState1_GetActions, DefaultPlayer) {
    HexState<1> state;
    std::vector<Action> v;

    state.get_actions(v);

    EXPECT_EQ(v.size(), 1)
        << "Empty 1x1 board did not have exactly 1 available move.";
    EXPECT_EQ(v.at(0), Action(0, 0, PLAYER_NONE))
        << "Empty 1x1 board did not have the available move (0, 0, NONE).";
}

TEST(HexState1_GetActions, NonemptyVector) {
    HexState<1> state;
    std::vector<Action> v;

    state.get_actions(v);
    state.get_actions(v);
    state.get_actions(v);

    EXPECT_EQ(v.size(), 3)
        << "HexState::get_actions did not add its elements at the end of a vector.";
}

TEST(HexState1_GetActions, ExplicitPlayer) {
    HexState<1> state;
    std::vector<Action> v;

    state.get_actions(v, PLAYER_NONE);
    EXPECT_EQ(v.at(0), Action(0, 0, PLAYER_NONE))
        << "HexState::get_actions did not use provided player.";
    v.clear();

    state.get_actions(v, PLAYER_ONE);
    EXPECT_EQ(v.at(0), Action(0, 0, PLAYER_ONE))
        << "HexState::get_actions did not use provided player.";
    v.clear();

    state.get_actions(v, PLAYER_TWO);
    EXPECT_EQ(v.at(0), Action(0, 0, PLAYER_TWO))
        << "HexState::get_actions did not use provided player.";
}

TEST(HexState4_GetActions, EmptyBoard) {
    HexState<4> state;
    std::vector<Action> v;
    state.get_actions(v);

    EXPECT_EQ(v.size(), 16)
        << "get_actions returned the wrong amount of actions.";

    int i = 0;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            EXPECT_EQ(v.at(i++), Action(x, y, PLAYER_NONE))
                << "Action at " << (i - 1) << " had incorrect value.";
        }
    }
}

TEST(HexState4_GetActions, NonemptyBoard) {
    HexState<4> state;
    std::vector<Action> v;

    state.succeed(Action(0, 1, PLAYER_ONE));
    state.succeed(Action(0, 2, PLAYER_TWO));
    state.succeed(Action(1, 2, PLAYER_ONE));
    state.succeed(Action(1, 3, PLAYER_ONE));
    state.succeed(Action(2, 0, PLAYER_TWO));
    state.succeed(Action(2, 1, PLAYER_ONE));
    state.succeed(Action(2, 2, PLAYER_TWO));
    state.succeed(Action(2, 3, PLAYER_TWO));
    Action expected[8] = {
        { 0, 0, PLAYER_NONE },
        { 0, 3, PLAYER_NONE },
        { 1, 0, PLAYER_NONE },
        { 1, 1, PLAYER_NONE },
        { 3, 0, PLAYER_NONE },
        { 3, 1, PLAYER_NONE },
        { 3, 2, PLAYER_NONE },
        { 3, 3, PLAYER_NONE },
    };

    state.get_actions(v);
    EXPECT_EQ(v.size(), 8)
        << "get_actions returned the wrong amount of actions.";

    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(v.at(i), expected[i])
            << "Action at " << i << " had bad value.";
    }
}

TEST(HexState4_GetActions, WonBoard) {
    HexState<4> state;
    std::vector<Action> v;
    state.succeed({ 0, 0, PLAYER_ONE });
    state.succeed({ 0, 1, PLAYER_ONE });
    state.succeed({ 0, 2, PLAYER_ONE });
    state.succeed({ 0, 3, PLAYER_ONE });

    state.get_actions(v);
    EXPECT_EQ(v.size(), 0)
        << "get_actions returned the wrong amount of actions.";
}

