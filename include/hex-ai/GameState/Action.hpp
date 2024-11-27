#ifndef HEX_AI_GAMESTATE_ACTION_HPP
#define HEX_AI_GAMESTATE_ACTION_HPP

#include "hex-ai/GameState/enums.hpp"

namespace GameState {

struct Action {
    unsigned char x = 0, y = 0;
    GameState::PLAYERS whose = GameState::PLAYERS::PLAYER_NONE;

    /**
     * The default constructor creates an action to claim the (0, 0) tile
     * by PLAYER_NONE.
     */
    Action() = default;

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
    Action(unsigned char x, unsigned char y, GameState::PLAYERS whose):
        x(x), y(y), whose(whose) {}

    /**
     * Serializes an Action instance to a cereal archive
     * using the cereal library.
     *
     * @param archive the cereal archive to serialize the Action to.
     */
    template<class Archive>
    void serialize(Archive &archive) {
        archive(x, y, whose);
    }
};

}

#endif // !HEX_AI_GAMESTATE_ACTION_HPP

