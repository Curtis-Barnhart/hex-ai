# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(
    test_HexState 
    test_HexState.cpp
)

target_include_directories(
    test_HexState
    PUBLIC
    "../extern/cereal/include"
)

# we want to add a compilation feature to one of our targets (test_HexState).
# we add a feature that uses C++20.
target_compile_features(
    test_HexState
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    test_HexState 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

target_link_libraries(
    test_HexState
    gtest
    gtest_main
)
add_test(
    NAME test_HexState
    COMMAND test_HexState
)


