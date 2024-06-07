add_executable(hello_test hello_test.cpp)

# we want to add a compilation feature to one of our targets (hello_test).
# we add a feature that uses C++20.
target_compile_features(
    hello_test
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    hello_test 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

target_link_libraries(hello_test gtest gtest_main)

