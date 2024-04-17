cmake_minimum_required(VERSION 3.27)

project(
    cpp-ai
    VERSION 1.0
    DESCRIPTION "Hex ai for fun in C++"
    LANGUAGES CXX
)

add_executable(
    main
    src/main.cpp
    src/Games/Hex/HexState.hpp
    src/GameSolve/Maximin.hpp
    src/GameSolve/LRUCache.hpp
)

target_compile_features(
    main
    PRIVATE
    cxx_std_17
)

target_compile_options(
    main 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

