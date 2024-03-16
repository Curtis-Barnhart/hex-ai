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
        /**
         * Constructor sets a move that is not possible (made by player -1)
         */
        TA();

        /**
        * Constructor sets memeber values according to the given parameters.
        * @param x the x coordinate of what tile should be claimed
        * @param y the y coordinate of what tile should be claimed
        * @param whose 0 for the first player, 1 for the second, -1 for no player
        */
        TA(signed char x, signed char y, signed char whose);

        signed char x = 0, y = 0, whose = 0;
    };
}

#endif // !GAMES_TIC_TAC_TOE_TTT_ACTIONS_HPP

