# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(test_enum_values "test_enums.cpp")
target_compile_features(
    test_enum_values
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_enum_values
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_enum_values
    gtest
    gtest_main
)
add_test(
    NAME test_enum_values
    COMMAND test_enum_values
)

