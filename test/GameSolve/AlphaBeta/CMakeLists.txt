# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(test_AlphaBeta_1 test_AlphaBeta_1.cpp)
target_include_directories(
    test_AlphaBeta_1
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_AlphaBeta_1
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_AlphaBeta_1
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_AlphaBeta_1
    gtest
    gtest_main
)
add_test(
    NAME test_AlphaBeta_1
    COMMAND test_AlphaBeta_1
)

