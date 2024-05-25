cmake_minimum_required(VERSION 3.26)

# set the project description and name and whatnot
project(
    cpp-ai
    DESCRIPTION "Hex ai for fun in C++"
    LANGUAGES CXX
)

# we want to make an executable: define its name and deps here
add_executable(
    main
    src/main.cpp
    src/GameState/HexState.hpp
    src/GameSolve/LRUCache.hpp
)

# we want to set a sort of "root directory" so that all our includes
# don't have to be goofy relative path messes
# by using "include_directories" we can set a directory to include everything from
include_directories(
    src
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++17.
target_compile_features(
    main
    PRIVATE
    cxx_std_17
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    main 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

