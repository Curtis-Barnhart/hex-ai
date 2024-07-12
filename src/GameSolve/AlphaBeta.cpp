#include "hex-ai/Util/LRUCache.hpp"
#include "hex-ai/GameState/HexState.hpp"

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
    // First check the transposition table to see if we've done this before
    // initialize one_wins to false because that is the default
    // (if no actions we take force a win)
    bool one_wins = false;
    if (this->cache->lookup(state, one_wins)) {
        return one_wins;
    }
    this->nodes_miss++;

    HexState::HexActReader act_iter(state);
    if (act_iter) {
        HexState::Action backwards;
        while (act_iter) {
            state.succeed_in_place(act_iter.get_action(), backwards);
            if (this->one_wins_two_turn(state)) {
                one_wins = true;
                state.reverse_in_place(backwards);
                break;
            }
            state.reverse_in_place(backwards);
            act_iter++;
        }
    } else {
        one_wins = (state.who_won() == HexState::PLAYERS::PLAYER_ONE);
    }

    this->cache->insert(state, one_wins);
    return one_wins;
}

bool AB::one_wins_two_turn(HexState &state) {
    // First check the transposition table to see if we've done this before
    // initialize one_wins to true because that is the default
    // (if no actions we take force a win)
    bool one_wins = true;
    if (this->cache->lookup(state, one_wins)) {
        return one_wins;
    }
    this->nodes_miss++;

    HexState::HexActReader act_iter(state);
    if (act_iter) {
        HexState::Action backwards;
        while (act_iter) {
            state.succeed_in_place(act_iter.get_action(), backwards);
            if (!this->one_wins_one_turn(state)) {
                one_wins = false;
                state.reverse_in_place(backwards);
                break;
            }
            state.reverse_in_place(backwards);
            act_iter++;
        }
    } else {
        one_wins = (state.who_won() == HexState::PLAYERS::PLAYER_ONE);
    }

    this->cache->insert(state, one_wins);
    return one_wins;
}

