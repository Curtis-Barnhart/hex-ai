/*
 * So this hasn't really worked out and I may try to make it to.
 * I'm not sure how to effectively combine transposition tables
 * with alpha-beta pruning.
 */


#include "LRUCache.hpp"
#include "../Games/Hex/HexState.hpp"

namespace GameSolve {

/**
*
*/
struct AlphaBeta2PlayersCached {
private:
    /**
    *
    */
    struct ValueRecord {
        enum cmp {LEQ, EQ, GEQ};
        cmp op;
        double value;
    };

    // tracks how many nodes have been calculated
    long nodes_hit = 0, nodes_hit_total = 0;
    // tracks how many terminal nodes have been calculated
    long terms_hit = 0, terms_hit_total = 0;
    // internal cache of nodes
    Cache::LRUCache<GameState::HexState, ValueRecord> *cache = nullptr;

public:
    long get_nodes_hit() const { return this->nodes_hit; }
    long get_nodes_hit_total() const { return this->nodes_hit_total; }
    long get_terms_hit() const { return this->terms_hit; }
    long get_terms_hit_total() const { return this->terms_hit_total; }

    AlphaBeta2PlayersCached(unsigned int cache_size) {
        this->cache = new Cache::LRUCache<GameState::HexState, std::array<double, 2>>(cache_size);
    }

    ~AlphaBeta2PlayersCached() {
        delete this->cache;
    }

private:
    void ab_recursive(
        GameState::HexState &state,
        double alpha,
        double beta
    ) {
        // we are expanding a new node; increment nodes_hit
        this->nodes_hit++;
        
        // best_score will record the best that a player 
        double best_score;
        if (this->cache->lookup(state, best_score)) {

        }
    }
};

}

