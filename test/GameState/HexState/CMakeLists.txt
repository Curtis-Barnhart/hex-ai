# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(test_HexState_Constructor test_Constructor.cpp)
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

add_executable(test_HexState_get_actions test_get_actions.cpp)
target_include_directories(
    test_HexState_get_actions
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_HexState_get_actions
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_HexState_get_actions
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_HexState_get_actions
    gtest
    gtest_main
)
add_test(
    NAME test_HexState_get_actions
    COMMAND test_HexState_get_actions
)

add_executable(test_HexState_action_iterator test_action_iterator.cpp)
target_include_directories(
    test_HexState_action_iterator
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_HexState_action_iterator
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_HexState_action_iterator
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_HexState_action_iterator
    gtest
    gtest_main
)
add_test(
    NAME test_HexState_action_iterator
    COMMAND test_HexState_action_iterator
)

add_executable(test_HexState_who_won test_who_won.cpp)
target_include_directories(
    test_HexState_who_won
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_HexState_who_won
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_HexState_who_won
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_HexState_who_won
    gtest
    gtest_main
)
add_test(
    NAME test_HexState_who_won
    COMMAND test_HexState_who_won
)

add_executable(test_HexState_succeed test_make_moves.cpp)
target_include_directories(
    test_HexState_succeed
    PRIVATE
    ../../../extern/cereal/include
)
target_compile_features(
    test_HexState_succeed
    PRIVATE
    cxx_std_20
)
target_compile_options(
    test_HexState_succeed
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)
target_link_libraries(
    test_HexState_succeed
    gtest
    gtest_main
)
add_test(
    NAME test_HexState_succeed
    COMMAND test_HexState_succeed
)
