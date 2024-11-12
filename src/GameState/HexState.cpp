/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

using std::array;
using GameState::HexState;
using GameState::HexState::PLAYERS::PLAYER_ONE;
using GameState::HexState::PLAYERS::PLAYER_TWO;
using GameState::HexState::PLAYERS::PLAYER_NONE;

/*************************************************
 * Methods for HexState                          *
 ************************************************/

bool HexState::operator==(const HexState &other) const {
    return this->board == other.board &&
           this->turn  == other.turn;
}

bool HexState::operator!=(const HexState &other) const {
    return !(*this == other);
}

bool HexState::board_state_equal(const HexState &other) const {
    return this->board != other.board;
}

HexState::PLAYERS HexState::whose_turn() const {
    return this->turn;
}

// TODO: this is genuinely some of the most disgusting code I've ever written
// please redo this at some point in the future
HexState::PLAYERS HexState::who_won() const {
    /* each char in each array denotes that the respective player owns the
     * tile at the x or y value on that border. The four chars represent
     * how many such tiles are actually owned in each array.*/
    unsigned char one_bottom[BOARD_SIZE], one_top[BOARD_SIZE], two_left[BOARD_SIZE], two_right[BOARD_SIZE],
                  one_bottom_i = 0      , one_top_i = 0      , two_left_i = 0      , two_right_i = 0      ;
    const int MAX_SIZE = BOARD_SIZE - 1;

    for (int i = 0; i < BOARD_SIZE; i++) {
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
    for (int bottom = 0; bottom < one_bottom_i; bottom++) {
        for (int top = 0; top < one_top_i; top++) {
            if (this->is_connected(one_bottom[bottom], 0, one_top[top], MAX_SIZE)) {
                return PLAYER_ONE;
            }
        }
    }
    for (int left = 0; left < two_left_i; left++) {
        for (int right = 0; right < two_right_i; right++) {
            if (this->is_connected(0, two_left[left], MAX_SIZE, two_right[right])) {
                return PLAYER_TWO;
            }
        }
    }
    return PLAYER_NONE;
}

HexState &HexState::succeed_in_place(const Action &action) {
    assert(action.x < BOARD_SIZE);
    assert(action.y < BOARD_SIZE);
    this->board[action.x][action.y] = action.whose;
    this->turn = (this->turn == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    return *this;
}

HexState &HexState::succeed_in_place(const Action &action, Action &baction) {
    assert(action.x < BOARD_SIZE);
    assert(action.y < BOARD_SIZE);
    this->board[action.x][action.y] = action.whose;
    this->turn = (this->turn == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;

    baction = action;
    return *this;
}

HexState &HexState::reverse_in_place(const Action &baction) {
    assert(baction.x < BOARD_SIZE);
    assert(baction.y < BOARD_SIZE);
    this->board[baction.x][baction.y] = PLAYER_NONE;
    this->turn = baction.whose;
    return *this;
}

void HexState::get_actions(std::vector<Action> &buffer) const {
    // If someone has already won, there are no actions
    if (this->who_won() != PLAYER_NONE) {
        return;
    }

    HexState::PLAYERS whose_turn = this->turn;
    buffer.reserve(BOARD_SIZE * BOARD_SIZE);
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (this->board[x][y] == PLAYER_NONE) {
                buffer.emplace_back(x, y, whose_turn);
            }
        }
    }
}

void GameState::HexState::simple_string(std::ostream &out) const {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            switch (this->board[x][y]) {
                case PLAYER_ONE:
                    out << '1';
                    break;
                case PLAYER_TWO:
                    out << '2';
                    break;
                case PLAYER_NONE:
                    out << '0';
                    break;
            }
        }
    }
    out << '\n';
}

HexState::PLAYERS HexState::at(int x, int y) const {
    assert(x > 0);
    assert(y > 0);
    assert(x < BOARD_SIZE);
    assert(y < BOARD_SIZE);
    return this->board[x][y];
}

HexState &HexState::flip(HexState::AXIS axis) {
    switch (axis) {
        case HexState::HORIZONTAL:
            for (int x = 0; x < BOARD_SIZE / 2; x++) {
                for (int y = 0; y < BOARD_SIZE; y++) {
                    std::swap(this->board[x][y], this->board[BOARD_SIZE - x - 1][y]);
                }
            }
            break;
        case HexState::VERTICAL:
            for (int x = 0; x < BOARD_SIZE; x++) {
                for (int y = 0; y < BOARD_SIZE / 2; y++) {
                    std::swap(this->board[x][y], this->board[x][BOARD_SIZE - y - 1]);
                }
            }
            break;
        case HexState::BOTH:
            return (*this).flip(HexState::AXIS::VERTICAL).flip(HexState::AXIS::HORIZONTAL);
    }
    return *this;
}

bool HexState::verify_board_state() {
    for (const array<HexState::PLAYERS, BOARD_SIZE> &x_row : this->board) {
        for (HexState::PLAYERS p : x_row) {
            if (p < PLAYER_ONE || p > PLAYER_NONE) {
                return false;
            }
        }
    }
    return (this->turn >= PLAYER_ONE && this->turn <= PLAYER_NONE);
}

// TODO: someday change these from being signed chars to ints
bool HexState::is_connected(signed char x1, signed char y1, signed char x2, signed char y2) const {
    const unsigned char COLOR = this->board[x1][y1];
    signed char neigh_x, neigh_y;
    std::vector<std::array<signed char, 2>> neighbors;
    bool visited[BOARD_SIZE][BOARD_SIZE] = {{ false }}, connected = false;
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

void HexState::get_neighbors(signed char x, signed char y, std::vector<std::array<signed char, 2>> &output) const {
    signed char neighbors[6][2] = {
        {static_cast<signed char>(x + 1),                          y     },
        {                         x     , static_cast<signed char>(y + 1)},
        {static_cast<signed char>(x - 1), static_cast<signed char>(y + 1)},
        {static_cast<signed char>(x - 1),                          y     },
        {                        x     , static_cast<signed char>(y - 1) },
        {static_cast<signed char>(x + 1), static_cast<signed char>(y - 1)},
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

/*************************************************
 * Methods for friends of HexState               *
 ************************************************/

size_t std::hash<HexState>::operator()(const GameState::HexState &state) {
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

std::ostream &GameState::operator<<(std::ostream &out, const GameState::HexState &state) {
    out << "╭";
    for (int t = 0; t < BOARD_SIZE; t++) {
        out << "──";
    }
    out << "╮\n";

    for (int y = BOARD_SIZE - 1; y >= 0; y--) {
        out << "│";
        for (int x = 0; x < BOARD_SIZE; x++) {
            switch (state.board[x][y]) {
                case PLAYER_NONE:
                    out << "  ";
                    break;
                case PLAYER_ONE:
                    out << "1 ";
                    break;
                case PLAYER_TWO:
                    out << "2 ";
                    break;
                default:
                    out << "?" << (int) state.board[x][y];
                    break;
            }
        }
        out << "│\n";
    }
    out << "╰";
    for (int t = 0; t < BOARD_SIZE; t++) {
        out << "──";
    }
    out << "╯\n";
    return out;
}

