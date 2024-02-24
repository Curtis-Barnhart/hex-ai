#ifndef GAMES_TIC_TAC_TOE_TTT_ACTIONS_HPP
#define GAMES_TIC_TAC_TOE_TTT_ACTIONS_HPP

namespace GameState {
    /**
    * TTT_Action holds a record of a player's turn in a game of tic-tac-toe.
    * The coordinate of the tile they mark that turn is held by `x` and `y`.
    * The player whose turn it was is held by `whose`.
    * Because tic-tac-toe is "monotonic" (pieces are only ever added to the
    * board), a previous state may be determined given a current state and
    * the action that took us from the previous state to the current one.
    * Thus, TTT_Action suffices for both the forwards and the backwards action
    * for tic-tac-toe.
    */
    struct TA {
        signed char x = 0, y = 0, whose = 0;
    };
}

#endif // !GAMES_TIC_TAC_TOE_TTT_ACTIONS_HPP

