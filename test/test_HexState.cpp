#include <gtest/gtest.h>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameSolve/DeepNodeSolve.hpp"

using GameState::HexState;
using GameState::HexState::PLAYERS::PLAYER_ONE;
using GameState::HexState::PLAYERS::PLAYER_TWO;
using GameState::HexState::PLAYERS::PLAYER_NONE;

/*************************************************
* Definitions                                    *
*************************************************/

TEST(HexState_Definitions)

/*************************************************
* Trivials                                       *
*************************************************/

TEST(HexState_Trivials, EmptyConstructor) {
    HexState state, empty;

    // make a state which has no moves on it
    for (unsigned char x = 0; x < 11; x++) {
        for (unsigned char y = 0; y < 11; y++) {
            empty.succeed_in_place({x, y, PLAYER_NONE});
        }
    }

    EXPECT_EQ(state.board_state_equal(empty), true)
        << "A newly constructed board should be empty";
    EXPECT_EQ(state.whose_turn(), 0)
        << "A newly constructed board should be at turn 0";
}

TEST(HexState_Trivials, EmptyWin) {
    HexState empty;

    EXPECT_EQ(empty.who_won(), PLAYER_NONE)
        << "A newly constructed board should be won by no one";
}

TEST(HexState_Trivials, EmptyBoardMoves) {
    HexState empty;

    EXPECT_EQ(empty.get_actions()->size(), 121)
        << "A newly constructed board should have 121 available moves";
}

/*************************************************
* Movement                                       *
*************************************************/

TEST(HexState_Movement, TurnCount) {
    HexState state;

    GameSolve::hex_rand_moves(state, 5);
    ASSERT_EQ(state.who_won(), PLAYER_NONE)
        << "No one can win in 5 turns";
    EXPECT_EQ(state.whose_turn(), 1);
}

/*************************************************
* WinCondition                                   *
*************************************************/

/*************************************************
* ConstructionAssignment                         *
*************************************************/

/*************************************************
* Equality                                       *
*************************************************/

