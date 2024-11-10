/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef GAMES_HEX_HEXSTATE_HPP
#define GAMES_HEX_HEXSTATE_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <vector>

#include <cereal/cereal.hpp>

#define BOARD_SIZE 5

namespace GameState {

/**
 * HexState represents a state of a game of Hex.
 * This includes a board of pieces which belongs to two players
 * as well as a counter of whose turn it is.
 */
class HexState {
    /*
     * Declaring a friend streaming operator so that a state may be printed.
     * This will probably have to be redone when this gets templated.
     */
    friend std::ostream &operator<<(std::ostream &out, const GameState::HexState &state);

    /*
     * Give access to std::hash of HexState private members like the board so
     * that it can be hashed.
     */
    friend class std::hash<GameState::HexState>;

public:

    // some enums
    enum PLAYERS : unsigned char { PLAYER_ONE, PLAYER_TWO, PLAYER_NONE };
    enum AXIS { VERTICAL, HORIZONTAL, BOTH };

    /**
     * HexState::Action is a class for representing an action that a player
     * can take in a game of Hex. It holds location data as well as player
     * identification.
     */
    struct Action {
        unsigned char x = 0, y = 0;
        HexState::PLAYERS whose = HexState::PLAYER_NONE;

        /**
         * The default constructor creates an action at (0, 0) by PLAYER_NONE.
         */
        Action() = default;

        /**
        * Constructor sets memeber values according to the given parameters.
        * NO ERROR CHECKING is performed on x and y to make sure they are
        * within an allowable range [0, BOARD_SIZE).
        *
        * @param x the x coordinate of what tile should be claimed.
        * @param y the y coordinate of what tile should be claimed.
        * @param whose 0 for the first player, 1 for the second, -1 for no player.
         */
        Action(unsigned char x, unsigned char y, HexState::PLAYERS whose): x(x), y(y), whose(whose) {}

        /**
         * Serializes an Action instance to a cereal archive
         * using the cereal library.
         *
         * @param archive the cereal archive to serialize the Action to.
         */
        template<class Archive>
        void serialize(Archive &archive) {
            archive(x, y, whose);
        }
    };

    /**
     * The HexActReader is an iterator over valid moves of a given HexState.
     */
    class HexActReader {
    private:
        const HexState *book;
        Action a;
        bool valid = false;
    public:
        HexActReader(const HexState &state);

        const Action &get_action() const;

        explicit operator bool() const;

        HexActReader &operator++(int);
    };

    /**
     * The constructor with no arguments will make an empty board.
     */
    HexState();

    /*
    * Copies turn and steals board array.
    */
    HexState(HexState &&other);

    /*
    * Copies turn and board array.
    */
    HexState(const HexState &other);

    /*
    * Copies turn and board array.
    */
    HexState &operator=(const HexState &other);

    /*
    * Copies turn and steals board array.
    */
    HexState &operator=(HexState &&other);

    /*
     * If some data was owned, delete it
     */
    ~HexState();

    /**
     * Test for equality against another HexState instance.
     * Two states are considered equal if the contents of their internal
     * `board` arrays are the same and if their `turn`s are the same.
     *
     * @param other the state to test equality against.
     * @return      true if the two states are equal, false otherwise.
     */
    bool operator==(const HexState &other) const;

    /**
     * Test for inequality against another HexState instance.
     *
     * @param other the state to test inequality against.
     * @return      true if the states are not equal, false otherwise.
     */
    bool operator!=(const HexState &other) const;

    /**
     * Test for equality against another HexState instance,
     * but only as far as the board state goes (not turn count)
     *
     * @param other the state to test equality against.
     * @return      true if the boards of the two states are equal, else false.
     */
    bool board_state_equal(const HexState &other) const;

    /**
     * Returns which player's turn it currently is.
     * 0 for the first player and 1 for the second.
     */
    PLAYERS whose_turn() const;

    /**
     * Tell which player has won the game.
     *
     * @return an element from the PLAYERS enum detailing which player has won.
     */
    HexState::PLAYERS who_won() const;

    /**
     * Fills in an array of doubles with the current score of the game.
     * The nth element in the array is the score of the nth player.
     * 0 indicates a loss, 1 a win, and 0.5 neither.
     *
     * @param score the array of doubles in which to write the current score.
     */
    void current_score_array(double score[2]) const;

    /**
     *
     *
     */
    double current_score_double() const;

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    *
    */
    [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
    HexState *succeed(const Action &action) const;

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    * To tell how to undo an action, you just need to know where a player claimed
    * a tile.
    */
    [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
    HexState *succeed(const Action &action, Action &baction) const;

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    */
    HexState &succeed_in_place(const Action &action);

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    * To tell how to undoe an action, you just need to know where a player claimed
    * a tile.
    */
    HexState &succeed_in_place(const Action &action, Action &baction);

    /*
    * To undoe an action, remove a player's value from the tile they claimed in
    * their action.
    * Then set the current turn as belonging to that player.
    */
    [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
    HexState *reverse(const Action &baction) const;

    /*
    * To undoe an action, remove a player's value from the tile they claimed in
    * their action.
    * Then set the current turn as belonging to that player.
    */
    HexState &reverse_in_place(const Action &baction);

    
    /*
    * Slight wrapper around get_actions(std::vector<HA<S>> &).
    */
    [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
    std::vector<Action> *get_actions() const;

    /*
    * For every tile on the board, if that tile is empty, that's a valid location
    * a player could claim as their next move.
    */
    void get_actions(std::vector<Action> &buffer) const;

    void simple_string(std::ostream &out) const;

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
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
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

        // put in the current turn
        // WARNING: this only works assuming that the area of the board
        // is not divisible by 4????
        // hopefully I don't change it from 11 anyways?
        pack4 |= (0x03 & this->turn);
        packed_in++;
        pack4 <<= (2 * (4 - packed_in));
        archive(pack4);
    }

    /**
     * Reads in a serialized HexState instance from a cereal archive
     * using the cereal library.
     *
     * @param archive the cereal archive to read from.
     * @raises cereal::Exception if the gamestate read in had bad values.
     */
    template<class Archive>
    void load(Archive &archive){
        uint8_t pack4 = 0;
        uint8_t value;
        int packed_in = 0;

        // get all board states
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                if (packed_in == 0) {
                    archive(pack4);
                    packed_in = 4;
                }

                value = (pack4 & 0xc0) >> 6;
                // okay to cast - we check correctness later
                this->board[x][y] = static_cast<HexState::PLAYERS>(value);
                pack4 <<= 2;
                packed_in--;
            }
        }

        // WARNING: just like in save,
        // load assumes that the area of a board is not divisible by four
        // (so that there are "remainder" spots and you don't need to get
        // another byte to read in the turn)
        value = (pack4 & 0xc0) >> 6;
        // okay to cast - we check correctness later
        this->turn = static_cast<HexState::PLAYERS>(value);
        if (!this->verify_board_state()) {
            throw cereal::Exception("Bad HexState value in cereal import.");
        }
    }

    /**
     * reads a HexState in from a file (must have been put there by `pack_to_stream`).
     * @param in the file to read the HexState in from
     * @return 0 if the HexState was read in successfully.
     *         1 if the eof was encountered before the entire HexState could be read.
     *         2 if the file contained a value that should not exist in the HexState.
     *         3 if the given ifstream was bad from the start (!in.good())
     */
    [[nodiscard("Return value is an error code - do not discard.")]]
    int deserialize(std::istream &in);

    /**
     * `at` allows the caller to query what piece is at a given position
     * in a HexState instance.
     * `x` and `y` must both be integers in [0, 10].
     * NO ERROR CHECKING is done to ensure that this is actually true of
     * `x` and `y`...
     *
     * @param x x coordinate (0 <= x < 11)
     * @param y y coordinate (0 <= y < 11)
     * @return value of position (x, y) on current board
     */
    PLAYERS at(int x, int y) const;

    /**
     * allows you to flip a board on a certain axis
     *
     * @param axis the axis on which you wish to flip the board
     * @return reference to self
     */
    GameState::HexState &flip(GameState::HexState::AXIS axis);

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
    bool verify_board_state();

private:
    PLAYERS turn = PLAYER_ONE;
    PLAYERS (*board)[BOARD_SIZE] = nullptr;

    /**
     * is_connected tells whether two points in a Hex game are connected
     * by like colored tiles (including the two original points).
     * @param x1 x coordinate of point 1.
     * @param y1 y coordinate of point 1.
     * @param x2 x coordinate of point 2.
     * @param y2 y coordinate of point 2.
     * @return   true if the points are connected, else false.
     */
    // TODO: someday change these from being signed chars to ints
    bool is_connected(signed char x1, signed char y1, signed char x2, signed char y2) const;

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
    void get_neighbors(signed char x, signed char y, std::vector<std::array<signed char, 2>> &output) const;
};

std::ostream &operator<<(std::ostream &out, const GameState::HexState &state);

}

/**
* Template specialization of std::hash for HexState
*/
template<>
struct std::hash<GameState::HexState> {
    size_t operator()(const GameState::HexState &state);
};


#endif // !GAMES_HEX_HEXSTATE_HPP

