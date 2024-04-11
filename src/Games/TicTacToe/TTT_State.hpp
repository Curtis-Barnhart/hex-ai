#ifndef GAMES_TICTACTOE_TTT_STATE_HPP
#define GAMES_TICTACTOE_TTT_STATE_HPP

#include <cstddef>
#include <iostream>
#include <cstring>
#include <vector>

#define PLAYER_X 0
#define PLAYER_O 1
#define PLAYER_NONE -1

namespace GameState {

    /**
     * TTT_State is a basic implementation of a tic-tac-toe game state.
     * For details on its interface, read GameState::State.
     */
    class TTT_State {
        /*
         * prints out the board like you'd expect
         */
        friend std::ostream &operator<<(std::ostream &out, const TTT_State &game) {
            char c;
            for (int y = 2; y >= 0; y--) {
                for (int x = 0; x < 3; x++) {
                    switch (game.board[x][y]) {
                        case PLAYER_NONE:
                            c = ' ';
                            break;
                        case PLAYER_X:
                            c = 'x';
                            break;
                        case PLAYER_O:
                            c = 'o';
                            break;
                    }
                    out << c;
                }
                out << '\n';
            }

            return out;
        }

        friend class std::hash<TTT_State>;

    public:
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
        struct Action {
            signed char x = 0, y = 0, whose = PLAYER_NONE;

            /**
             * Constructor sets a move that is not possible (made by player -1)
             */
            Action() = default;

            /**
            * Constructor sets memeber values according to the given parameters.
            * @param x the x coordinate of what tile should be claimed
            * @param y the y coordinate of what tile should be claimed
            * @param whose 0 for the first player, 1 for the second, -1 for no player
            */
            Action(signed char x, signed char y, signed char whose): x(x), y(y), whose(whose) {}
        };
        
        /*
        * Constructor doesn't need to do anything because private members of object
        * have default values that indicate an empty board.
        */
        TTT_State() = default;

        /*
        * Copies turn and board array.
        */
        TTT_State(TTT_State &&other) {
            this->turn = other.turn;
            std::memcpy(this->board, other.board, sizeof(this->board));
        }

        /*
        * Copies turn and board array.
        */
        TTT_State(const TTT_State &other) {
            this->turn = other.turn;
            std::memcpy(this->board, other.board, sizeof(this->board));
        }

        /*
        * Copies turn and board array.
        */
        TTT_State &operator=(const TTT_State &other) {
            this->turn = other.turn;
            std::memcpy(this->board, other.board, sizeof(this->board));
            return *this;
        }

        /*
        * Copies turn and board array.
        */
        TTT_State &operator=(TTT_State &&other) {
            this->turn = other.turn;
            std::memcpy(this->board, other.board, sizeof(this->board));
            return *this;
        }

        /*
        * No memory is dynamically allocated in constructor, so constructor does
        * nothing.
        */
        ~TTT_State() = default;
        
        /*
         * Gotta test for equality somehow;
         */
        bool operator==(const TTT_State &other) const {
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    if (this->board[x][y] != other.board[x][y]) {
                        return false;
                    }
                }
            }
            return this->turn == other.turn;
        }

        /*
        * Simply returns the turn number.
        */
        int whose_turn() const {
            return this->turn;
        }

        /*
        * If a player owns 3 tiles in a row either horizontally, vertically, or
        * diagonally, they win.
        */
        int who_won() const {
            signed char mid;
            // iterate over rows and columns at the same time
            // if any row or column contains all the same value and that value is not
            // PLAYER_NONE, then whoever's value it is won.
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
            // check both diagonals (same requirement for winning as rows and columns)
            mid = this->board[1][1];
            if (mid != PLAYER_NONE) {
                if ((this->board[0][0] == mid && mid == this->board[2][2])
                    || (this->board[0][2] == mid && mid == this->board[2][0])) {
                    return mid;
                }
            }
            return PLAYER_NONE;
        }

        /*
         * If a player wins, their score is 1.
         */
        void current_score(double score[2]) const {
            switch (this->who_won()) {
                case PLAYER_NONE:
                    score[0] = 0.5l;
                    score[1] = 0.5l;
                    return;
                case PLAYER_X:
                    score[0] = 1l;
                    score[1] = 0l;
                    return;
                case PLAYER_O:
                    score[0] = 0l;
                    score[1] = 1l;
                    return;
            }
        }

        /*
        * If player P has an action at (x, y), then that tile in the board array should
        * hold their value.
        */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        TTT_State *succeed(const TTT_State::Action &action) const {
            auto next = new TTT_State(*this);
            next->board[action.x][action.y] = action.whose;
            next->turn = (action.whose + 1) % 2;
            return next;
        }

        /*
        * If player P has an action at (x, y), then that tile in the board array should
        * hold their value.
        * To tell how to undoe an action, you just need to know where a player claimed
        * a tile.
        */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        TTT_State *succeed(const TTT_State::Action &action, TTT_State::Action &baction) const {
            auto next = new TTT_State(*this);
            next->board[action.x][action.y] = action.whose;
            next->turn = (action.whose + 1) % 2;

            baction = action;
            return next;
        }

        /*
        * If player P has an action at (x, y), then that tile in the board array should
        * hold their value.
        */
        TTT_State &succeed_in_place(const TTT_State::Action &action) {
            this->board[action.x][action.y] = action.whose;
            this->turn = (action.whose + 1) % 2;
            return *this;
        }

        /*
        * If player P has an action at (x, y), then that tile in the board array should
        * hold their value.
        * To tell how to undoe an action, you just need to know where a player claimed
        * a tile.
        */
        TTT_State &succeed_in_place(const TTT_State::Action &action, TTT_State::Action &baction) {
            this->board[action.x][action.y] = action.whose;
            this->turn = (action.whose + 1) % 2;

            baction = action;
            return *this;
        }

        /*
        * To undoe an action, remove a player's value from the tile they claimed in
        * their action.
        * Then set the current turn as belonging to that player.
        */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        TTT_State *reverse(const TTT_State::Action &baction) const {
            auto previous = new TTT_State(*this);
            previous->board[baction.x][baction.y] = PLAYER_NONE;
            previous->turn = baction.whose;
            return previous;
        }

        /*
        * To undoe an action, remove a player's value from the tile they claimed in
        * their action.
        * Then set the current turn as belonging to that player.
        */
        TTT_State &reverse_in_place(const TTT_State::Action &baction) {
            this->board[baction.x][baction.y] = PLAYER_NONE;
            this->turn = baction.whose;
            return *this;
        }

        /*
        * Slight wrapper around get_actions(std::vector<TA> &).
        */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        std::vector<TTT_State::Action> *get_actions() const {
            std::vector<TTT_State::Action> *a = new std::vector<TTT_State::Action>();
            this->get_actions(*a);
            return a;
        }

        /*
        * For every tile on the board, if that tile is empty, that's a valid location
        * a player could claim as their next move.
        */
        void get_actions(std::vector<TTT_State::Action> &buffer) const {
            // If someone has already won, there are no actions
            if (this->who_won() != PLAYER_NONE) {
                return;
            }

            signed char whose_turn = this->turn;
            buffer.reserve(9);
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    if (this->board[x][y] == PLAYER_NONE) {
                        buffer.emplace_back(x, y, whose_turn);
                    }
                }
            }
        }

    private:
        signed char turn = 0;
        signed char board[3][3] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    };
}

/*
*
*/
template<>
struct std::hash<GameState::TTT_State> {
    size_t operator()(const GameState::TTT_State &state) {
        size_t result = 0;
        int pow_three = 1;
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                result += (state.board[x][y] + 1) * pow_three;
                pow_three *= 3;
            }
        }
        result += (state.whose_turn() * pow_three);

        return result;
    }
};

#undef PLAYER_X
#undef PLAYER_O
#undef PLAYER_NONE

#endif // !GAMES_TICTACTOE_TTT_STATE_HPP

