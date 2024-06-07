cmake_minimum_required(VERSION 3.26)

# set the project description and name and whatnot
project(
    cpp-ai
    DESCRIPTION "Hex ai for fun in C++"
    LANGUAGES CXX
)

# This is where tiny-dnn lives
add_subdirectory("extern/tiny-dnn")
# And also do an include_directories so we can reference it with absolute path
include_directories("${tiny_dnn_SOURCE_DIR}")

# # we want to make an executable: define its name and deps here
# add_executable(
#     main
#     src/main.cpp
#     src/GameState/HexState.hpp
#     src/GameSolve/LRUCache.hpp
# )

# we want to set a sort of "root directory" so that all our includes
# don't have to be goofy relative path messes
# by using "include_directories" we can set a directory to include everything from
include_directories(src)

# this is the subdirectory of our main executable
add_subdirectory(src)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    main
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    main 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

# include(FetchContent)
# FetchContent_Declare(
#     googletest
#     URL https://github.com/google/googletest/archive/305e5a238b3c8d11266fbafd85520fb6b3184851.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# This next section is to set up gtest I don't understand it help
# enable_testing()
# add_executable(
#     hello_test
#     test/hello_test.cpp
# )
# target_link_libraries(
#     hello_test
#     GTest::gtest_main
# )
# include(GoogleTest)
# gtest_discover_tests(hello_test)

option(CPP_AI_TESTS "Build tests for cpp-ai" OFF)
message(STATUS "CPP_AI_TESTS set to ${CPP_AI_TESTS}")
if (CPP_AI_TESTS)
    # compile targets for googletest
    add_subdirectory("extern/googletest")
    include_directories("${gtest_SOURCE_DIR}")
    message(STATUS "including from ${gtest_SOURCE_DIR}")
    add_subdirectory("test")
endif()

