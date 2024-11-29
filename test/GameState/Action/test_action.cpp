/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtest/gtest.h>

#include "hex-ai/GameState/Action.hpp"
#include "hex-ai/GameState/enums.hpp"

using GameState::Action;

TEST(Action_construction, no_arg) {
    Action default_con, expected(0, 0, GameState::PLAYER_NONE);
    EXPECT_EQ(default_con, expected)
        << "Default constructor did not make Action (0, 0, NONE).\n";
}

TEST(Action_construction, two_arg) {
    Action default_con(1), expected(1, 0, GameState::PLAYER_NONE);
    EXPECT_EQ(default_con, expected)
        << "Constructor default for y and whose is not 0 and PLAYER_NONE.\n";
}

TEST(Action_construction, one_arg) {
    Action default_con(1, 3), expected(1, 3, GameState::PLAYER_NONE);
    EXPECT_EQ(default_con, expected)
        << "Constructor default for whose is not PLAYER_NONE.\n";
}

