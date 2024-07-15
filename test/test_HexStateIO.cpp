#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameSolve/HexUtil.hpp"

using GameState::HexState;

#ifndef HEX_AI_TESTING_DIR
    #define HEX_AI_TESTING_DIR "./data"
#endif // !HEX_AI_TESTING_DIR

/*************************************************
* HexState Instance Reads                        *
*************************************************/

TEST(HexState_FromFile, Empty) {
    HexState state, from_file;

    std::ofstream ofile(HEX_AI_TESTING_DIR"HexState_FromFile__Empty");
    ASSERT_EQ(state.pack_to_stream(ofile), 0);
    ofile.close();

    std::ifstream ifile(HEX_AI_TESTING_DIR"HexState_FromFile__Empty");
    ASSERT_EQ(from_file.unpack_from_stream(ifile), 0);

    EXPECT_EQ(state, from_file);

    std::remove(HEX_AI_TESTING_DIR"HexState_FromFile__Empty");
}

TEST(HexState_FromFile, Nonempty) {
    HexState state, from_file;

    ASSERT_EQ(GameSolve::hex_rand_moves(state, 20), 0);

    std::ofstream ofile(HEX_AI_TESTING_DIR"HexState_FromFile__Nonempty");
    ASSERT_EQ(state.pack_to_stream(ofile), 0);
    ofile.close();

    std::ifstream ifile(HEX_AI_TESTING_DIR"HexState_FromFile__Nonempty");
    ASSERT_EQ(from_file.unpack_from_stream(ifile), 0);

    EXPECT_EQ(state, from_file);

    std::remove(HEX_AI_TESTING_DIR"HexState_FromFile__Nonempty");
}

TEST(HexState_FromFile, ReadCutOff) {
    HexState state;

    std::ofstream ofile(HEX_AI_TESTING_DIR"HexState_FromFile__ReadCutOff");
    for (int t = 0; t < 10; t++) {
        ofile.put(HexState::PLAYERS::PLAYER_ONE);
    }

    std::ifstream ifile(HEX_AI_TESTING_DIR"HexState_FromFile__ReadCutOff");
    EXPECT_EQ(state.unpack_from_stream(ifile), 1);

    std::remove(HEX_AI_TESTING_DIR"HexState_FromFile__ReadCutOff");
}

TEST(HexState_FromFile, ReadBadValue) {
    HexState state;

    std::ofstream ofile(HEX_AI_TESTING_DIR"HexState_FromFile__ReadBadValue");
    for (int t = 0; t < 1000; t++) {
        ofile.put(0xff);
    }
    ofile.close();

    std::ifstream ifile(HEX_AI_TESTING_DIR"HexState_FromFile__ReadBadValue");
    EXPECT_EQ(state.unpack_from_stream(ifile), 2);

    std::remove(HEX_AI_TESTING_DIR"HexState_FromFile__ReadBadValue");
}

TEST(HexState_FromFile, ToBadStream) {
    HexState state;

    std::ofstream ofile("bad/file/name");
    EXPECT_EQ(state.pack_to_stream(ofile), 1);
}

TEST(HexState_FromFile, FromBadStream) {
    HexState state;

    std::ifstream ifile("bad/file/name");
    EXPECT_EQ(state.unpack_from_stream(ifile), 3);
}

/*************************************************
* Bulk HexState Reads                            *
*************************************************/

