#ifndef GAMES_HEX_HEXSTATE_HPP
#define GAMES_HEX_HEXSTATE_HPP

#include <cstring>
#include <ostream>
#include <vector>

#include "HexStateBase.hpp"

namespace GameState {

/**
 * HexState represents a state of a game of Hex.
 */
template<char S>
class HexState: GameState::State<HexAction<S>> {
    friend std::ostream &operator<<(std::ostream &out, const GameState::HexState<S> &game) {
        for (int y = S - 1; y >= 0; y--) {
            for (int x = 0; x < S - 1; x++) {
                switch (game.board[x][y]) {
                    case GameState::HexVal::EMPTY:
                        out << ' ';
                        break;
                    case GameState::HexVal::ONE:
                        out << '1';
                        break;
                    case GameState::HexVal::TWO:
                        out << '2';
                        break;
                }
            }
            out << "\n";
        }
        out << std::endl;
        return out;
    }

public:
    /**
     * The constructor for HexState creates a state representing the very start
     * of a game of Hex.
     */
    HexState() = default;

    /**
     * The copy constructor copies the state of the board and the current turn.
     */
    HexState(const GameState::HexState<S> &other) {
        this->turn = other.turn;
        std::memcpy(this->board, other.board, sizeof(this->board));
    }

    HexState(GameState::HexState<S> &&) = default;
    // TODO: hey add in the = operator later?
    
    // methods to determine winning
    int whose_turn() const override {
        return this->turn;
    }

    // methods for move making

    /**
     * The who_won method 
     */
    int who_won() const override {
        /* each char in each array denotes that the respective player owns the
         * tile at the x or y value on that border. The four chars represent
         * how many such tiles are actually owned in each array.*/
        char one_bottom[S]   , one_top[S]   , two_left[S]   , two_right[S]   ,
             one_bottom_i = 0, one_top_i = 0, two_left_i = 0, two_right_i = 0;
        const char M = S - 1;

        for (char i = 0; i < S; i++) {
            if (this->board[i][0] == GameState::HexVal::ONE) {
                one_bottom[one_bottom_i++] = i;
            }
            if (this->board[i][M] == GameState::HexVal::ONE) {
                one_top[one_top_i++] = i;
            }
            if (this->board[0][i] == GameState::HexVal::TWO) {
                two_left[two_left_i++] = i;
            }
            if (this->board[M][i] == GameState::HexVal::TWO) {
                two_left[two_right_i++] = i;
            }
        }

        // if any of a player's pieces on opposite edges connects, they win
        for (char bottom = 0; bottom < one_bottom_i; bottom++) {
            for (char top = 0; top < one_top_i; top++) {
                if (this->is_connected(one_bottom[bottom], 0, one_top[top], M)) {
                    return 0;
                }
            }
        }
        for (char left = 0; left < two_left_i; left++) {
            for (char right = 0; right < two_right_i; right++) {
                if (this->is_connected(0, two_left[left], M, two_right[right])) {
                    return 1;
                }
            }
        }
        return -1;
    }

    GameState::HexState<S> *succeed(const GameState::HexAction<S> &action) const override {
        GameState::HexState<S> *next = new GameState::HexState<S>(*this);
        next->board[action.x][action.y] = action.whose;
        return next;
    }

    GameState::HexState<S> &succeed_in_place(const GameState::HexAction<S> &action) override {
        this->board[action.x][action.y] = action.whose;
        return *this;
    }

    std::vector<GameState::HexAction<S>> *get_actions() const override {
        auto actions = new std::vector<GameState::HexAction<S>>();
        return actions;
    }

    void get_actions(std::vector<GameState::HexAction<S>> &buffer) const override {
        return;
    }

private:
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
        const GameState::HexVal COL = this->board[x1][y1];
        char neighbors[6][2], n_count, nx, ny;
        bool visited[S][S] = { false }, connected = false;
        visited[x1][y1] = true;
        std::vector<char> x_stack, y_stack;
        x_stack.push_back(x1);
        y_stack.push_back(y1);

        // for each tile we can touch (in the stack),
        // pop it off and add its neighbors to the stack if unvisited.
        while (x_stack.size() > 0) {
            // fill up neighbors buffer and n_count - how many there are
            n_count = GameState::HexState<S>::get_neighbors(x1, y1, neighbors);
            while (--n_count>=0) {
                nx = neighbors[n_count][0];
                ny = neighbors[n_count][1];

                if (this->board[nx][ny] == COL) {
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
    static char get_neighbors(char x, char y, char buffer[6][2]) {
        char spots[6][2] = {
                              x     , static_cast<char>(y + 1),
            static_cast<char>(x - 1), static_cast<char>(y + 1),
            static_cast<char>(x - 1),                   y     ,
                              x     , static_cast<char>(y - 1),
            static_cast<char>(x + 1), static_cast<char>(y - 1),
            static_cast<char>(x + 1),                   y     ,
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

        char total = 0;
        for (size_t i = 0; i < 6; i++) {
            if (valid[i]) {
                buffer[i][0] = spots[i][0], buffer[i][1] = spots[i][1];
                total++;
            }
        }
        return total;
    }
};

}

/*
template<char S>
std::ostream &operator<<(std::ostream &out, const GameState::HexState<S> &game) {
    for (int y = S - 1; y >= 0; y--) {
        for (int x = 0; x < S - 1; x++) {
            out << game.board[x][y];
        }
        out << "\n";
    }
    out << std::endl;
    return out;
}
*/

#endif // !GAMES_HEX_HEXSTATE_HPP

