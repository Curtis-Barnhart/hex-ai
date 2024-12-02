/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdint>
#include <random>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>
#include <cereal/archives/binary.hpp>

#include "hex-ai/GameSolve/HexUtil.hpp"
#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Io/GamestateBool0.hpp"
#include "hex-ai/Io/io_enums.hpp"

TEST(test_HexState_cereal_1, many_states) {
    std::minstd_rand0 rand(0);

    for (int x = 256; x-->0;) {
        std::stringstream s;
        std::vector<GameState::HexState<1>> states;
        std::vector<bool> bools;
        {
            Io::GamestateBool0Writer<1> writer(s);
            for (int y = 256; y-->0;) {
                states.emplace_back();
                GameSolve::hex_rand_moves(
                    states.back(),
                    rand() % 2,
                    rand() % 2 ? GameState::PLAYER_ONE : GameState::PLAYER_TWO
                );
                bool b = rand() % 2 == 0;
                bools.push_back(b);
                EXPECT_EQ(writer.read_err(), Io::GamestateBool0Writer<1>::CLEAR);
                writer.push(states.back(), b);
            }
        }

        {
            cereal::BinaryInputArchive arc(s);
            uint8_t i;
            arc(i);
            EXPECT_EQ(i, Io::GAMESTATE_BOOL);
            arc(i);
            EXPECT_EQ(i, 0);
            arc(i);
            EXPECT_EQ(i, 1);
        }

        Io::GamestateBool0Reader<1> reader(s);
        for (int y = 0; y < 256; ++y) {
            GameState::HexState<1> state;
            bool b;
            EXPECT_EQ(reader.read_err(), Io::GamestateBool0Reader<1>::CLEAR);
            reader.pop(state, b);

            EXPECT_EQ(state, states.at(y));
            EXPECT_EQ(b, bools.at(y));
        }
        EXPECT_EQ(reader.read_err(), Io::GamestateBool0Reader<1>::EMPTY);
    }
}

TEST(test_HexState_cereal_4, many_states) {
    std::minstd_rand0 rand(0);

    for (int x = 256; x-->0;) {
        std::stringstream s;
        std::vector<GameState::HexState<4>> states;
        std::vector<bool> bools;
        {
            Io::GamestateBool0Writer<4> writer(s);
            for (int y = 256; y-->0;) {
                states.emplace_back();
                GameSolve::hex_rand_moves(
                    states.back(),
                    rand() % 2,
                    rand() % 2 ? GameState::PLAYER_ONE : GameState::PLAYER_TWO
                );
                bool b = rand() % 2 == 0;
                bools.push_back(b);
                EXPECT_EQ(writer.read_err(), Io::GamestateBool0Writer<4>::CLEAR);
                writer.push(states.back(), b);
            }
        }

        {
            cereal::BinaryInputArchive arc(s);
            uint8_t i;
            arc(i);
            EXPECT_EQ(i, Io::GAMESTATE_BOOL);
            arc(i);
            EXPECT_EQ(i, 0);
            arc(i);
            EXPECT_EQ(i, 4);
        }

        Io::GamestateBool0Reader<4> reader(s);
        for (int y = 0; y < 256; ++y) {
            GameState::HexState<4> state;
            bool b;
            EXPECT_EQ(reader.read_err(), Io::GamestateBool0Reader<4>::CLEAR);
            reader.pop(state, b);

            EXPECT_EQ(state, states.at(y));
            EXPECT_EQ(b, bools.at(y));
        }
        EXPECT_EQ(reader.read_err(), Io::GamestateBool0Reader<4>::EMPTY);
    }
}

TEST(test_HexState_cereal_9, many_states) {
    std::minstd_rand0 rand(0);

    for (int x = 256; x-->0;) {
        std::stringstream s;
        std::vector<GameState::HexState<9>> states;
        std::vector<bool> bools;
        {
            Io::GamestateBool0Writer<9> writer(s);
            for (int y = 256; y-->0;) {
                states.emplace_back();
                GameSolve::hex_rand_moves(
                    states.back(),
                    rand() % 2,
                    rand() % 2 ? GameState::PLAYER_ONE : GameState::PLAYER_TWO
                );
                bool b = rand() % 2 == 0;
                bools.push_back(b);
                EXPECT_EQ(writer.read_err(), Io::GamestateBool0Writer<9>::CLEAR);
                writer.push(states.back(), b);
            }
        }

        {
            cereal::BinaryInputArchive arc(s);
            uint8_t i;
            arc(i);
            EXPECT_EQ(i, Io::GAMESTATE_BOOL);
            arc(i);
            EXPECT_EQ(i, 0);
            arc(i);
            EXPECT_EQ(i, 9);
        }

        Io::GamestateBool0Reader<9> reader(s);
        for (int y = 0; y < 256; ++y) {
            GameState::HexState<9> state;
            bool b;
            EXPECT_EQ(reader.read_err(), Io::GamestateBool0Reader<9>::CLEAR);
            reader.pop(state, b);

            EXPECT_EQ(state, states.at(y));
            EXPECT_EQ(b, bools.at(y));
        }
        EXPECT_EQ(reader.read_err(), Io::GamestateBool0Reader<9>::EMPTY);
    }
}

