# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_subdirectory(AlphaBeta)

add_executable(test_hex_rand_moves test_hex_rand_moves.cpp)
target_include_directories(
    test_hex_rand_moves
    PRIVATE
    ../../extern/cereal/include
)
target_compile_features(
    test_hex_rand_moves
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_hex_rand_moves
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_hex_rand_moves
    gtest
    gtest_main
)
add_test(
    NAME test_hex_rand_moves
    COMMAND test_hex_rand_moves
)

