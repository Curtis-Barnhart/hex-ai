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

option(CPP_AI_TESTS "Build tests for cpp-ai" OFF)
message(STATUS "CPP_AI_TESTS set to ${CPP_AI_TESTS}")
if (CPP_AI_TESTS)
    # compile targets for googletest
    add_subdirectory("extern/googletest")
    # For some reason the following line is not necessary for headers to work properly
    # I don't understand why yet.
    # What _is_ necessary that I didn't expect was linking the test target
    # in the test dir CMakeLists.txt. I also am not sure why.
    # include_directories("${gtest_SOURCE_DIR}")
    add_subdirectory("test")
endif()

