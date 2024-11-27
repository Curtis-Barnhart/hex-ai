# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(test_HexState_Constructor test_HexState.cpp)
target_include_directories(
    test_HexState_Constructor
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_HexState_Constructor
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_HexState_Constructor
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_HexState_Constructor
    gtest
    gtest_main
)
add_test(
    NAME test_HexState_Constructor
    COMMAND test_HexState_Constructor
)

