#
# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
#
# This file is part of hex-ai.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

################################
# main library                 #
################################

add_library(
    hex-ai-core
    GameState/HexState.cpp
    GameSolve/AlphaBeta.cpp
    GameSolve/HexUtil.cpp
    Util/FileIO/file_types.cpp
    Players/util.cpp
)

# we want to add a compilation feature to one of our targets (hex-ai-core).
# we add a feature that uses C++20.
target_compile_features(
    hex-ai-core
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    hex-ai-core 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

################################
# generating games and solving #
################################

add_executable(
    generate_example_games
    generate_example_games.cpp
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

target_link_libraries(
    generate_example_games
    hex-ai-core
)

################################
# for combining game data      #
################################

# add_executable(
#     file_combine
#     file_combine.cpp
#     Util/generated_games_combiner.cpp
#     GameState/HexState.cpp
#     GameSolve/HexUtil.cpp
#     GameState/io.cpp
# )

# # we want to add a compilation feature to one of our targets (main).
# # we add a feature that uses C++20.
# target_compile_features(
#     file_combine
#     PRIVATE
#     cxx_std_20
# )

# # we want to set some compilation options (for how many warnings to show).
# target_compile_options(
#     file_combine 
#     PRIVATE 
#     -Wall 
#     -Wextra 
#     -Wpedantic
# )

################################
# verify gamestate file valid  #
################################

add_executable(
    verify_game_file
    verify_game_file.cpp
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    verify_game_file
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    verify_game_file 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

target_link_libraries(
    verify_game_file
    hex-ai-core
)

################################
# training nerual network      #
################################

# add_executable(
#     train_bottom_model
#     train_bottom_model.cpp
#     GameState/HexState.cpp
#     GameState/io.cpp
# )

# # we want to add a compilation feature to one of our targets (main).
# # we add a feature that uses C++20.
# target_compile_features(
#     train_bottom_model
#     PRIVATE
#     cxx_std_14
# )

# # we want to set some compilation options (for how many warnings to show).
# target_compile_options(
#     train_bottom_model 
#     PRIVATE 
#     -Wall 
#     -Wextra 
#     -Wpedantic
# )

################################
# two player hex               #
################################

add_executable(
    play_two_players
    play_two_players.cpp
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

target_link_libraries(
    play_two_players
    hex-ai-core
)

