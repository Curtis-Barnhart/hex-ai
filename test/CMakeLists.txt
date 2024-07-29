add_executable(
    test_HexState 
    test_HexState.cpp
    "../src/GameState/HexState.cpp"
    "../src/GameSolve/HexUtil.cpp"
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

target_link_libraries(test_HexState gtest gtest_main)
add_test(
    NAME test_HexState
    COMMAND test_HexState
)


add_executable(
    test_HexStateIO
    test_HexStateIO.cpp
    "../src/GameState/HexState.cpp"
    "../src/GameSolve/HexUtil.cpp"
    "../src/GameState/io.cpp"
)

# we want to add a compilation feature to one of our targets (test_HexStateIO).
# we add a feature that uses C++20.
target_compile_features(
    test_HexStateIO
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    test_HexStateIO 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

target_link_libraries(test_HexStateIO gtest gtest_main)
add_test(
    NAME test_HexStateIO
    COMMAND test_HexStateIO
)

