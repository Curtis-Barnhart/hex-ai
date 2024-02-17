#include "TTT_State.hpp"

int GameState::TTT_State::whose_turn() const {
    return this->turn;
}

int GameState::TTT_State::who_won() const {
            // iterate over rows and columns at the same time
            for (size_t i = 0; i < 3; i++) {
                if (this->board[i][0] != EMPTY
                    && this->board[i][0] == this->board[i][1]
                    && this->board[i][1] == this->board[i][2]) {
                    return this->board[i][0] == X ? 0 : 1;
                }
                if (this->board[0][i] != EMPTY
                    && this->board[0][i] == this->board[1][i]
                    && this->board[1][i] == this->board[2][i]) {
                    return this->board[i][0] == X ? 0 : 1;
                }
            }
            // check both diagonals
            TTT mid = this->board[1][1];
            if (mid != EMPTY) {
                if ((this->board[0][0] == mid && mid == this->board[2][2])
                    || (this->board[0][2] == mid && mid == this->board[2][0])) {
                    return mid == X ? 0 : 1;
                }
            }
            return -1;
        }

GameState::TTT_State *GameState::TTT_State::succeed(const TTT_Action &action) const {
    TTT_State *next_state = new TTT_State(*this);
    next_state->succeed(action);
    return next_state;
}

GameState::TTT_State &GameState::TTT_State::succeed_in_place(const GameState::TTT_Action &action) {
    this->board[action.x][action.y] = action.whose;
    return *this;
}

std::vector<GameState::TTT_Action> *GameState::TTT_State::get_actions() const {
    std::vector<GameState::TTT_Action> *a = new std::vector<GameState::TTT_Action>();
    this->get_actions(*a);    
    return a;
}

void GameState::TTT_State::get_actions(std::vector<GameState::TTT_Action> &buffer) const {
    GameState::TTT whose_turn = this->turn == 0 ? GameState::X : GameState::O;
    buffer.reserve(9);
    for (std::size_t x = 0; x < 3; x++) {
        for (std::size_t y = 0; y < 3; y++) {
            if (this->board[x][y] == GameState::EMPTY) {
                buffer.emplace_back(whose_turn , x, y);
            }
        }
    }
}

