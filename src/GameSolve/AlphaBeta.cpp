/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "hex-ai/Util/LRUCache.hpp"
#include "hex-ai/GameState/HexState.hpp"
#include <cassert>
#include <vector>

#include "hex-ai/GameSolve/AlphaBeta.hpp"

using AB = GameSolve::AlphaBeta2PlayersCached;
using GameState::HexState;

long AB::get_nodes_hit() const {
    return this->nodes_miss;
}

long AB::get_nodes_hit_total() const {
    return this->nodes_miss_total;
}

AB::AlphaBeta2PlayersCached(unsigned int cache_size) {
    this->cache = new Cache::LRUCache<HexState, bool>(cache_size);
}

AB::~AlphaBeta2PlayersCached() {
    delete this->cache;
}

bool AB::one_wins_one_turn(HexState &state) {
    assert(state.whose_turn() == GameState::HexState::PLAYER_ONE);
    // First check the transposition table to see if we've done this before
    // initialize one_wins to false because that is the default
    // (if no actions we take force a win)
    bool one_wins = false;
    if (this->cache->lookup(state, one_wins)) {
        return one_wins;
    }
    this->nodes_miss++;

    std::vector<HexState::Action> actions;
    HexState::Action backwards;
    state.get_actions(actions);

    if (actions.size() == 0) {
        one_wins = (state.who_won() == HexState::PLAYERS::PLAYER_ONE);
    } else {
        for (const HexState::Action &a : actions) {
            state.succeed_in_place(a, backwards);
            if (this->one_wins_two_turn(state)) {
                one_wins = true;
                state.reverse_in_place(backwards);
                break;
            }
            state.reverse_in_place(backwards);
        }
    }

    this->cache->insert(state, one_wins);
    return one_wins;
}

bool AB::one_wins_two_turn(HexState &state) {
    assert(state.whose_turn() == GameState::HexState::PLAYER_TWO);
    // First check the transposition table to see if we've done this before
    // initialize one_wins to true because that is the default
    // (if no actions we take force a win)
    bool one_wins = true;
    if (this->cache->lookup(state, one_wins)) {
        return one_wins;
    }
    this->nodes_miss++;

    std::vector<HexState::Action> actions;
    HexState::Action backwards;
    state.get_actions(actions);

    if (actions.size() == 0) {
        one_wins = (state.who_won() == HexState::PLAYERS::PLAYER_ONE);
    } else {
        for (const HexState::Action &a : actions) {
            state.succeed_in_place(a, backwards);
            if (!this->one_wins_one_turn(state)) {
                one_wins = false;
                state.reverse_in_place(backwards);
                break;
            }
            state.reverse_in_place(backwards);
        }
    }

    this->cache->insert(state, one_wins);
    return one_wins;
}

