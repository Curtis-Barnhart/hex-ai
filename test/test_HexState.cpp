#include <gtest/gtest.h>
#include <utility>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameSolve/HexUtil.hpp"

using GameState::HexState;
using GameState::HexState::PLAYERS::PLAYER_ONE;
using GameState::HexState::PLAYERS::PLAYER_TWO;
using GameState::HexState::PLAYERS::PLAYER_NONE;

/*************************************************
* Definitions                                    *
*************************************************/

TEST(HexState_Definitions, PLAYERS_Value) {
    EXPECT_EQ(PLAYER_ONE, 0);
    EXPECT_EQ(PLAYER_TWO, 1);
    EXPECT_EQ(PLAYER_NONE, 2);
}

TEST(HexState_Definitions, BOARD_SIZE_Value) {
    EXPECT_EQ(BOARD_SIZE, 11);
}

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
    
    auto actions = empty.get_actions();

    EXPECT_EQ(actions->size(), 121)
        << "A newly constructed board should have 121 available moves";

    delete actions;
}

/*************************************************
* Movement                                       *
*************************************************/

TEST(HexState_Movement, TurnCount) {
    HexState state;

    GameSolve::hex_rand_moves(state, 5);
    ASSERT_EQ(state.who_won(), PLAYER_NONE)
        << "No one can win in 5 turns";
    EXPECT_EQ(state.whose_turn(), PLAYER_TWO)
        << "It should be player 2's turn after 5 moves";
    EXPECT_EQ(state.get_actions()->size(), 116)
        << "There should be 116 moves after 5 have been taken";

    GameSolve::hex_rand_moves(state, 5);
    ASSERT_EQ(state.who_won(), PLAYER_NONE)
        << "No one can win in 10 turns";
    EXPECT_EQ(state.whose_turn(), PLAYER_ONE)
        << "It should be player 1's turn after 10 moves";
    EXPECT_EQ(state.get_actions()->size(), 111)
        << "There should be 111 moves after 10 have been taken";

    GameSolve::hex_rand_moves(state, 5);
    ASSERT_EQ(state.who_won(), PLAYER_NONE)
        << "No one can win in 15 turns";
    EXPECT_EQ(state.whose_turn(), PLAYER_TWO)
        << "It should be player 2's turn after 15 moves";
    EXPECT_EQ(state.get_actions()->size(), 106)
        << "There should be 106 moves after 15 have been taken";
}

TEST(HexState_Movement, WinMoveCount) {
    HexState state;

    for (unsigned char y = 0; y < 11; y++) {
        state.succeed_in_place({0, y, PLAYER_ONE});
    }

    EXPECT_EQ(state.get_actions()->size(), 0)
        << "There should be 0 available moves after a game has been won";
}

/*************************************************
* WinCondition                                   *
*************************************************/

TEST(HexState_WinCondition, PlayerOneWin) {
    HexState state;

    for (unsigned char y = 0; y < 11; y++) {
        state.succeed_in_place({0, y, PLAYER_ONE});
    }

    EXPECT_EQ(state.who_won(), PLAYER_ONE)
        << "Player 1 should have won this game";
}

TEST(HexState_WinCondition, PlayerOneWinFalse) {
    HexState state;

    for (unsigned char x = 0; x < 11; x++) {
        state.succeed_in_place({x, 0, PLAYER_NONE});
    }

    EXPECT_EQ(state.who_won(), PLAYER_NONE)
        << "No one should win as Player 1 has gone the wrong direction";
}

TEST(HexState_WinCondition, PlayerTwoWin) {
    HexState state;

    for (unsigned char x = 0; x < 11; x++) {
        state.succeed_in_place({x, 0, PLAYER_TWO});
    }

    EXPECT_EQ(state.who_won(), PLAYER_TWO)
        << "Player 2 should have won";
}

TEST(HexState_WinCondition, PlayerTwoWinFalse) {
    HexState state;

    for (unsigned char y = 0; y < 11; y++) {
        state.succeed_in_place({0, y, PLAYER_TWO});
    }

    EXPECT_EQ(state.who_won(), PLAYER_NONE)
        << "No one should win as Player 2 has gone the wrong direction";
}

/*************************************************
* ConstructionAssignment                         *
*************************************************/

TEST(HexState_CtorAndAsnmt, CopyMoveCtor) {
    HexState state;
    GameSolve::hex_rand_moves(state, 10);

    HexState s1(state);
    ASSERT_EQ(s1, state)
        << "s1 was copy constructed from state and should be equal";
    HexState s2(std::move(state));
    EXPECT_EQ(s2, s1)
        << "s2 was move constructed from state and should be equal to s1";
}

TEST(HexState_CtorAndAsnmt, CopyMoveAsnmt) {
    HexState state, s1, s2;
    GameSolve::hex_rand_moves(state, 10);

    s1 = state;
    ASSERT_EQ(s1, state)
        << "s1 was copy assigned from state and should be equal";
    s2 = std::move(state);
    EXPECT_EQ(s2, s1)
        << "s2 was move assigned from state and should be equal to s1";
}

/*************************************************
* Equality                                       *
*************************************************/

TEST(HexState_Equality, EqualStates) {
    HexState s1, s2;
    s1.succeed_in_place({0, 0, PLAYER_ONE});
    s2.succeed_in_place({0, 0, PLAYER_ONE});
    s1.succeed_in_place({1, 1, PLAYER_TWO});
    s2.succeed_in_place({1, 1, PLAYER_TWO});
    s1.succeed_in_place({2, 2, PLAYER_ONE});
    s2.succeed_in_place({2, 2, PLAYER_ONE});

    EXPECT_EQ(s1, s2)
        << "s1 and s2 have the same moves applied and should be equal";
}

TEST(HexState_Equality, InequalStates) {
    HexState s1, s2;
    s1.succeed_in_place({0, 0, PLAYER_ONE});
    s1.succeed_in_place({1, 1, PLAYER_TWO});
    s1.succeed_in_place({2, 2, PLAYER_ONE});

    EXPECT_NE(s1, s2)
        << "s1 and s2 have different moves applied and should be inequal";
}

/*************************************************
* Other                                          *
*************************************************/

TEST(HexState_Miscellaneous, FlipEmpty) {
    HexState s1, s2;
    EXPECT_EQ(s1.flip(GameState::HexState::HORIZONTAL), s2);
    EXPECT_EQ(s1.flip(GameState::HexState::VERTICAL), s2);
    EXPECT_EQ(s1.flip(GameState::HexState::BOTH), s2);
}

TEST(HexState_Miscellaneous, FlipNonempty) {
    HexState original, s1, s2, s3;

    original.succeed_in_place({1, 1, PLAYER_ONE});
    original.succeed_in_place({5, 1, PLAYER_TWO});
    HexState o2(original), o3(original);
    
    s1.succeed_in_place({9, 1, PLAYER_ONE});
    s1.succeed_in_place({5, 1, PLAYER_TWO});

    s2.succeed_in_place({1, 9, PLAYER_ONE});
    s2.succeed_in_place({5, 9, PLAYER_TWO});
    
    s3.succeed_in_place({9, 9, PLAYER_ONE});
    s3.succeed_in_place({5, 9, PLAYER_TWO});

    EXPECT_EQ(original.flip(GameState::HexState::HORIZONTAL), s1);
    EXPECT_EQ(o2.flip(GameState::HexState::VERTICAL), s2);
    EXPECT_EQ(o3.flip(GameState::HexState::BOTH), s3);
}

