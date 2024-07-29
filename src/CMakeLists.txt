################################
# generating games and solving #
################################

add_executable(
    generate_example_games
    generate_example_games.cpp
    GameState/HexState.cpp
    GameSolve/AlphaBeta.cpp
    GameSolve/HexUtil.cpp
    GameState/io.cpp
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    generate_example_games
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    generate_example_games 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

################################
# training nerual network      #
################################

add_executable(
    train_bottom_model
    train_bottom_model.cpp
    GameState/HexState.cpp
    GameState/io.cpp
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    train_bottom_model
    PRIVATE
    cxx_std_14
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    train_bottom_model 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

################################
# two player hex               #
################################

add_executable(
    play_two_players
    play_two_players.cpp
    GameState/HexState.cpp
    Players/util.cpp
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    play_two_players
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    play_two_players 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

