/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef GAMES_HEX_HEXSTATE_HPP
#define GAMES_HEX_HEXSTATE_HPP
#include <array>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <type_traits>
#include <vector>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

#include "hex-ai/GameState/enums.hpp"
#include "hex-ai/GameState/Action.hpp"

namespace GameState {

/**
 * HexState represents a state of a game of Hex.
 * This includes a board of pieces which belongs to two players
 */
template<int bsize, typename std::enable_if<(bsize > 0), int>::type = 0>
class HexState {
    /*
     * Give access to std::hash of HexState private members like the board so
     * that it can be hashed.
     */
    friend class std::hash<GameState::HexState<bsize>>;

public:
    class ActionIterator {
        friend class GameState::HexState<bsize>;

    public:
        const GameState::Action &operator*() const {
            return this->a;
        }

        GameState::HexState<bsize>::ActionIterator operator++(int) {
            ActionIterator copy = *this;
            if (this->x == bsize) {
                return *this;
            }
            if (++(this->y) == bsize) {
                this->y = 0;
                if (++(this->x) == bsize) {
                    return *this;
                }
            }
            if (state[x][y] == PLAYER_NONE) {
                this->a = {x, y, this->default_player};
            } else {
                (*this)++;
            }
            return copy;
        }

        int operator<=>(const ActionIterator &other) const {
            if (&this->state != &other.state) {
                return 1;
            }
            return ((this->x - other.x) * bsize + this->y - other.y);
        }

    private:
        const HexState<bsize> &state;
        unsigned char x, y;
        GameState::PLAYERS default_player;
        GameState::Action a;

        ActionIterator(
            const HexState<bsize> &state,
            GameState::PLAYERS player = PLAYER_NONE,
            unsigned char x = 0,
            unsigned char y = 0
        ) : state(state), x(x), y(y), default_player(player) {
            if (x < bsize && y < bsize) {
                if (state[x][y] == PLAYER_NONE) {
                    this->a = {x, y, player};
                } else {
                    (*this)++;
                }
            }
        };
    };

    ActionIterator actions_begin(PLAYERS p = PLAYER_NONE) const {
        return { *this, p };
    }

    ActionIterator actions_end() const {
        return { *this, PLAYER_NONE, bsize, 0 };
    }

    /**
     * Test for equality against another HexState instance.
     * Two states are considered equal if the contents of their internal
     * `board` arrays are the same.
     *
     * @param other the state to test equality against.
     * @return      true if the two states are equal, false otherwise.
     */
    bool operator==(const HexState &other) const {
        return this->board == other.board;
    }

    /**
     * Test for inequality against another HexState instance.
     *
     * @param other the state to test inequality against.
     * @return      true if the states are not equal, false otherwise.
     */
    bool operator!=(const HexState &other) const {
        return !(*this==other);
    }

    /**
     * Allows caller to access HexState as though it were an array
     * of player values (as if it were the internal board it holds).
     *
     * @param i the x coordinate.
     * @return the array of values at that x coordinate.
     */
    const std::array<PLAYERS, bsize> &operator[](size_t i) const {
        assert(i < bsize);
        return this->board[i];
    }

    /**
     * Tell which player has won the game.
     *
     * @return an element from the PLAYERS enum detailing which player has won.
     */
    GameState::PLAYERS who_won() const {
        int xstack[bsize * bsize], ystack[bsize * bsize], stack = 0;
        PLAYERS mask[bsize][bsize] = { PLAYER_NONE };

        // for all player one positions starting at the bottom
        for (int start = 0; start < bsize; start++) {
            if (
                this->board[start][0] == PLAYER_ONE &&
                mask[start][0] == PLAYER_NONE
            ) {
                xstack[stack] = start;
                ystack[stack++] = 0;

                while (stack) {
                    // get next tile and mark as visited
                    int x = xstack[--stack];
                    int y = ystack[stack];
                    mask[x][y] = PLAYER_ONE;

                    if (
                        x + 1 < bsize &&
                        mask[x + 1][y] == PLAYER_NONE &&
                        this->board[x + 1][y] == PLAYER_ONE
                    ) {
                        mask[x + 1][y] = PLAYER_ONE;
                        xstack[stack] = x + 1;
                        ystack[stack++] = y;
                    }
                    if (
                        y + 1 < bsize &&
                        mask[x][y + 1] == PLAYER_NONE &&
                        this->board[x][y + 1] == PLAYER_ONE
                    ) {
                        if (y + 1 == bsize - 1) {
                            return PLAYER_ONE;
                        } else {
                            mask[x][y + 1] = PLAYER_ONE;
                            xstack[stack] = x;
                            ystack[stack++] = y + 1;
                        }
                    }
                    if (
                        x - 1 >= 0 &&
                        y + 1 < bsize &&
                        mask[x - 1][y + 1] == PLAYER_NONE &&
                        this->board[x - 1][y + 1] == PLAYER_ONE
                    ) {
                        if (y + 1 == bsize - 1) {
                            return PLAYER_ONE;
                        } else {
                            mask[x - 1][y + 1] = PLAYER_ONE;
                            xstack[stack] = x - 1;
                            ystack[stack++] = y + 1;
                        }
                    }
                    if (
                        x - 1 >= 0 &&
                        mask[x - 1][y] == PLAYER_NONE &&
                        this->board[x - 1][y] == PLAYER_ONE
                    ) {
                        mask[x - 1][y] = PLAYER_ONE;
                        xstack[stack] = x - 1;
                        ystack[stack++] = y;
                    }
                    if (
                        y - 1 >= 0 &&
                        mask[x][y - 1] == PLAYER_NONE &&
                        this->board[x][y - 1] == PLAYER_ONE
                    ) {
                        mask[x][y - 1] = PLAYER_ONE;
                        xstack[stack] = x;
                        ystack[stack++] = y - 1;
                    }
                    if (
                        x + 1 < bsize &&
                        y - 1 >= 0 &&
                        mask[x + 1][y - 1] == PLAYER_NONE &&
                        this->board[x + 1][y - 1] == PLAYER_ONE
                    ) {
                        mask[x + 1][y - 1] = PLAYER_ONE;
                        xstack[stack] = x + 1;
                        ystack[stack++] = y - 1;
                    }
                }
            }
        }

        // for all player two positions starting at the left
        for (int start = 0; start < bsize; start++) {
            if (
                this->board[0][start] == PLAYER_TWO &&
                mask[0][start] == PLAYER_NONE
            ) {
                xstack[stack] = 0;
                ystack[stack++] = start;

                while (stack) {
                    // get next tile and mark as visited
                    int x = xstack[--stack];
                    int y = ystack[stack];
                    mask[x][y] = PLAYER_TWO;

                    if (
                        x + 1 < bsize &&
                        mask[x + 1][y] == PLAYER_NONE &&
                        this->board[x + 1][y] == PLAYER_TWO
                    ) {
                        if (x + 1 == bsize - 1) {
                            return PLAYER_TWO;
                        } else {
                            mask[x + 1][y] = PLAYER_TWO;
                            xstack[stack] = x + 1;
                            ystack[stack++] = y;
                        }
                    }
                    if (
                        y + 1 < bsize &&
                        mask[x][y + 1] == PLAYER_NONE &&
                        this->board[x][y + 1] == PLAYER_TWO
                    ) {
                        mask[x][y + 1] = PLAYER_TWO;
                        xstack[stack] = x;
                        ystack[stack++] = y + 1;
                    }
                    if (
                        x - 1 >= 0 &&
                        y + 1 < bsize &&
                        mask[x - 1][y + 1] == PLAYER_NONE &&
                        this->board[x - 1][y + 1] == PLAYER_TWO
                    ) {
                        mask[x - 1][y + 1] = PLAYER_TWO;
                        xstack[stack] = x - 1;
                        ystack[stack++] = y + 1;
                    }
                    if (
                        x - 1 >= 0 &&
                        mask[x - 1][y] == PLAYER_NONE &&
                        this->board[x - 1][y] == PLAYER_TWO
                    ) {
                        mask[x - 1][y] = PLAYER_TWO;
                        xstack[stack] = x - 1;
                        ystack[stack++] = y;
                    }
                    if (
                        y - 1 >= 0 &&
                        mask[x][y - 1] == PLAYER_NONE &&
                        this->board[x][y - 1] == PLAYER_TWO
                    ) {
                        mask[x][y - 1] = PLAYER_TWO;
                        xstack[stack] = x;
                        ystack[stack++] = y - 1;
                    }
                    if (
                        x + 1 < bsize &&
                        y - 1 >= 0 &&
                        mask[x + 1][y - 1] == PLAYER_NONE &&
                        this->board[x + 1][y - 1] == PLAYER_TWO
                    ) {
                        if (x + 1 == bsize - 1) {
                            return PLAYER_TWO;
                        } else {
                            mask[x + 1][y - 1] = PLAYER_TWO;
                            xstack[stack] = x + 1;
                            ystack[stack++] = y - 1;
                        }
                    }
                }
            }
        }

        return PLAYER_NONE;
    }

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    */
    HexState &succeed(const Action &action) {
        assert(action.x < bsize);
        assert(action.y < bsize);
        this->board[action.x][action.y] = action.whose;
        return *this;
    }

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    * To tell how to undo an action, you just need to know where a player claimed
    * a tile.
    */
    HexState &succeed(const Action &action, Action &baction) {
        assert(action.x < bsize);
        assert(action.y < bsize);
        baction.x = action.x;
        baction.y = action.y;
        baction.whose = this->board[action.x][action.y];
        this->board[action.x][action.y] = action.whose;
        return *this;
    }

    /*
    * For every tile on the board, if that tile is empty, that's a valid location
    * a player could claim as their next move.
    */
    void get_actions(std::vector<Action> &buffer, PLAYERS turn = PLAYER_NONE) const {
        // If someone has already won, there are no actions
        if (this->who_won() != PLAYER_NONE) {
            return;
        }

        buffer.reserve(bsize * bsize);
        for (int x = 0; x < bsize; x++) {
            for (int y = 0; y < bsize; y++) {
                if (this->board[x][y] == PLAYER_NONE) {
                    buffer.emplace_back(x, y, turn);
                }
            }
        }
    }

    /**
     * prints out a simple string representation of a state.
     *
     * @param out the ostream to write the string to
     */
    void simple_string(std::ostream &out) const {
        for (int x = 0; x < bsize; x++) {
            for (int y = 0; y < bsize; y++) {
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

    /**
     * allows you to flip a board on a certain axis
     *
     * @param axis the axis on which you wish to flip the board
     * @return reference to self
     */
    HexState &flip(GameState::AXIS axis) {
        switch (axis) {
            case GameState::HORIZONTAL:
                for (int x = 0; x < bsize / 2; x++) {
                    for (int y = 0; y < bsize; y++) {
                        std::swap(this->board[x][y], this->board[bsize - x - 1][y]);
                    }
                }
                break;
            case GameState::VERTICAL:
                for (int x = 0; x < bsize; x++) {
                    for (int y = 0; y < bsize / 2; y++) {
                        std::swap(this->board[x][y], this->board[x][bsize - y - 1]);
                    }
                }
                break;
            case GameState::BOTH:
                return (*this).flip(GameState::VERTICAL).flip(GameState::HORIZONTAL);
        }
        return *this;
    }

    /**
     * verify_board_state makes sure that all values in the internal board
     * are valid instances of HexState::PLAYERS.
     * This probably shouldn't be used a ton,
     * as it should be able to be assumed that the state is always valid.
     * Occasionally (like when reading from files) I imagine it is helpful to
     * check first though.
     * That's what this function was originally intended for.
     * 
     * @return true if all values are valid.
     *         false if any value is not valid.
     */
    bool verify_board_state() const {
        for (const std::array<GameState::PLAYERS, bsize> &x_row : this->board) {
            for (GameState::PLAYERS p : x_row) {
                if (p < PLAYER_ONE || p > PLAYER_NONE) {
                    return false;
                }
            }
        }
        return (this->turn >= PLAYER_ONE && this->turn <= PLAYER_NONE);
    }

    /**
     * Serializes a HexState instance to a cereal archive
     * using the cereal library.
     *
     * @param archive the cereal archive to write to
     */
    template<class Archive>
    void save(Archive &archive) const {
        uint8_t pack4 = 0;
        int packed_in = 0;

        // pack in all the board states
        for (int x = 0; x < bsize; x++) {
            for (int y = 0; y < bsize; y++) {
                pack4 |= (0x03 & this->board[x][y]);
                packed_in++;

                if (packed_in == 4) {
                    packed_in = 0;
                    archive(pack4);
                    pack4 = 0;
                } else {
                    pack4 <<= 2;
                }
            }
        }

        pack4 <<= (2 * (4 - packed_in));
        if (!(packed_in == 0)) {
            archive(pack4);
        }
    }

    /**
     * Reads in a serialized HexState instance from a cereal archive
     * using the cereal library.
     *
     * @param archive the cereal archive to read from.
     * @raises cereal::Exception if the gamestate read in had bad values.
     */
    template<class Archive>
    void load(Archive &archive) {
        uint8_t pack4 = 0;
        uint8_t value;
        int packed_in = 0;

        // get all board states
        for (int x = 0; x < bsize; x++) {
            for (int y = 0; y < bsize; y++) {
                if (packed_in == 0) {
                    archive(pack4);
                    packed_in = 4;
                }

                value = (pack4 & 0xc0) >> 6;
                // okay to cast - we check correctness later
                this->board[x][y] = static_cast<GameState::PLAYERS>(value);
                pack4 <<= 2;
                packed_in--;
            }
        }

        if (!this->verify_board_state()) {
            throw cereal::Exception("Bad HexState value in cereal import.");
        }
    }

private:
    std::array<std::array<GameState::PLAYERS, bsize>, bsize> board {};
};

template<>
inline GameState::PLAYERS HexState<1>::who_won() const {
    return this->board[0][0];
}

}

/**
* Template specialization of std::hash for HexState
*/
template<int bsize>
struct std::hash<GameState::HexState<bsize>> {
    size_t operator()(const GameState::HexState<bsize> &state) {
        size_t result = 0;
        size_t pow_three = 1;
        for (int x = 0; x < bsize; x++) {
            for (int y = 0; y < bsize; y++) {
                result += ((state.board)[x][y] + 1) * pow_three;
                pow_three *= 3;
            }
        }

        return result;
    }
};

#endif // !GAMES_HEX_HEXSTATE_HPP

