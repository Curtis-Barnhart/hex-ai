# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(test_Action test_action.cpp)
target_include_directories(
    test_Action
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_Action
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_Action
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_Action
    gtest
    gtest_main
)
add_test(
    NAME test_Action
    COMMAND test_Action
)

