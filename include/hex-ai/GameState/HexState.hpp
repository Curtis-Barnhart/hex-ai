#ifndef GAMES_HEX_HEXSTATE_HPP
#define GAMES_HEX_HEXSTATE_HPP

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

#define BOARD_SIZE 11

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
    /**
     * HexState::Action is a class for representing an action that a player
     * can take in a game of Hex. It holds location data as well as player
     * identification.
     */
    struct Action {
        unsigned char x = 0, y = 0, whose = HexState::PLAYERS::PLAYER_NONE;

        /**
         * The default constructor creates an action at (0, 0) by PLAYER_NONE.
         */
        Action() = default;

        /**
        * Constructor sets memeber values according to the given parameters.
        * @param x the x coordinate of what tile should be claimed
        * @param y the y coordinate of what tile should be claimed
        * @param whose 0 for the first player, 1 for the second, -1 for no player
         */
        Action(unsigned char x, unsigned char y, unsigned char whose): x(x), y(y), whose(whose) {}
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

    enum PLAYERS { PLAYER_ONE, PLAYER_TWO, PLAYER_NONE };

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
    int whose_turn() const;

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
    */
    [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
    HexState *succeed(const Action &action) const;

    /*
    * If player P has an action at (x, y), then that tile in the board array should
    * hold their value.
    * To tell how to undoe an action, you just need to know where a player claimed
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

    // TODO: there may be some io exceptions you wanna throw or do I dunno
    void pack_to_stream(std::ofstream &out) const;

    // TODO: there may be some io exceptions you wanna throw or do I dunno
    void unpack_from_stream(std::ifstream &in);

private:
    unsigned char turn = 0;
    unsigned char (*board)[BOARD_SIZE] = nullptr;

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

