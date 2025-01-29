# Copyright 2025 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

#################################
## main library                 #
#################################
#
#add_library(
#    hex-ai-core
#    GameState/HexState.cpp
#    GameSolve/AlphaBeta.cpp
#    GameSolve/HexUtil.cpp
#    Util/FileIO/file_types.cpp
#    Util/FileIO/GamestateBool_1.cpp
#    Util/FileIO/info_file.cpp
#    Players/util.cpp
#)
#
#target_include_directories(
#    hex-ai-core
#    PUBLIC
#    ../extern/cereal/include/
#)
#
## we want to add a compilation feature to one of our targets (hex-ai-core).
## we add a feature that uses C++20.
#target_compile_features(
#    hex-ai-core
#    PRIVATE
#    cxx_std_20
#)
#
## we want to set some compilation options (for how many warnings to show).
#target_compile_options(
#    hex-ai-core 
#    PRIVATE 
#    -Wall 
#    -Wextra 
#    -Wpedantic
#)

################################
# verify gamestate file valid  #
################################

add_executable(
    info_file
    app/info_file.cpp
)

# We need the cereal headers to be exposed for this guy
target_include_directories(
    info_file
    PRIVATE
    ../extern/cereal/include/
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    info_file
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    info_file 
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
    app/generate_example_games.cpp
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    generate_example_games
    PRIVATE
    cxx_std_20
)

# We need the cereal headers to be exposed for this guy
target_include_directories(
    generate_example_games
    PRIVATE
    ../extern/cereal/include/
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    generate_example_games 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

#################################
## two player hex               #
#################################
#
#add_executable(
#    play_two_players
#    app/play_two_players.cpp
#)
#
## we want to add a compilation feature to one of our targets (main).
## we add a feature that uses C++20.
#target_compile_features(
#    play_two_players
#    PRIVATE
#    cxx_std_20
#)
#
## we want to set some compilation options (for how many warnings to show).
#target_compile_options(
#    play_two_players 
#    PRIVATE 
#    -Wall 
#    -Wextra 
#    -Wpedantic
#)
#
#target_link_libraries(
#    play_two_players
#    hex-ai-core
#)
#
#################################
## combine/split data files     #
#################################
#
#add_executable(
#    file_combine
#    app/file_combine.cpp
#)
#
## we want to add a compilation feature to one of our targets (main).
## we add a feature that uses C++20.
#target_compile_features(
#    file_combine
#    PRIVATE
#    cxx_std_20
#)
#
## we want to set some compilation options (for how many warnings to show).
#target_compile_options(
#    file_combine 
#    PRIVATE 
#    -Wall 
#    -Wextra 
#    -Wpedantic
#)
#
#target_link_libraries(
#    file_combine
#    hex-ai-core
#)

################################
# output data file to text     #
################################

add_executable(
    hexstate_to_string
    app/hexstate_to_string.cpp
)

# We need the cereal headers to be exposed for this guy
target_include_directories(
    hexstate_to_string
    PRIVATE
    ../extern/cereal/include/
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    hexstate_to_string
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    hexstate_to_string 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

