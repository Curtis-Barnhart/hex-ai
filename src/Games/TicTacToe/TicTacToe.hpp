#ifndef GAMES_TIC_TAC_TOE_TIC_TAC_TOE_HPP
#define GAMES_TIC_TAC_TOE_TIC_TAC_TOE_HPP

#include "../../GameState/GameState.hpp"

namespace GameState {
    /**
    * TTT is an enum for the state that each square in a tic-tac-toe game
    * may be in: empty, occupied by an x, and occupied by an o.
    */
    enum TTT {
        EMPTY, X, O
    };

    /**
     * TTT_Action represents an action taken in a game of tic-tac-toe.
     * It stores an x and y coordinate in the domain [0, 2],
     * as well a TTT enum for whose move it was.
     */
    struct TTT_Action {
        /**
         * @param whose which symbol (X or O) to place
         * @param x x coordinate to place a symbol at
         * @param y y coordinate to place a symbol at
         */
        TTT_Action(TTT whose, char x, char y) : whose(whose), x(x), y(y) {}
        TTT_Action(TTT_Action &&) = default;
        TTT_Action(const TTT_Action &) = default;
        TTT_Action &operator=(TTT_Action &&) = default;
        TTT_Action &operator=(const TTT_Action &) = default;
        ~TTT_Action() = default;
        
        TTT whose = EMPTY;
        char x = 0, y = 0;
    };

    template<>
    class State<TTT_Action> {
    public:
        /**
         * The default constructor for tic tac toe makes an empty board
         * with the current turn belonging to the first player (player 0).
         */
        State<TTT_Action>() = default;
        State<TTT_Action>(State<TTT_Action> &&) = default;
        State<TTT_Action>(const State<TTT_Action> &) = default;
        State<TTT_Action> &operator=(State<TTT_Action> &&) = default;
        State<TTT_Action> &operator=(const State<TTT_Action> &) = default;
        ~State<TTT_Action>() = default;

        // methods to determine winning
        /**
         * Returns the winner of the game.
         * @return 0 if first player wins, 1 if second player wins, -1 if neither.
         */
        int who_won() const {
            // iterate over rows and columns at the same time
            for (size_t i = 0; i < 3; i++) {
                if (this->board[i][0] != EMPTY
                    && this->board[i][0] == this->board[i][1]
                    && this->board[i][1] == this->board[i][2]) {
                    return this->board[i][0] == X ? 0 : 1;
                }
                if (this->board[0][i] != EMPTY
                    && this->board[0][i] == this->board[1][i]
                    && this->board[1][i] == this->board[2][i]) {
                    return this->board[i][0] == X ? 0 : 1;
                }
            }
            // check both diagonals
            TTT mid = this->board[1][1];
            if (mid != EMPTY) {
                if ((this->board[0][0] == mid && mid == this->board[2][2])
                    || (this->board[0][2] == mid && mid == this->board[2][0])) {
                    return mid == X ? 0 : 1;
                }
            }
            return -1;
        }

        // methods for move making
        /**
         * Returns whose turn it is.
         * @return 0 if first player's turn, 1 if second.
         */
        int whose_turn() const {
            return this->turn;
        }

        /**
         * Implements GameState's suceed method.
         * NOTE: no behavior is guaranteed for illegal actions.
         * TODO: if you can figure out how to link with Doxygen do that
         * @param action the action to play at the current state.
         * @return a pointer to the state created by playing the action.
         */
        State<TTT_Action> *succeed(const TTT_Action &action) const {
            State<TTT_Action> *next_state = new State<TTT_Action>(*this);
            next_state->board[action.x][action.y] = action.whose;
            return next_state;
        }

        /**
         * Implements GameState's get_actions method.
         * @return a pointer to a vector of legal actions from current state.
         */
        std::vector<TTT_Action> *get_actions() const {
            std::vector<TTT_Action> *a = new std::vector<TTT_Action>();
            this->get_actions(*a);    
            return a;
        }
        
        /**
         * Implements GameState's get_actions method.
         * @param buffer a vector to place the new actions into (optionally empty).
         */
        void get_actions(std::vector<TTT_Action> &buffer) const {
            TTT whose_turn = this->turn == 0 ? X : O;
            buffer.reserve(9);
            for (std::size_t x = 0; x < 3; x++) {
                for (std::size_t y = 0; y < 3; y++) {
                    if (this->board[x][y] == EMPTY) {
                        buffer.emplace_back(whose_turn , x, y);
                    }
                }
            }
        }

    private:
        int turn = 0;
        TTT board[3][3] = { EMPTY };
    };

    using TTT_State = GameState::State<TTT_Action>;
}

#endif // !GAMES_TIC_TAC_TOE_TIC_TAC_TOE_HPP

