/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "hex-ai/Util/LRUCache.hpp"
#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/Action.hpp"
#include "hex-ai/GameState/enums.hpp"
#include <cassert>

namespace GameSolve {

/**
* AlphaBeta2PlayersCached is a class that provides functions to run
* the alpha beta algorithm on a game of Hex
*/
template<int bsize>
struct AlphaBeta2PlayersCached {
public:
    // tracks how many nodes have been expanded 
    long nodes_expanded = 0;
    // internal cache of nodes
    Cache::LRUCache<GameState::HexState<bsize>, bool> cache;

public:
    /**
    * The constructor takes a single parameter to tell us how large to make
    * the internal cache for storing past gamestates.
    *
    * @param cache_size the size of the internal cache for past gamestates.
    */
    AlphaBeta2PlayersCached(unsigned int cache_size) : cache(cache_size) {};

    /**
    * This method should be given a HexState object in which it is currently
    * the first player's turn. This method will return true if player one can
    * force a win by playing a certain (unspecified) move from this position.
    * Similarly, it will return false if no matter what player one plays,
    * player two can force a win somehow.
    *
    * @param state The state at which to evaluate whether or not player one can
    *              force a win from.
    */
    bool one_wins_one_turn(GameState::HexState<bsize> &state) {
        bool one_wins = false;
        if (this->cache.lookup(state, one_wins)) {
            return one_wins;
        }
        this->nodes_expanded++;


    }

    /**
    * This method should be given a HexState object in which it is currently
    * the second player's turn. This method will return true if, regardless of
    * what player two plays as their move this turn, player one will be able to
    * force a win by playing a certain (unspecified) move afterwards.
    * Similarly, it will return false if there is a move that player two can make
    * such that player two will able to force a win somehow.
    *
    * @param state The state at which to evaluate whether or not player one can
    *              force a win from.
    */
    bool one_wins_two_turn(GameState::HexState<bsize> &state);
};

}

