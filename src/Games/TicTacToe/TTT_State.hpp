#ifndef GAMES_TIC_TAC_TOE_TTT_STATE_HPP
#define GAMES_TIC_TAC_TOE_TTT_STATE_HPP

#include "TTT_StateBase.hpp"

namespace GameState {
    /**
     * TTT_State is a basic implementation of a tic-tac-toe game state.
     * It stores 
     */
    class TTT_State: public GameState::State<TTT_Action> {
    public:
        /**
         * The default constructor for tic tac toe makes an empty board
         * with the current turn belonging to the first player (player 0).
         */
        TTT_State() = default;
        TTT_State(TTT_State &&) = default;
        TTT_State(const TTT_State &) = default;

        ~TTT_State() override = default;

        // methods to determine winning
        /**
         * Returns whose turn it is.
         * @return 0 if first player's turn, 1 if second.
         */
        int whose_turn() const override;

        // methods for move making
        /**
         * Returns the winner of the game.
         * @return 0 if first player wins, 1 if second player wins, -1 if neither.
         */
        int who_won() const override;

        /**
         * Implements GameState's suceed non-mutating method.
         * NOTE: no behavior is guaranteed for illegal actions.
         * TODO: if you can figure out how to link with Doxygen do that
         * @param action the action to play at the current state.
         * @return a pointer to the state created by playing the action.
         */
        TTT_State *succeed(const TTT_Action &action) const override;

        /**
         * Implements GameState's suceed mutating method.
         * NOTE: no behavior is guaranteed for illegal actions.
         * TODO: if you can figure out how to link with Doxygen do that
         * @param action the action to play at the current state.
         * @return a refence to the state it was called on after mutating it.
         */
        TTT_State &succeed_in_place(const TTT_Action &action) override;

        /**
         * Implements GameState's get_actions method.
         * @return a pointer to a vector of legal actions from current state.
         */
        std::vector<TTT_Action> *get_actions() const override;
        
        /**
         * Implements GameState's get_actions method.
         * @param buffer a vector to place the new actions into (optionally empty).
         */
        void get_actions(std::vector<TTT_Action> &buffer) const override;

    private:
        int turn = 0;
        TTT board[3][3] = { EMPTY };

        /**
         * reset is used to prepare an object for being = to from another.
         * In this case we do nothing. Curtis know how this works.
         * TODO: he should document it later
         */
        void reset() override {}
    };
}

#endif // !GAMES_TIC_TAC_TOE_TIC_TAC_TOE_HPP

