#ifndef GAMESTATE_GAMESTATE_HPP
#define GAMESTATE_GAMESTATE_HPP

#include <vector>

namespace GameState {
    template<class A>
    class State {
    public:
        State() = default;
        State(State &&) = default;
        State(const State &) = default;
        State &operator=(State &&) = default;
        State &operator=(const State &) = default;
        ~State() = default;
        
        // methods to determine winning
        int whose_turn() const;

        // methods for move making
        int who_won() const;
        State<A> *succeed(const A &action) const;
        std::vector<A> *get_actions() const;
        void get_actions(std::vector<A> &buffer) const;
    };
}

#endif // !GAMESTATE_HPP

