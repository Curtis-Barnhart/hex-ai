#include "hex-ai/Players/util.hpp"
#include <iostream>

using Action = GameState::HexState::Action;

void Players::get_action(Action &a) {
    int t;
    std::cout << "Enter x coordinate: ";
    std::cin >> t;
    a.x = t;
    std::cout << "Enter y coordinate: ";
    std::cin >> t;
    a.y = t;
}

