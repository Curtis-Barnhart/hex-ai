/*
 * So this hasn't really worked out and I may try to make it to.
 * I'm not sure how to effectively combine transposition tables
 * with alpha-beta pruning.
 */

#include "hex-ai/GameSolve/LRUCache.hpp"
#include "hex-ai/GameState/HexState.hpp"

namespace GameSolve {

/**
*
*/
struct AlphaBeta2PlayersCached {
public:
    // tracks how many nodes have been calculated
    long nodes_miss = 0, nodes_miss_total = 0;
    // internal cache of nodes
    Cache::LRUCache<GameState::HexState, bool> *cache = nullptr;

public:
    long get_nodes_hit() const { return this->nodes_miss; }
    long get_nodes_hit_total() const { return this->nodes_miss_total; }

    AlphaBeta2PlayersCached(unsigned int cache_size) {
        this->cache = new Cache::LRUCache<GameState::HexState, bool>(cache_size);
    }

    ~AlphaBeta2PlayersCached() {
        delete this->cache;
    }

    bool one_wins_one_turn(GameState::HexState &state) {
        // First check the transposition table to see if we've done this before
        // initialize one_wins to false because that is the default
        // (if no actions we take force a win)
        bool one_wins = false;
        if (this->cache->lookup(state, one_wins)) {
            return one_wins;
        }
        this->nodes_miss++;

        GameState::HexState::HexActReader act_iter(state);
        if (act_iter) {
            GameState::HexState::Action backwards;
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
            one_wins = (state.who_won() == GameState::HexState::PLAYERS::PLAYER_ONE);
        }

        this->cache->insert(state, one_wins);
        return one_wins;
    }

    bool one_wins_two_turn(GameState::HexState &state) {
        // First check the transposition table to see if we've done this before
        // initialize one_wins to true because that is the default
        // (if no actions we take force a win)
        bool one_wins = true;
        if (this->cache->lookup(state, one_wins)) {
            return one_wins;
        }
        this->nodes_miss++;

        GameState::HexState::HexActReader act_iter(state);
        if (act_iter) {
            GameState::HexState::Action backwards;
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
            one_wins = (state.who_won() == GameState::HexState::PLAYERS::PLAYER_ONE);
        }

        this->cache->insert(state, one_wins);
        return one_wins;
    }
};

}

