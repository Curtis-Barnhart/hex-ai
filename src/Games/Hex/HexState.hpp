#ifndef GAMES_HEX_HEXSTATE_HPP
#define GAMES_HEX_HEXSTATE_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <string>
#include <vector>

#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define PLAYER_NONE -1
#define BOARD_SIZE 3

namespace GameState {
/**
 * HexState represents a state of a game of Hex.
 */
class HexState {
    friend std::ostream &operator<<(std::ostream &out, const HexState &state) {
        int p_rows = 3 * BOARD_SIZE - 1;
        int p_cols = 2 * BOARD_SIZE;

        for (int row = p_rows - 1; row >= 0; row--) {
            for (int col = 0; col < p_cols; col++) {
                int boardx = col / 2;
                int pre_board_y = row - boardx;
                int boardy;
                if (pre_board_y < 0) {
                    boardy = (pre_board_y - 1) / 2;
                } else {
                    boardy = pre_board_y / 2;
                }
                std::string board_char;
                if (boardx < BOARD_SIZE && boardx >= 0 && boardy < BOARD_SIZE && boardy >= 0) {
                    switch (state.board[boardx][boardy]) {
                        case PLAYER_NONE:
                            board_char = "  ";
                            break;
                        case PLAYER_ONE:
                            board_char = "▓▓";
                            break;
                        case PLAYER_TWO:
                            board_char = "░░";
                            break;
                    }
                    // out << "(" << boardx << "," << boardy << ")" << board_char;
                    out << board_char;
                } else {
                    // out << "(" << boardx << "," << boardy << ")" << "X";
                    out << "  ";
                }
            }
            out << "\n";
        }

        // for (int y = BOARD_SIZE - 1; y >= 0; y--) {
        //     for (int x = 0; x < BOARD_SIZE; x++) {
        //         switch (state.board[x][y]) {
        //             case PLAYER_NONE:
        //                 out << ' ';
        //                 break;
        //             case PLAYER_ONE:
        //                 out << '1';
        //                 break;
        //             case PLAYER_TWO:
        //                 out << '2';
        //                 break;
        //         }
        //     }
        //     out << "\n";
        // }
        // out << std::endl;
        return out;
    }

    friend class std::hash<GameState::HexState>;

public:
    struct Action {
        signed char x = 0, y = 0, whose = PLAYER_NONE;

        /**
         * TODO: this is unneeded isn't it
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
     * Constructor needs to make board empty.
     */
    HexState() {
        this->board = new signed char[BOARD_SIZE][BOARD_SIZE];
        std::fill<signed char *, signed char>(&this->board[0][0], &this->board[0][0] + BOARD_SIZE * BOARD_SIZE, PLAYER_NONE);
    }

    /*
    * Copies turn and steals board array.
    */
    HexState(HexState &&other) {
        this->turn = other.turn;
        this->board = other.board;
        other.board = nullptr;
    }

    /*
    * Copies turn and board array.
    */
    HexState(const HexState &other) {
        this->turn = other.turn;
        std::memcpy(&this->board[0][0], &other.board[0][0], sizeof(signed char) * BOARD_SIZE * BOARD_SIZE);
    }

    /*
    * Copies turn and board array.
    */
    HexState &operator=(const HexState &other) {
        this->turn = other.turn;
        std::memcpy(&this->board[0][0], &other.board[0][0], sizeof(signed char) * BOARD_SIZE * BOARD_SIZE);
        return *this;
    }

    /*
    * Copies turn and steals board array.
    */
    HexState &operator=(HexState &&other) {
        if (this->board != nullptr) {
            delete[] this->board;
        }
        this->turn = other.turn;
        this->board = other.board;
        other.board = nullptr;
        return *this;
    }

    /*
     * If some data was owned, delete it
     */
    ~HexState() {
        if (this->board != nullptr) {
            delete[] this->board;
        }
    }

    /*
     * Gotta test for equality
     */
    bool operator==(const HexState &other) const {
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
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
        signed char one_bottom[BOARD_SIZE], one_top[BOARD_SIZE], two_left[BOARD_SIZE], two_right[BOARD_SIZE],
                    one_bottom_i = 0      , one_top_i = 0      , two_left_i = 0      , two_right_i = 0      ;
        const signed char MAX_SIZE = BOARD_SIZE - 1;

        for (char i = 0; i < BOARD_SIZE; i++) {
            if (this->board[i][0] == PLAYER_ONE) {
                one_bottom[one_bottom_i++] = i;
            }
            if (this->board[i][MAX_SIZE] == PLAYER_ONE) {
                one_top[one_top_i++] = i;
            }
            if (this->board[0][i] == PLAYER_TWO) {
                two_left[two_left_i++] = i;
            }
            if (this->board[MAX_SIZE][i] == PLAYER_TWO) {
                two_right[two_right_i++] = i;
            }
        }

        // if any of a player's pieces on opposite edges connects, they win
        for (char bottom = 0; bottom < one_bottom_i; bottom++) {
            for (char top = 0; top < one_top_i; top++) {
                if (this->is_connected(one_bottom[bottom], 0, one_top[top], MAX_SIZE)) {
                    return PLAYER_ONE;
                }
            }
        }
        for (char left = 0; left < two_left_i; left++) {
            for (char right = 0; right < two_right_i; right++) {
                if (this->is_connected(0, two_left[left], MAX_SIZE, two_right[right])) {
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
    HexState *succeed(const Action &action) const {
        auto next = new HexState(*this);
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
    HexState *succeed(const Action &action, Action &baction) const {
        auto next = new HexState(*this);
        next->board[action.x][action.y] = action.whose;
        next->turn = (action.whose + 1) % 2;

        baction = action;
        return next;
    }

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    */
    HexState &succeed_in_place(const Action &action) {
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
    HexState &succeed_in_place(const Action &action, Action &baction) {
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
    HexState *reverse(const Action &baction) const {
        auto previous = new HexState(*this);
        previous->board[baction.x][baction.y] = PLAYER_NONE;
        previous->turn = baction.whose;
        return previous;
    }

    /*
    * To undoe an action, remove a player's value from the tile they claimed in
    * their action.
    * Then set the current turn as belonging to that player.
    */
    HexState &reverse_in_place(const Action &baction) {
        this->board[baction.x][baction.y] = PLAYER_NONE;
        this->turn = baction.whose;
        return *this;
    }

    
    /*
    * Slight wrapper around get_actions(std::vector<HA<S>> &).
    */
    [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
    std::vector<Action> *get_actions() const {
        std::vector<Action> *a = new std::vector<Action>();
        this->get_actions(*a);    
        return a;
    }

    /*
    * For every tile on the board, if that tile is empty, that's a valid location
    * a player could claim as their next move.
    */
    void get_actions(std::vector<Action> &buffer) const {
        // If someone has already won, there are no actions
        if (this->who_won() != PLAYER_NONE) {
            return;
        }

        signed char whose_turn = this->turn;
        buffer.reserve(BOARD_SIZE * BOARD_SIZE);
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                if (this->board[x][y] == PLAYER_NONE) {
                    buffer.emplace_back(x, y, whose_turn);
                }
            }
        }
    }

private:
    signed char turn = 0;
    signed char (*board)[BOARD_SIZE] = nullptr;

    /**
     * is_connected tells whether two points in a Hex game are connected
     * by like colored tiles (including the two original points).
     * @param x1 x coordinate of point 1.
     * @param y1 y coordinate of point 1.
     * @param x2 x coordinate of point 2.
     * @param y2 y coordinate of point 2.
     * @return   true if the points are connected, else false.
     */
    bool is_connected(signed char x1, signed char y1, signed char x2, signed char y2) const {
        const signed char COLOR = this->board[x1][y1];
        signed char neigh_x, neigh_y;
        std::vector<std::array<signed char, 2>> neighbors;
        bool visited[BOARD_SIZE][BOARD_SIZE] = { false }, connected = false;
        visited[x1][y1] = true;
        std::vector<signed char> x_stack, y_stack;
        x_stack.push_back(x1);
        y_stack.push_back(y1);

        // for each tile we can touch (in the stack),
        // pop it off and add its neighbors to the stack if unvisited.
        while (x_stack.size() > 0) {
            signed char next_x = x_stack.back();
            signed char next_y = y_stack.back();
            x_stack.pop_back();
            y_stack.pop_back();

            // fill up neighbors buffer
            neighbors.clear();
            HexState::get_neighbors(next_x, next_y, neighbors);
            for (std::array<signed char, 2> &neighbor : neighbors) {
                neigh_x = neighbor[0], neigh_y = neighbor[1];
                if (this->board[neigh_x][neigh_y] == COLOR) {
                    // if the touching piece is destination, perfect!
                    if (neigh_x == x2 && neigh_y == y2) {
                        return true;
                    }
                    // otherwise, if we haven't seen it before, put on stack
                    if (!visited[neigh_x][neigh_y]) {
                        visited[neigh_x][neigh_y] = true;
                        x_stack.push_back(neigh_x);
                        y_stack.push_back(neigh_y);
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
    void get_neighbors(signed char x, signed char y, std::vector<std::array<signed char, 2>> &output) const {
        const int maximum = BOARD_SIZE - 1;
        signed char neighbors[6][2] = {
            static_cast<signed char>(x + 1),                          y,
                                     x     , static_cast<signed char>(y + 1),
            static_cast<signed char>(x - 1), static_cast<signed char>(y + 1),
            static_cast<signed char>(x - 1),                          y,
                                     x     , static_cast<signed char>(y - 1),
            static_cast<signed char>(x + 1), static_cast<signed char>(y - 1),
        };

        for (int point = 0; point < 6; point++) {
            if (
                0 <= neighbors[point][0] && neighbors[point][0] < BOARD_SIZE &&
                0 <= neighbors[point][1] && neighbors[point][1] < BOARD_SIZE
            ) {
                output.push_back({neighbors[point][0], neighbors[point][1]});
            }
        }
    }
};

}

template<>
struct std::hash<GameState::HexState> {
    size_t operator()(const GameState::HexState &state) {
        size_t result = 0;
        size_t pow_three = 1;
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                result += ((state.board)[x][y] + 1) * pow_three;
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

