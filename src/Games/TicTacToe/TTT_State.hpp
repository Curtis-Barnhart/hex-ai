#ifndef GAMES_TIC_TAC_TOE_TTT_STATE_HPP
#define GAMES_TIC_TAC_TOE_TTT_STATE_HPP

#include <ostream>

#include "../../GameState/GameState.hpp"
#include "TTT_Actions.hpp"

namespace GameState {

    /**
     * TTT_State is a basic implementation of a tic-tac-toe game state.
     * For details on its interface, read GameState::State.
     */
    template<>
    class State<GameState::TA, GameState::TA> {
    public:
        State();

        State(State<GameState::TA, GameState::TA> &&other);

        State(const State &other);

        GameState::State<GameState::TA, GameState::TA> &operator=(const GameState::State<GameState::TA, GameState::TA> &other);

        GameState::State<GameState::TA, GameState::TA> &operator=(GameState::State<GameState::TA, GameState::TA> &&other);

        ~State<GameState::TA, GameState::TA>();

        int whose_turn() const;

        int who_won() const;

        GameState::State<GameState::TA, GameState::TA> *succeed(const GameState::TA &action) const;

        GameState::State<GameState::TA, GameState::TA> *succeed(const GameState::TA &action, GameState::TA &baction) const;

        GameState::State<GameState::TA, GameState::TA> &succeed_in_place(const GameState::TA &action);

        GameState::State<GameState::TA, GameState::TA> &succeed_in_place(const GameState::TA &action, GameState::TA &baction);

        GameState::State<GameState::TA, GameState::TA> *reverse(const GameState::TA &baction) const;

        GameState::State<GameState::TA, GameState::TA> &reverse_in_place(const GameState::TA &baction);

        std::vector<GameState::TA> *get_actions() const;
        
        void get_actions(std::vector<GameState::TA> &buffer) const;

        friend std::ostream &operator<<(std::ostream &out, const GameState::State<TA, TA> &game);

    private:
        signed char turn = -1;
        signed char board[3][3] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    };

    std::ostream &operator<<(std::ostream &out, const GameState::State<GameState::TA, GameState::TA> &game);

    using TTT_State = GameState::State<GameState::TA, GameState::TA>;

}

#endif // !GAMES_TIC_TAC_TOE_TIC_TAC_TOE_HPP

