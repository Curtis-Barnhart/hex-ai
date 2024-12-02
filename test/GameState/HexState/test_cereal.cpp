/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sstream>

#include <gtest/gtest.h>
#include <cereal/cereal.hpp>

#include "cereal/archives/binary.hpp"
#include "hex-ai/GameSolve/HexUtil.hpp"
#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/enums.hpp"

TEST(test_HexState_cereal_1, empty_state) {
    GameState::HexState<1> original, read;
    std::stringstream s;
    {
        cereal::BinaryOutputArchive arc(s);
        arc(original);
    }
    cereal::BinaryInputArchive arc(s);
    arc(read);
    EXPECT_EQ(read, original);
}

TEST(test_HexState_cereal_1, nonempty_state) {
    for (int x = 4096; x-->0;) {
        GameState::HexState<1> original, read;
        original.succeed({0, 0, GameState::PLAYER_TWO});
        std::stringstream s;
        {
            cereal::BinaryOutputArchive arc(s);
            arc(original);
        }
        cereal::BinaryInputArchive arc(s);
        arc(read);
        EXPECT_EQ(read, original);
    }
}


TEST(test_HexState_cereal_4, empty_state) {
    GameState::HexState<4> original, read;
    std::stringstream s;
    {
        cereal::BinaryOutputArchive arc(s);
        arc(original);
    }
    cereal::BinaryInputArchive arc(s);
    arc(read);
    EXPECT_EQ(read, original);
}

TEST(test_HexState_cereal_4, nonempty_state) {
    for (int x = 4096; x-->0;) {
        GameState::HexState<4> original, read;
        GameSolve::hex_rand_moves(original, 8, GameState::PLAYER_ONE);
        std::stringstream s;
        {
            cereal::BinaryOutputArchive arc(s);
            arc(original);
        }
        cereal::BinaryInputArchive arc(s);
        arc(read);
        EXPECT_EQ(read, original);
    }
}

TEST(test_HexState_cereal_9, empty_state) {
    GameState::HexState<9> original, read;
    std::stringstream s;
    {
        cereal::BinaryOutputArchive arc(s);
        arc(original);
    }
    cereal::BinaryInputArchive arc(s);
    arc(read);
    EXPECT_EQ(read, original);
}

TEST(test_HexState_cereal_9, nonempty_state) {
    for (int x = 4096; x-->0;) {
        GameState::HexState<9> original, read;
        GameSolve::hex_rand_moves(original, 20, GameState::PLAYER_ONE);
        std::stringstream s;
        {
            cereal::BinaryOutputArchive arc(s);
            arc(original);
        }
        cereal::BinaryInputArchive arc(s);
        arc(read);
        EXPECT_EQ(read, original);
    }
}

