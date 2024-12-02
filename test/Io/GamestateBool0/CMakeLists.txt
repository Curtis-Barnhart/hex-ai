# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(test_GamestateBool0 test_GamestateBool0.cpp)
target_include_directories(
    test_GamestateBool0
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_GamestateBool0
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_GamestateBool0
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_GamestateBool0
    gtest
    gtest_main
)
add_test(
    NAME test_GamestateBool0
    COMMAND test_GamestateBool0
)

