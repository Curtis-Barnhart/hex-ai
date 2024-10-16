/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtest/gtest.h>
#include <sstream>
#include <cstdio>

#include <cereal/archives/binary.hpp>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameSolve/HexUtil.hpp"

using GameState::HexState;

#ifndef HEX_AI_TESTING_DIR
    #define HEX_AI_TESTING_DIR "./data"
#endif // !HEX_AI_TESTING_DIR

/*************************************************
* HexState Instance Reads                        *
*************************************************/

TEST(HexState_SingleIO, Empty) {
    HexState written, read;
    std::stringstream ss;

    {
        cereal::BinaryOutputArchive ocer(ss);
        ocer(written);
    }
    {
        cereal::BinaryInputArchive icer(ss);
        icer(read);
    }

    EXPECT_EQ(written, read);
}

TEST(HexState_SingleIO, Nonempty) {
    HexState written, read;
    std::stringstream ss;
    ASSERT_EQ(GameSolve::hex_rand_moves(written, 20), 0);

    {
        cereal::BinaryOutputArchive ocer(ss);
        ocer(written);
    }
    {
        cereal::BinaryInputArchive icer(ss);
        icer(read);
    }

    EXPECT_EQ(written, read);
}

// TEST(HexState_SingleIO, ReadCutOff) {
//     HexState state;

//     std::ofstream ofile(HEX_AI_TESTING_DIR"HexState_SingleIO__ReadCutOff");
//     for (int t = 0; t < 10; t++) {
//         ofile.put(HexState::PLAYERS::PLAYER_ONE);
//     }

//     std::ifstream ifile(HEX_AI_TESTING_DIR"HexState_SingleIO__ReadCutOff");
//     EXPECT_EQ(state.deserialize(ifile), 1);

//     std::remove(HEX_AI_TESTING_DIR"HexState_SingleIO__ReadCutOff");
// }

// TEST(HexState_SingleIO, ReadBadValue) {
//     HexState state;

//     std::ofstream ofile(HEX_AI_TESTING_DIR"HexState_SingleIO__ReadBadValue");
//     for (int t = 0; t < 1000; t++) {
//         ofile.put(0xff);
//     }
//     ofile.close();

//     std::ifstream ifile(HEX_AI_TESTING_DIR"HexState_SingleIO__ReadBadValue");
//     EXPECT_EQ(state.deserialize(ifile), 2);

//     std::remove(HEX_AI_TESTING_DIR"HexState_SingleIO__ReadBadValue");
// }

// TEST(HexState_SingleIO, ToBadStream) {
//     HexState state;

//     std::ofstream ofile("bad/file/name");
//     EXPECT_EQ(state.serialize(ofile), 1);
// }

// TEST(HexState_SingleIO, FromBadStream) {
//     HexState state;

//     std::ifstream ifile("bad/file/name");
//     EXPECT_EQ(state.deserialize(ifile), 3);
// }

/*************************************************
* Bulk HexState Reads                            *
*************************************************/

// TEST(HexState_MultiIO, NoWrites) {
//     std::vector<HexState> states;
//     ASSERT_EQ(GameState::write_hexstates("HexState_MultiIO__NoWrites", states), 0);
//     ASSERT_EQ(GameState::read_hexstates("HexState_MultiIO__NoWrites", states), 0);
//     EXPECT_EQ(states.size(), 0);
// }

// TEST(HexState_MultiIO, Empty) {
//     std::vector<HexState> states_write, states_read;
//     for (int x = 0; x < 32; x++) {
//         states_write.push_back(HexState());
//     }
//     ASSERT_EQ(GameState::write_hexstates("HexState_MultiIO__Empty", states_write), 0);
//     ASSERT_EQ(GameState::read_hexstates("HexState_MultiIO__Empty", states_read), 0);
//     ASSERT_EQ(states_read.size(), 32);
//     for (int x = 0; x < 32; x++) {
//         EXPECT_EQ(states_read.at(x) == states_write.at(x), true);
//     }
// }

// TEST(HexState_MultiIO, Nonempty) {
//     std::vector<HexState> states_write, states_read;
//     HexState state;
//     for (int x = 0; x < 32; x++) {
//         state = HexState();
//         GameSolve::hex_rand_moves(state, 10);
//         states_write.push_back(state);
//     }
//     ASSERT_EQ(GameState::write_hexstates("HexState_MultiIO__Nonempty", states_write), 0);
//     ASSERT_EQ(GameState::read_hexstates("HexState_MultiIO__Nonempty", states_read), 0);
//     ASSERT_EQ(states_read.size(), 32);
//     for (int x = 0; x < 32; x++) {
//         EXPECT_EQ(states_read.at(x) == states_write.at(x), true);
//     }
// }

// TEST(HexState_MultiIO, ReadCutOff) {
//     std::vector<HexState> states_write, states_read;
//     HexState state;
//     for (int x = 0; x < 16; x++) {
//         state = HexState();
//         GameSolve::hex_rand_moves(state, 10);
//         states_write.push_back(state);
//     }
//     ASSERT_EQ(Util::write_hexstates("HexState_MultiIO__Nonempty", states_write), 0);
//     ASSERT_EQ(Util::read_hexstates("HexState_MultiIO__Nonempty", states_read), 0);
//     ASSERT_EQ(states_read.size(), 32);
//     for (int x = 0; x < 32; x++) {
//         EXPECT_EQ(states_read.at(x) == states_write.at(x), true);
//     }
// }

// TEST(HexState_MultiIO, ReadBadValue) {

// }

// TEST(HexState_MultiIO, ToBadStream) {

// }

// TEST(HexState_MultiIO, FromBadStream) {

// }

