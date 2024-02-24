#include <cstddef>
#include <iostream>
#include <vector>

#include "Games/Hex/HexState.hpp"
#include "Games/Hex/HexStateBase.hpp"

int main(int argc, char *argv[]) {
    GameState::HexState<8> *initial = new GameState::HexState<8>();
    auto action = GameState::HexAction<8>();
    action.whose = GameState::HexVal::ONE;
    auto next_state = initial->succeed(action);
    std::cout << *next_state << "\n";
    return 0;
}

