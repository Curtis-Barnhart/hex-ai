#ifndef GAMES_TTT_TTTSTATEBASE_HPP
#define GAMES_TTT_TTTSTATEBASE_HPP

#include "../../GameState/GameState.hpp"
#include <cstring>

namespace GameState {
    /**
    * TTT is an enum for the state that each square in a tic-tac-toe game
    * may be in: empty, occupied by an x, and occupied by an o.
    */
    enum TTT {
        EMPTY, X, O
    };

    /**
     * TTT_Action represents an action taken in a game of tic-tac-toe.
     * It stores an x and y coordinate in the domain [0, 2],
     * as well a TTT enum for whose move it was.
     */
    struct TTT_Action {
        /**
         * @param whose which symbol (X or O) to place
         * @param x x coordinate to place a symbol at
         * @param y y coordinate to place a symbol at
         */
        TTT_Action(TTT whose, char x, char y) : whose(whose), x(x), y(y) {}
        TTT_Action(TTT_Action &&) = default;
        TTT_Action(const TTT_Action &) = default;
        TTT_Action &operator=(TTT_Action &&) = default;
        TTT_Action &operator=(const TTT_Action &) = default;
        ~TTT_Action() = default;
        
        TTT whose = EMPTY;
        char x = 0, y = 0;
    };

    /**
     * GameState::State<TTT_Action> is an abstract class for classes which represent a state of a game
     * of tic-tac-toe.
     * It is aliased to TTT_State_Base.
     */
    template<>
    class State<TTT_Action> {
    public:
        State() = default;

        // TODO: figure out what to do with these later
        State(GameState::State<TTT_Action> &&) = default;
        State(const GameState::State<TTT_Action> &) = default;

        virtual GameState::State<TTT_Action> &operator=(GameState::State<TTT_Action> &&other) {
            this->reset();
            this->turn = other.turn;
            std::memcpy(this->board, other.board, sizeof(other.board));
            return *this;
        };
        virtual GameState::State<TTT_Action> &operator=(const GameState::State<TTT_Action> &other) {
            this->reset();
            this->turn = other.turn;
            std::memcpy(this->board, other.board, sizeof(other.board));
            return *this;
        };
        virtual ~State() = default;

        // methods to determine winning
        virtual int whose_turn() const = 0;

        // methods for move making
        virtual int who_won() const = 0;
        virtual State<TTT_Action> *succeed(const TTT_Action &action) const = 0;
        virtual State<TTT_Action> &succeed_in_place(const TTT_Action &action) = 0;
        virtual std::vector<TTT_Action> *get_actions() const = 0;
        virtual void get_actions(std::vector<TTT_Action> &buffer) const = 0;

    protected:
        int turn = 0;
        TTT board[3][3] = { EMPTY };

    private:
        virtual void reset() = 0;
    };
    
    //using TTT_StateBase = GameState::State<TTT_Action>;
}

#endif // !GAMES_TTT_TTTSTATEBASE_HPP

