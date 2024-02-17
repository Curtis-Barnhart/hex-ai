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
        virtual State<A> &operator=(State<A> &&) = 0;
        virtual State<A> &operator=(const State<A> &) = 0;
        virtual ~State() = default;
        
        // methods to determine winning
        virtual int whose_turn() const = 0;

        // methods for move making
        virtual int who_won() const = 0;
        virtual State<A> *succeed(const A &action) const = 0;
        virtual State<A> &succeed_in_place(const A &action) = 0;
        virtual std::vector<A> *get_actions() const = 0;
        virtual void get_actions(std::vector<A> &buffer) const = 0;
    };
}

#endif // !GAMESTATE_HPP

