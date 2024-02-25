/**
*
*/

#include <cstring>

#include "TTT_State.hpp"

using GameState::TA;
using GameState::State;

#define PLAYER_X 0
#define PLAYER_Y 1
#define PLAYER_NONE -1

/*
* Constructor doesn't need to do anything because private members of object
* have default values that indicate an empty board.
*/
State<TA, TA>::State() = default;

/*
* Copies turn and board array.
*/
State<TA, TA>::State(State<TA, TA> &&other) {
    this->turn = other.turn;
    std::memcpy(this->board, other.board, sizeof(this->board));
}

/*
* Copies turn and board array.
*/
State<TA, TA>::State(const State<TA, TA> &other) {
    this->turn = other.turn;
    std::memcpy(this->board, other.board, sizeof(this->board));
}

/*
* Copies turn and board array.
*/
State<TA, TA> &State<TA, TA>::operator=(const State<TA, TA> &other) {
    this->turn = other.turn;
    std::memcpy(this->board, other.board, sizeof(this->board));
    return *this;
}

/*
* Copies turn and board array.
*/
State<TA, TA> &State<TA, TA>::operator=(State<TA, TA> &&other) {
    this->turn = other.turn;
    std::memcpy(this->board, other.board, sizeof(this->board));
    return *this;
}

/*
* No memory is dynamically allocated in constructor, so constructor does
* nothing.
*/
State<TA, TA>::~State<TA, TA>() = default;

/*
* Simply returns the turn number.
*/
int State<TA, TA>::whose_turn() const {
    return this->turn;
}

/*
* If a player owns 3 tiles in a row either horizontally, vertically, or
* diagonally, they win.
*/
int State<TA, TA>::who_won() const {
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
    return -1;
}

/*
* If player P has an action at (x, y), then that tile in the board array should
* hold their value.
*/
State<TA, TA> *State<TA, TA>::succeed(const TA &action) const {
    auto next = new State<TA, TA>(*this);
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
State<TA, TA> *State<TA, TA>::succeed(const TA &action, TA &baction) const {
    auto next = new State<TA, TA>(*this);
    next->board[action.x][action.y] = action.whose;
    next->turn = (action.whose + 1) % 2;

    baction = action;
    return next;
}

/*
* If player P has an action at (x, y), then that tile in the board array should
* hold their value.
*/
State<TA, TA> &State<TA, TA>::succeed_in_place(const TA &action) {
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
State<TA, TA> &State<TA, TA>::succeed_in_place(const TA &action, TA &baction) {
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
State<TA, TA> *State<TA, TA>::reverse(const TA &baction) const {
    auto previous = new State<TA, TA>(*this);
    previous->board[baction.x][baction.y] = PLAYER_NONE;
    previous->turn = baction.whose;
    return previous;
}

/*
* To undoe an action, remove a player's value from the tile they claimed in
* their action.
* Then set the current turn as belonging to that player.
*/
State<TA, TA> &State<TA, TA>::reverse_in_place(const TA &baction) {
    this->board[baction.x][baction.y] = PLAYER_NONE;
    this->turn = baction.whose;
    return *this;
}

/*
* Slight wrapper around get_actions(std::vector<TA> &).
*/
std::vector<TA> *State<TA, TA>::get_actions() const {
    std::vector<TA> *a = new std::vector<TA>();
    this->get_actions(*a);    
    return a;
}

/*
* For every tile on the board, if that tile is empty, that's a valid location
* a player could claim as their next move.
*/
void State<TA, TA>::get_actions(std::vector<TA> &buffer) const {
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

