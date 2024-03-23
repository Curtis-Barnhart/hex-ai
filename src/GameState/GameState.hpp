#ifndef GAMESTATE_GAMESTATE_HPP
#define GAMESTATE_GAMESTATE_HPP

#include <functional>
#include <iostream>
#include <ostream>
#include <vector>

namespace GameState {
    // Forward declaration of ostream << operator
    // requires forward declaration of State
    template<class Action, class Back, unsigned int Players>
    class State;
}

// Friending of templated non member operator << in GameState::State
// requires forward declaration of that operator
template<class Action, class Back, unsigned int Players>
std::ostream &operator<<(std::ostream &out, const GameState::State<Action, Back, Players> &state);

// Friending of templated non member struct std::hash in GameState::State
// requires forward declaration of that operator
template<class Action, class Back, unsigned int Players>
struct std::hash<GameState::State<Action, Back, Players>>;

namespace GameState {
    template<class Action, class Back, unsigned int Players>
    class State {
        // This operator is overloaded so that a game's state may be easily printed.
        friend std::ostream &operator<<<Action, Back, Players>(std::ostream &out, const GameState::State<Action, Back, Players> &game);

        // This struct is overloaded so that hash may be done on any gamestate
        friend struct std::hash<GameState::State<Action, Back, Players>>;

    public:
        /**
        * The constructor returns an object that represents the initial state of
        * a game, before any players have made a move.
        */
        State() = default;

        /**
        * Constructs a new state that represents the same state as the old one.
        * @param other the state which you are copying to make this one
        */
        State(State &&other) = default;

        /**
        * Constructs a new state that represents the same state as the old one.
        * @param other the state which you are copying to make this one
        */
        State(const State &other) = default;

        /**
        * Changes the state that is being set equal so that it represents the
        * same state as the state which it is being set equal to.
        * @param other the state which you are copying into this one
        * @return a reference to the state which you have just set
        */
        State<Action, Back, Players> &operator=(State<Action, Back, Players> &&other);

        /**
        * Changes the state that is being set equal so that it represents the
        * same state as the state which it is being set equal to.
        * @param other the state which you are copying into this one
        * @return a reference to the state which you have just set
        */
        State<Action, Back, Players> &operator=(const State<Action, Back, Players> &other);

        /**
        * Destructor for a state.
        */
        ~State() = default;
        
        /*
        * @return 0 if the first player is the next player to make a move in the
        * current state, 1 if the second player is the next player to make a move,
        * etc.
        */
        unsigned int whose_turn() const;

        /**
        * @return 0 if the first player has won the game, 1 if the second player
        * has won the game, etc. (returns -1 if no player has won)
        */
        int who_won() const;

        /**
         * Returns the current score of the game.
         * In a game where there is no "score" and only a win or loss at the end,
         * this method should return 0s if the game is not over,
         * and can use 1 to indicate victory and 0 defeat.
         * @param score an array that can be filled with doubles,
         *              indicating the score of each player in the current state.
         */
        void current_score(double score[Players]) const;

        /**
        * succeed takes an action and applies it to a state of a game, returning
        * the new state that occurs as a result of that action.
        * @param action the action to simulate
        * @return a pointer to a new state that represents the state of a game
        * after the action has been applied.
        */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        State<Action, Back, Players> *succeed(const Action &action) const;

        /**
         * succeed takes an action and applies it to the state of a game, returning
         * the new state that occurs as a result of that action.
         * It places a backwards action into baction, which can be applied to the
         * succeeding state to get back the original state.
         * @param action the action to simulate
         * @param baction the object into which succeed will record the backwards
         * action to return from the succeeding state to the original state
         * @return a pointer to a new state that represents the state of a game
         * after the action has been applied.
         */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        State<Action, Back, Players> *succeed(const Action &action, Back &baction) const;

        /**
        * succeed_in_place takes an action and applies it to a state of a game,
        * mutating the current state to represent the game after that action
        * has been applied
        * @param action the action to simulate
        * @return a reference to the state on which succeed_in_place was called,
        * freshly mutated by the new action
        */
        State<Action, Back, Players> &succeed_in_place(const Action &action);

        /**
        * succeed_in_place takes an action and applies it to a state of a game,
        * mutating the current state to represent the game after that action
        * has been applied.
        * It places a backwards action into baction, which can be applied to the
        * succeeding state to get back the original state.
        * @param action the action to simulate
        * @param baction the object into which succeed_in_place will record the
        * backwards action to return from the succeeding state to the original
        * state
        * @return a reference to the state on which succeed_in_place was called,
        * freshly mutated by the new action
        */
        State<Action, Back, Players> &succeed_in_place(const Action &action, Back &baction);

        /**
        * reverse takes a backwards action and applies it to the state of a game,
        * returning the "new" state from which the current state would have come
        * given that the forwards version of the backwards action was what was
        * applied to a state to get to the current state.
        * @param baction the backwards action to apply to the current state
        * @return a pointer to a state representing the state preceding the current
        * state
        */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        State<Action, Back, Players> *reverse(const Back &baction) const;

        /**
        * reverse_in_place takes a backwards action and applies it ot the state of
        * a game, mutating the current state to the "new" state from which the
        * current state would have come given that the forwards version of the
        * backwards action was what was applied to a state to get to the current
        * state.
        * @param baction the backwards action to apply to the current state
        * @return a reference to the state on which reverse_in_place was called,
        * freshly mutated according to the backwards action
        */
        State<Action, Back, Players> &reverse_in_place(const Back &baction);

        /**
        * get_actions returns a pointer to a vector of all legal actions from a
        * current state of the game.
        * @return a vector of legal actions from the current state.
        */
        [[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
        std::vector<Action> *get_actions();

        /**
        * get_actions fills a vector with all the legal actions from a current
        * state of the game.
        * get_actions will only append elements to the vector and does not
        * check if it is empty first.
        * @param buffer the vector to fill with legal actions.
        */
        void get_actions(std::vector<Action> &buffer) const;
    };
}

#endif // !GAMESTATE_HPP

