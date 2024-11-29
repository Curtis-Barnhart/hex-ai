#ifndef HEX_AI_GAMESTATE_ACTION_HPP
#define HEX_AI_GAMESTATE_ACTION_HPP

#include "hex-ai/GameState/enums.hpp"

namespace GameState {

struct Action {
    unsigned char x, y;
    GameState::PLAYERS whose;

    bool operator==(const Action &other) const = default;

    /**
    * Constructor sets memeber values according to the given parameters.
    * NO ERROR CHECKING is performed on x and y to make sure they are
    * within an allowable range [0, BOARD_SIZE).
    *
    * @param x the x coordinate of what tile should be claimed.
    * @param y the y coordinate of what tile should be claimed.
    * @param whose 0 for the first player, 1 for the second, -1 for no player.
     */
    Action(
        unsigned char x = 0,
        unsigned char y = 0,
        GameState::PLAYERS whose = PLAYER_NONE
    ): x(x), y(y), whose(whose) {}
};

}

#endif // !HEX_AI_GAMESTATE_ACTION_HPP

