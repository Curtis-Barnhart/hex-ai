#include <array>
#include <cstring>
#include <limits>
#include <vector>

#include "../GameState/GameState.hpp"
#include "LRUCache.hpp"

namespace GameSolve {
/**
 * Maximin is a struct which defines several methods for running the maximin
 * algorithm on a GameState.
 * It also records how many nodes and leaves were expanded in the state space
 * during a particular search so that performance may be measured afterwards.
 */
template<class Action, class Back, unsigned int Players>
struct Maximin {
    Maximin() = default;
    ~Maximin() = default;

    long nodes_hit = 0;
    long terms_hit = 0;

    /**
     * maximin_no_prune_recursive tells what the best possible score a player can
     * achieve from a given state is assuming that all agents act rationally.
     * @param state the state from which the agent is to act to maximize their
     *        minimum score.
     * @param best_score an array of doubles which represents the score at the
     *        state which is the best score the player at the current state can
     *        force assuming all agents act ratoinally.
     */
    void maximin_no_prune_recursive(
        GameState::State<Action, Back, Players> &state,
        double best_score[Players]
    ) {
        this->nodes_hit++;
        double child_score[Players];

        // if this node is terminal, then its optimal score is just its score
        std::vector<Action> actions;
        state.get_actions(actions);
        if (actions.empty()) {
            this->terms_hit++;
            state.current_score(best_score);
            return;
        }

        // Look through all the children of this state -
        // choose whichever has the highest score for you as your highest score
        unsigned int player = state.whose_turn();
        best_score[player] = -std::numeric_limits<double>::infinity();

        for(Action &a: actions) {
            Back back;
            state.succeed_in_place(a, back);
            this->maximin_no_prune_recursive(state, child_score);
            state.reverse_in_place(back);

            if (child_score[player] > best_score[player]) {
                std::memcpy(best_score, child_score, sizeof(double) * Players);
            }
        }
    }

    /**
     * maximin_no_prune is a wrapper around maximin_no_prune_recursive
     * as the latter does not report which move is optimal, but simply what the
     * best score would be with an optimal move.
     * maximin_no_prune returns the action that is optimal to take from the
     * current state.
     * @param state the state at which you wish to find the optimal move
     * @return a pointer to an 
     */
    Action *maximin_no_prune(GameState::State<Action, Back, Players> &state) {
        this->nodes_hit = 0;
        this->terms_hit = 0;
        double best_score[Players];
        double child_score[Players];
        this->nodes_hit++;

        // if this node is terminal, then there's no action to return
        std::vector<Action> actions;
        state.get_actions(actions);
        if (actions.empty()) {
            this->terms_hit++;
            return nullptr;
        }

        // look through all children of this state,
        // and choose the action which results in a state with highest score
        unsigned int player = state.whose_turn();
        best_score[player] = -std::numeric_limits<double>::infinity();
        Action *best_action = new Action;

        for(Action &a: actions) {
            Back back;
            state.succeed_in_place(a, back);
            this->maximin_no_prune_recursive(state, child_score);
            state.reverse_in_place(back);
            
            if (child_score[player] > best_score[player]) {
                std::memcpy(best_score, child_score, sizeof(double) * Players);
                *best_action = a;
            }
        }

        return best_action;
    }
};

/**
 * MaximinCached is similar to Maximin in that it defines methods for running
 * the maximin algorithm on GameStates,
 * but it utilizes fine-tunable caching so that GameStates which are reachable
 * from multiple parents don't have to be expanded multiple times.
 */
template<class Action, class Back, unsigned int Players>
struct MaximinCached {

    /**
     * Creates a new instance of MaximinCached object.
     * Constructor initializes an empty cache.
     * @param cache_size size of the cache to use for this Maximin algorithm runner.
     */
    MaximinCached(unsigned int cache_size) {
        this->cache = new Cache::LRUCache<GameState::State<Action, Back, Players>, std::array<double, Players>>(cache_size);
    }

    /**
     * Destructor deletes the internal cache.
     */
    ~MaximinCached() {
        delete this->cache;
    }

    // tracks how many nodes have been calculated
    long nodes_hit = 0;
    // tracks how many terminal nodes have been calculated
    long terms_hit = 0;
    // internal cache of nodes
    Cache::LRUCache<GameState::State<Action, Back, Players>, std::array<double, Players>> *cache = nullptr;

    /**
     * maximin_no_prune_recursive tells what the best possible score a player can
     * achieve from a given state is assuming that all agents act rationally.
     * @param state the state from which the agent is to act to maximize their
     *        minimum score.
     * @param best_score an array of doubles which represents the score at the
     *        state which is the best score the player at the current state can
     *        force assuming all agents act ratoinally.
     */
    void maximin_no_prune_recursive(
        GameState::State<Action, Back, Players> &state,
        std::array<double, Players> &best_score
    ) {
        this->nodes_hit++;

        // check for the node in the cache
        if (this->cache->lookup(state, best_score)) {
            return;
        }

        // if this node is terminal, then its optimal score is just its score
        std::vector<Action> actions;
        state.get_actions(actions);
        if (actions.empty()) {
            this->terms_hit++;
            state.current_score(best_score.data());
            return;
        }

        std::array<double, Players> child_score;

        // Look through all the children of this state -
        // choose whichever has the highest score for you as your highest score
        unsigned int player = state.whose_turn();
        best_score[player] = -std::numeric_limits<double>::infinity();

        for(Action &a: actions) {
            Back back;
            state.succeed_in_place(a, back);
            this->maximin_no_prune_recursive(state, child_score);
            state.reverse_in_place(back);

            if (child_score[player] > best_score[player]) {
                best_score = child_score;
            }
        }

        // enter the calculated node into the cache so we can hopefully catch it early next time
        this->cache->insert(state, best_score);
    }

    /**
     * maximin_no_prune is a wrapper around maximin_no_prune_recursive
     * as the latter does not report which move is optimal, but simply what the
     * best score would be with an optimal move.
     * maximin_no_prune returns the action that is optimal to take from the
     * current state.
     * @param state the state at which you wish to find the optimal move
     * @return a pointer to an 
     */
    Action *maximin_no_prune(
        GameState::State<Action, Back, Players> &state
    ) {
        this->nodes_hit = 0;
        this->terms_hit = 0;
        std::array<double, Players> best_score;
        std::array<double, Players> child_score;
        this->nodes_hit++;

        // if this node is terminal, then there's no action to return
        std::vector<Action> actions;
        state.get_actions(actions);
        if (actions.empty()) {
            this->terms_hit++;
            return nullptr;
        }

        // look through all children of this state,
        // and choose the action which results in a state with highest score
        unsigned int player = state.whose_turn();
        best_score[player] = -std::numeric_limits<double>::infinity();
        Action *best_action = new Action;

        for(Action &a: actions) {
            Back back;
            state.succeed_in_place(a, back);
            this->maximin_no_prune_recursive(state, child_score);
            state.reverse_in_place(back);
            
            if (child_score[player] > best_score[player]) {
                best_score = child_score;
                *best_action = a;
            }
        }

        return best_action;
    }
};
}

