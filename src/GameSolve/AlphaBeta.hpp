#include "LRUCache.hpp"

namespace GameSolve {

template<class State, class Action, class Back>
struct AlphaBetaCached {
    // tracks how many nodes have been calculated
    long nodes_hit = 0, nodes_hit_total = 0;
    // tracks how many terminal nodes have been calculated
    long terms_hit = 0, terms_hit_total = 0;
    // internal cache of nodes
    Cache::LRUCache<State, std::array<double, 2>> *cache = nullptr;

    AlphaBetaCached(unsigned int cache_size) {
        this->cache = new Cache::LRUCache<State, std::array<double, 2>>(cache_size);
    }

    ~AlphaBetaCached() {
        delete this->cache;
    }
    
    
};

}

