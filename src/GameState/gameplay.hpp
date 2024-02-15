#ifndef GAMESTATE_GAMEPLAY_HPP
#define GAMESTATE_GAMEPLAY_HPP

#include <vector>

#include "GameState.hpp"

namespace GameState {

    template<class A>
    int random_play_game(GameState::State<A> *cur_state) {
        GameState::State<A> *next_state;
        std::vector<A> legal_actions;
        
        while (true) {
            // Get next legal moves. If there are none, return the winner.
            cur_state->get_actions(legal_actions);
            if (legal_actions.size() == 0) {
                int winner = cur_state->who_won();
                delete cur_state;
                return winner;
            }

            // Get successor state and free old state
            next_state = cur_state->succeed(legal_actions.at(0));
            delete cur_state;
            cur_state = next_state;
            next_state = nullptr;
        }
    }

}

#endif // !GAMESTATE_GAMEPLAY_HPP

