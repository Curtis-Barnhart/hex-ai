#ifndef GAMES_HEX_HEXSTATE_HPP
#define GAMES_HEX_HEXSTATE_HPP

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <vector>

#include "../../GameState/GameState.hpp"

#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define PLAYER_NONE -1

namespace GameState {
/**
 * 
 */
template<unsigned char S>
struct HA {
    signed char x = 0, y = 0, whose = PLAYER_NONE;

    /**
     * TODO: this is unneeded isn't it
     * Constructor sets a move that is not possible (made by player -1)
     */
    HA() = default;

    /**
    * Constructor sets memeber values according to the given parameters.
    * @param x the x coordinate of what tile should be claimed
    * @param y the y coordinate of what tile should be claimed
    * @param whose 0 for the first player, 1 for the second, -1 for no player
     */
    HA(signed char x, signed char y, signed char whose): x(x), y(y), whose(whose) {}
};

/**
 * HexState represents a state of a game of Hex.
 */
template<unsigned char S>
class State<HA<S>, HA<S>, 2> {
    friend std::ostream &operator<<<HA<S>, HA<S>, 2>(std::ostream &out, const State<HA<S>, HA<S>, 2> &state) {
        for (int y = S - 1; y >= 0; y--) {
            for (int x = 0; x < S; x++) {
                switch (*state->board[x][y]) {
                    case PLAYER_NONE:
                        out << ' ';
                        break;
                    case PLAYER_ONE:
                        out << '1';
                        break;
                    case PLAYER_TWO:
                        out << '2';
                        break;
                }
            }
            out << "\n";
        }
        out << std::endl;
        return out;
    }

    friend class std::hash<GameState::State<HA<S>, HA<S>, 2>>;

public:
    /*
     * Constructor needs to make board empty.
     */
    State() {
        this->board = new signed char[S][S];
        std::fill<signed char *, signed char>(&this->board[0][0], &this->board[0][0] + sizeof(this->board), PLAYER_NONE);
    }

    /*
    * Copies turn and steals board array.
    */
    State(State<HA<S>, HA<S>, 2> &&other) {
        this->turn = other.turn;
        this->board = other.board;
        other.board = nullptr;
    }

    /*
    * Copies turn and board array.
    */
    State(const State<HA<S>, HA<S>, 2> &other) {
        this->turn = other.turn;
        std::memcpy(this->board, other.board, sizeof(this->board));
    }

    /*
    * Copies turn and board array.
    */
    State<HA<S>, HA<S>, 2> &operator=(const State<HA<S>, HA<S>, 2> &other) {
        this->turn = other.turn;
        std::memcpy(this->board, other.board, sizeof(this->board));
        return *this;
    }

    /*
    * Copies turn and steals board array.
    */
    State<HA<S>, HA<S>, 2> &operator=(State<HA<S>, HA<S>, 2> &&other) {
        if (this->board != nullptr) {
            delete this->board;
        }
        this->turn = other.turn;
        this->board = other.board;
        other.board = nullptr;
        return *this;
    }

    /*
     * If some data was owned, delete it
     */
    ~State() {
        if (this->board != nullptr) {
            delete this->board;
        }
    }

    /*
     * Gotta test for equality
     */
    bool operator==(const State<HA<S>, HA<S>, 2> &other) const {
        for (int x = 0; x < S; x++) {
            for (int y = 0; y < S; y++) {
                if (this->board[x][y] != other.board[x][y]) {
                    return false;
                }
            }
        }
        return this->turn == other.turn;
    }

    /*
     *
     */
    int whose_turn() const {
        return this->turn;
    }

    /*
     *
     */
    int who_won() const {
        /* each char in each array denotes that the respective player owns the
         * tile at the x or y value on that border. The four chars represent
         * how many such tiles are actually owned in each array.*/
        signed char one_bottom[S]   , one_top[S]   , two_left[S]   , two_right[S]   ,
                    one_bottom_i = 0, one_top_i = 0, two_left_i = 0, two_right_i = 0;
        const signed char M = S - 1;

        for (char i = 0; i < S; i++) {
            if (this->board[i][0] == PLAYER_ONE) {
                one_bottom[one_bottom_i++] = i;
            }
            if (this->board[i][M] == PLAYER_ONE) {
                one_top[one_top_i++] = i;
            }
            if (this->board[0][i] == PLAYER_TWO) {
                two_left[two_left_i++] = i;
            }
            if (this->board[M][i] == PLAYER_TWO) {
                two_left[two_right_i++] = i;
            }
        }

        // if any of a player's pieces on opposite edges connects, they win
        for (char bottom = 0; bottom < one_bottom_i; bottom++) {
            for (char top = 0; top < one_top_i; top++) {
                if (this->is_connected(one_bottom[bottom], 0, one_top[top], M)) {
                    return PLAYER_ONE;
                }
            }
        }
        for (char left = 0; left < two_left_i; left++) {
            for (char right = 0; right < two_right_i; right++) {
                if (this->is_connected(0, two_left[left], M, two_right[right])) {
                    return PLAYER_TWO;
                }
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
            case PLAYER_ONE:
                score[0] = 1l;
                score[1] = 0l;
                return;
            case PLAYER_TWO:
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
    State<HA<S>, HA<S>, 2> *succeed(const HA<S> &action) const {
        auto next = new State<HA<S>, HA<S>, 2>(*this);
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
    State<HA<S>, HA<S>, 2> *succeed(const HA<S> &action, HA<S> &baction) const {
        auto next = new State<HA<S>, HA<S>, 2>(*this);
        next->board[action.x][action.y] = action.whose;
        next->turn = (action.whose + 1) % 2;

        baction = action;
        return next;
    }

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    */
    State<HA<S>, HA<S>, 2> &succeed_in_place(const HA<S> &action) {
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
    State<HA<S>, HA<S>, 2> &succeed_in_place(const HA<S> &action, HA<S> &baction) {
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
    State<HA<S>, HA<S>, 2> *reverse(const HA<S> &baction) const {
        auto previous = new State<HA<S>, HA<S>, 2>(*this);
        previous->board[baction.x][baction.y] = PLAYER_NONE;
        previous->turn = baction.whose;
        return previous;
    }

    /*
    * To undoe an action, remove a player's value from the tile they claimed in
    * their action.
    * Then set the current turn as belonging to that player.
    */
    State<HA<S>, HA<S>, 2> &reverse_in_place(const HA<S> &baction) {
        this->board[baction.x][baction.y] = PLAYER_NONE;
        this->turn = baction.whose;
        return *this;
    }

    
    /*
    * Slight wrapper around get_actions(std::vector<HA<S>> &).
    */
    [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
    std::vector<HA<S>> *get_actions() const {
        std::vector<HA<S>> *a = new std::vector<HA<S>>();
        this->get_actions(*a);    
        return a;
    }

    /*
    * For every tile on the board, if that tile is empty, that's a valid location
    * a player could claim as their next move.
    */
    void get_actions(std::vector<HA<S>> &buffer) const {
        // If someone has already won, there are no actions
        if (this->who_won() != PLAYER_NONE) {
            return;
        }

        signed char whose_turn = this->turn;
        buffer.reserve(S * S);
        for (int x = 0; x < S; x++) {
            for (int y = 0; y < S; y++) {
                if (this->board[x][y] == PLAYER_NONE) {
                    buffer.emplace_back(x, y, whose_turn);
                }
            }
        }
    }

private:
    signed char turn = 0;
    signed char (*board)[S] = nullptr;

    /**
     * is_connected tells whether two points in a Hex game are connected
     * by like colored tiles (including the two original points).
     * @param x1 x coordinate of point 1.
     * @param y1 y coordinate of point 1.
     * @param x2 x coordinate of point 2.
     * @param y2 y coordinate of point 2.
     * @return   true if the points are connected, else false.
     */
    bool is_connected(char x1, char y1, char x2, char y2) const {
        const signed char COLOR = this->board[x1][y1];
        signed char neighbors[6][2], n_count, nx, ny;
        bool visited[S][S] = { false }, connected = false;
        visited[x1][y1] = true;
        std::vector<char> x_stack, y_stack;
        x_stack.push_back(x1);
        y_stack.push_back(y1);

        // for each tile we can touch (in the stack),
        // pop it off and add its neighbors to the stack if unvisited.
        while (x_stack.size() > 0) {
            // fill up neighbors buffer and n_count - how many there are
            n_count = State<HA<S>, HA<S>, 2>::get_neighbors(x1, y1, neighbors);
            while (--n_count>=0) {
                nx = neighbors[n_count][0];
                ny = neighbors[n_count][1];

                if (this->board[nx][ny] == COLOR) {
                    if (nx == x2 && ny == y2) {
                        return true;
                    }
                    // if we hadn't seen it before, mark tile as visited and
                    // push onto the stack.
                    if (!visited[nx][ny]) {
                        visited[nx][ny] == true;
                        x_stack.push_back(nx);
                        y_stack.push_back(ny);
                    }
                }
            }
        }
        return connected;
    }

    /**
    * get_neighbors fills an array with the coordinates of
    * valid neighbors of any hexagon.
    * A neighbor is invalid if its x or y coordinate lies outside
    * [0, S - 1].
    * @param x      x coordinate of the point whose neighbors to find.
    * @param y      y coordinate of the point whose neighbors to find.
    * @param buffer buffer to fill with coordinates of neighbors.
    * @return       how many valid neighbors there were
    */
    static int get_neighbors(signed char x, signed char y, signed char buffer[6][2]) {
        signed char spots[6][2] = {
                                     x     , static_cast<signed char>(y + 1),
            static_cast<signed char>(x - 1), static_cast<signed char>(y + 1),
            static_cast<signed char>(x - 1),                          y     ,
                                     x     , static_cast<signed char>(y - 1),
            static_cast<signed char>(x + 1), static_cast<signed char>(y - 1),
            static_cast<signed char>(x + 1),                          y     ,
        };
        bool valid[6] = { true };

        if (x == 0) {
            valid[1] = false, valid[2] = false;
        } else if (x == S - 1) {
            valid[4] = false, valid[5] = false;
        }

        if (y == 0) {
            valid[3] = false, valid[4] = false;
        } else if (y == S - 1) {
            valid[0] = false, valid[1] = false;
        }

        int total = 0;
        for (int i = 0; i < 6; i++) {
            if (valid[i]) {
                buffer[i][0] = spots[i][0], buffer[i][1] = spots[i][1];
                total++;
            }
        }
        return total;
    }
};

}

template<unsigned char S>
struct std::hash<GameState::State<GameState::HA<S>, GameState::HA<S>, 2>> {
    size_t operator()(const GameState::State<GameState::HA<S>, GameState::HA<S>, 2> &state) {
        size_t result = 0;
        size_t pow_three = 1;
        for (int x = 0; x < S; x++) {
            for (int y = 0; y < S; y++) {
                result += ((*state.board)[x][y] + 1) * pow_three;
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

#endif // !GAMES_HEX_HEXSTATE_HPP

