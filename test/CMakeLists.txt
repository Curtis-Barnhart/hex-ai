add_executable(test_hello test_hello.cpp)

# we want to add a compilation feature to one of our targets (test_hello).
# we add a feature that uses C++20.
target_compile_features(
    test_hello
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    test_hello 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

target_link_libraries(test_hello gtest gtest_main)

add_executable(
test_HexState 
test_HexState.cpp
"../src/GameState/HexState.cpp"
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

