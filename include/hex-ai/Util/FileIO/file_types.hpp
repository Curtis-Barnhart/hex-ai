/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef UTIL_FILE_IO_HPP
#define UTIL_FILE_IO_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

namespace Util::FileIO {

/**
 * HEX_FILE_TYPE is an enum listing every type of file I want to record.
 * Hopefully the first byte of every file should contain the type it is
 * but because old me was stupid and lazy and needed something quickly
 * I already know some files won't... :(
 *
 * END is a special value which marks the highest value in the enum.
 */
enum HEX_FILE_TYPE: uint8_t {
    UNRECOGNIZED,
    GAMESTATE,
    BOOL,
    END
};

/**
 * info_gamestate_00 prints out information about a GAMESTATE
 * version 0 file to stdout.
 *
 * @param filename path to the file to analyze.
 */
void info_file(const std::string &filename);

/**
 * read_gamestate_00 reads a GAMESTATE version 0 file
 * into a vector of HexStates.
 *
 * Layout:
 *   0x00: number of states
 *   0x04: states
 *
 * @param filename path to file to read in.
 * @param states vector of HexStates to write to.
 * @return 0 if the file was successfully read in.
 *         1 if the file could not be opened for reading.
 *         2 if the file content was corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_gamestate_00(
    const std::string &filename,
    std::vector<GameState::HexState> &states
);

/**
 * read_gamestate_00 reads a BOOL version 0 file
 * into a vector of bools.
 *
 * Layout:
 *   0x00: number of bools
 *   0x04: bools
 *
 * @param filename path to file to write to.
 * @param states vector of bools to write out.
 * @return 0 if the file was successfully read from.
 *         1 if the file could not be opened for reading.
 *         2 if the file content was corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_bools_00(
    const std::string &filename,
    std::vector<bool> &bools
);

/**
 * read_gamestate_01 reads a GAMESTATE version 1 file
 * into a vector of HexStates.
 *
 * TODO: error codes for
 *          - file ending early
 *          - file containing bad values
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of states
 *
 * @param filename path to the file to read from.
 * @param states vector to write HexStates into.
 * @return 0 if the file was successfully read from.
 *         1 if the file could not be opened for reading.
 *         2 if the file was not of the type GAMESTATE.
 *         3 if the file version was not 1.
 *         4 if the file contents were corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_gamestate_01(
    const std::string &filename,
    std::vector<GameState::HexState> &states
);

/**
 * write_gamestate_01 writes a vector of HexStates to a GAMESTATE file
 * with version 1.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of states
 *
 * @param filename path to the file to write to.
 * @param states vector of states to write.
 * @return 0 if the file was successfully written to.
 *         1 if the file could not be opened for writing.
 */
[[nodiscard("Return value is error code.")]]
unsigned int write_gamestate_01(
    const std::string &filename,
    const std::vector<GameState::HexState> &states
);

/**
 * read_bools_01 reads a BOOL version 1 file
 * into a vector of bools.
 *
 * TODO: error codes for
 *          - file ending early
 *          - file containing bad values
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of bools
 *
 * @param filename path to the file to read from.
 * @param bools vector to write bools into.
 * @return 0 if the file was successfully read from.
 *         1 if the file could not be opened for reading.
 *         2 if the file was not of the type BOOL.
 *         3 if the file version was not 1.
 *         4 if the file contents were corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_bools_01(
    const std::string &filename,
    std::vector<bool> &bools
);

/**
 * write_bools_01 writes a vector of bools to a BOOL file
 * with version 1.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of bools
 *
 * @param filename path to the file to write to.
 * @param states vector of bools to write.
 * @return 0 if the file was successfully written to.
 *         1 if the file could not be opened for writing.
 */
[[nodiscard("Return value is error code.")]]
unsigned int write_bools_01(
    const std::string &filename,
    const std::vector<bool> &bools
);

}

#endif // !UTIL_FILE_IO_HPP

