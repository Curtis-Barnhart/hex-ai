/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "hex-ai/Players/util.hpp"
#include <iostream>
#include <limits>

using Action = GameState::HexState::Action;

void Players::get_action(Action &a) {
    int t;

    // every darn time the user enters the wrong number, we correct them
    for (;;) { 

        std::cout << "Enter x coordinate: ";

        // store user input in temp variable
        std::cin >> t;

        if (std::cin.fail()) {

            // notify user
            std::cout << "Input should be an integer between 0 and 10 \n";

            // now remove `failbit` (error flag) to reset stream state
            std::cin.clear(); 

            // ignore entered characters (up to first of max len of streamsize || '\n')
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        } else if (t < 0 || t > 10) {

            // stream in good state since number provided so no need to reset state or ignore prev input
            std::cout << "Input should be a number between 0 and 10.\n";
        } else {
            a.x = t; 
            break;
        }
    }

        // again for y-coordinate
    for (;;) {
        std::cout << "Enter y coordinate: ";
        std::cin >> t;
        if (std::cin.fail()) {
            std::cout << "Input should be an integer between 0 and 10\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (t < 0 || t > 10) {
            std::cout << "Input should be a number between 0 and 10.\n";
        } else a.y = t; break;
    }
}

