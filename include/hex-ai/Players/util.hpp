#ifndef PLAYERS_UTIL_HPP
#define PLAYERS_UTIL_HPP

#include "hex-ai/GameState/HexState.hpp"

using Action = GameState::HexState::Action;

namespace Players {

void get_action(Action &a);

}

#endif // !PLAYERS_UTIL_HPP
//
