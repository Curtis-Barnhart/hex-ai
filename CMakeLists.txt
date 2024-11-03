# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.26)

# set the project description and name and whatnot
project(
    hex-ai
    DESCRIPTION "Hex ai for fun in C++"
    LANGUAGES CXX
)

# we want to set a sort of "root directory" so that all our includes
# don't have to be goofy relative path messes
# by using "include_directories" we can set a directory to include everything from
include_directories("include")

# this is the subdirectory of our main executable
add_subdirectory(src)

option(HEX_AI_TESTS "Build tests for cpp-ai" OFF)
message(STATUS "HEX_AI_TESTS set to ${HEX_AI_TESTS}")
if (HEX_AI_TESTS)
    # use ctest for tests?
    include(CTest)

    # compile targets for googletest
    add_subdirectory("extern/googletest")
    # For some reason the following line is not necessary for headers to work properly
    # I don't understand why yet.
    # What _is_ necessary that I didn't expect was linking the test target
    # in the test dir CMakeLists.txt. I also am not sure why.
    # include_directories("${gtest_SOURCE_DIR}")
    add_subdirectory("test")
endif()

# HEX_AI_TESTING_DIR is the absolute path to a directory where unit tests for hex-ai
# can create temporary files that are used during the testing process
# It was being used by test/test_HexStateIO.cpp but is not any longer.
if (NOT "${HEX_AI_TESTING_DIR}" STREQUAL "")
    add_definitions("-DHEX_AI_TESTING_DIR=${HEX_AI_TESTING_DIR}")
    message(STATUS "HEX_AI_TESTING_DIR set to ${HEX_AI_TESTING_DIR}.")
else()
    message(STATUS "HEX_AI_TESTING_DIR not set (using default directory './data').")
endif()

# HEX_AI_DATA_DIR is the absolute path to the directory where hex-ai can store
# data that isn't part of the source code or need to be tracked.
# It is a macro that will be used in Util/io.cpp
# If it is not defined by the user at compile time, it will use a relative path
# that is defined in Util/io.cpp
if (NOT "${HEX_AI_DATA_DIR}" STREQUAL "")
    add_definitions("-DHEX_AI_DATA_DIR=${HEX_AI_DATA_DIR}")
    message(STATUS "HEX_AI_DATA_DIR set to ${HEX_AI_DATA_DIR}.")
else()
    message(STATUS "HEX_AI_DATA_DIR not set (using default directory './data').")
endif()

