#ifndef GAMES_TIC_TAC_TOE_TTT_STATE_HPP
#define GAMES_TIC_TAC_TOE_TTT_STATE_HPP

#include "../../GameState/GameState.hpp"
#include "TTT_Actions.hpp"
#include <cstring>

#define PLAYER_X 0
#define PLAYER_Y 1
#define PLAYER_NONE -1

using GameState::TA;

namespace GameState {

/**
 * TTT_State is a basic implementation of a tic-tac-toe game state.
 * It stores 
 */
template<>
class State<TA, TA> {
public:
    /**
     * The default constructor for tic tac toe makes an empty board
     * with the current turn belonging to the first player (player 0).
     */
    State() = default;

    /**
     * Copies board and turn from other.
     * @param other the tic-tac-toe state you want to copy
     */
    State(State<TA, TA> &&other) {
        this->turn = other.turn;
        std::memcpy(this->board, other.board, sizeof(this->board));
    }

    /**
     * Copies board and turn from other.
     * @param other the tic-tac-toe state you want to copy
     */
    State(const State &other) {
        this->turn = other.turn;
        std::memcpy(this->board, other.board, sizeof(this->board));
    }

    /**
     * Copies board and turn from other.
     * @param other the tic-tac-toe state you want to copy
     * @return the original State object that has just been set equal to other
     */
    GameState::State<TA, TA> &operator=(const GameState::State<TA, TA> &other) {
        this->turn = other.turn;
        std::memcpy(this->board, other.board, sizeof(this->board));
        return *this;
    }

    /**
     * Copies board and turn from other.
     * @param other the tic-tac-toe state you want to copy
     * @return the original State object that has just been set equal to other
     */
    GameState::State<TA, TA> &operator=(GameState::State<TA, TA> &&other) {
        this->turn = other.turn;
        std::memcpy(this->board, other.board, sizeof(this->board));
        return *this;
    }

    /**
     * The destructor does nothing.
     */
    ~State<TA, TA>() = default;

    /**
     * Returns whose turn it is.
     * @return 0 if first player's turn, 1 if second.
     */
    int whose_turn() const {
        return this->turn;
    }

    /**
     * Returns the winner of the game.
     * @return 0 if first player wins, 1 if second player wins, -1 if neither.
     */
    int who_won() const {
        signed char mid;
        // iterate over rows and columns at the same time
        for (size_t i = 0; i < 3; i++) {
            if ((mid = this->board[i][1]) != PLAYER_NONE
                && mid == this->board[i][0]
                && mid == this->board[i][2]) {
                return mid;
            }
            if ((mid = this->board[1][i]) != PLAYER_NONE
                && mid == this->board[0][i]
                && mid == this->board[2][i]) {
                return mid;
            }
        }
        // check both diagonals
        mid = this->board[1][1];
        if (mid != PLAYER_NONE) {
            if ((this->board[0][0] == mid && mid == this->board[2][2])
                || (this->board[0][2] == mid && mid == this->board[2][0])) {
                return mid;
            }
        }
        return -1;
    }

    /**
     * Given an action in a game of tic-tac-toe, return a pointer to a new
     * state that represents the state of the game after that action has
     * been played.
     * NOTE: no behavior is guaranteed for illegal actions.
     * TODO: if you can figure out how to link with Doxygen do that
     * @param action the action to play at the current state.
     * @return a pointer to the state created by playing the action.
     */
    GameState::State<TA, TA> *succeed(const TA &action) const {
        GameState::State<TA, TA> *next = new GameState::State<TA, TA>(*this);
        next->board[action.x][action.y] = action.whose;
        next->turn = (action.whose + 1) % 2;
        return next;
    }

    /**
     * Given an action in a game of tic-tac-toe, mutate state so that the new
     * state will represent the state of the game after that action has
     * been played.
     * NOTE: no behavior is guaranteed for illegal actions.
     * TODO: if you can figure out how to link with Doxygen do that
     * @param action the action to play at the current state.
     * @return a refence to the state it was called on after mutating it.
     */
    GameState::State<TA, TA> &succeed_in_place(const TA &action) {
        this->board[action.x][action.y] = action.whose;
        this->turn = (action.whose + 1) % 2;
        return *this;
    }

    /**
     * Implements GameState's get_actions method.
     * @return a pointer to a vector of legal actions from current state.
     */
    std::vector<TA> *get_actions() const {
        std::vector<TA> *a = new std::vector<TA>();
        this->get_actions(*a);    
        return a;
    }
    
    /**
     * Implements GameState's get_actions method.
     * @param buffer a vector to place the new actions into (optionally empty).
     */
    void get_actions(std::vector<TA> &buffer) const {
        signed char whose_turn = this->turn;
        buffer.reserve(9);
        for (char x = 0; x < 3; x++) {
            for (char y = 0; y < 3; y++) {
                if (this->board[x][y] == PLAYER_NONE) {
                    buffer.emplace_back(whose_turn , x, y);
                }
            }
        }
    }

private:
    signed char turn = PLAYER_NONE;
    signed char board[3][3] = { PLAYER_NONE };
};

}

#undef PLAYER_X
#undef PLAYER_Y
#undef PLAYER_NONE

#endif // !GAMES_TIC_TAC_TOE_TIC_TAC_TOE_HPP

