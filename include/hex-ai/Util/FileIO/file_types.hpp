/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef UTIL_FILE_IO_HPP
#define UTIL_FILE_IO_HPP

#include <ostream>
#include <istream>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

namespace Util::FileIO {

/**
 * read_gamestate_00 reads a GAMESTATE version 0 istream
 * into a vector of HexStates.
 * `in` is expected to be a working, readable state.
 *
 * Layout:
 *   0x00: number of states
 *   0x04: states
 *
 * @param in istream to read from.
 * @param states vector of HexStates to write to.
 * @return 0 if the istream was successfully read in.
 *         1 if the istream content was corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_gamestate_00(
    std::istream &in,
    std::vector<GameState::HexState> &states
);

/**
 * read_gamestate_00 reads a BOOL version 0 file
 * into a vector of bools.
 *
 * Layout:
 *   0x00: bools
 *
 * @param in istream to read from.
 * @param states vector of bools to write into.
 * @return 0 if the istream was successfully read from.
 *         1 if the istream content was corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_bools_00(
    std::istream &in,
    std::vector<bool> &bools
);

/**
 * read_gamestate_01 reads a GAMESTATE version 1 istream
 * into a vector of HexStates.
 * The istream is assumed to not include the type and version.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of states <- istream position should start here
 *
 * @param in istream to read from.
 * @param states vector to write HexStates into.
 * @return 0 if the istream was successfully read from.
 *         1 if the istream contents were corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_gamestate_01(
    std::istream &in,
    std::vector<GameState::HexState> &states
);

/**
 * write_gamestate_01 writes a vector of HexStates to a GAMESTATE file
 * with version 1.
 * The istream is assumed to not include the type and version.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of states <- istream pos should start here
 *
 * @param out ostream to write to.
 * @param states vector of states to write.
 * @return 0 if the ostream was successfully written to.
 *         1 if there was an error while writing.
 */
[[nodiscard("Return value is error code.")]]
unsigned int write_gamestate_01(
    std::ostream &out,
    const std::vector<GameState::HexState> &states
);

/**
 * read_bools_01 reads a BOOL version 1 istream
 * into a vector of bools.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of bools
 *
 * @param filename path to the file to read from.
 * @param bools vector to write bools into.
 * @throws cereal::Exception if an error is encountered trying to read
 *     from the outstream.
 * @return 0 if the istream was successfully read from.
 *         1 if the file contents were corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_bools_01(
    std::istream &in,
    std::vector<bool> &bools
);

/**
 * write_bools_01 writes a vector of bools to a BOOL ostream
 * with version 1.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of bools
 *
 * @param ostream to write to.
 * @param bools vector of bools to write.
 * @return 0 if the ostream was successfully written to.
 *         1 if an error was encountered trying to write to the ostream.
 */
[[nodiscard("Return value is error code.")]]
unsigned int write_bools_01(
    std::ostream &out,
    const std::vector<bool> &bools
);

/**
 * write_gamestate_bools_00 writes a vector of gamestates
 * and a vector of bools to a GAMESTATE_BOOL file
 * with version 0.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of gamestates
 *   0x??: vector of bools
 *
 * @param out ostream to write to.
 * @param states vector of hexstates to write out.
 * @param bools vector of bools to write out.
 * @return 0 if the ostream was successfully written to,
 *         1 if the ostream could not be written to.
 */
unsigned int write_gamestate_bools_00(
    std::ostream &out,
    const std::vector<GameState::HexState> &states,
    const std::vector<bool> &bools
);

/**
 * read_gamestate_bools_00 reads in a vector of gamestates
 * and a vector of bools from a GAMESTATE_BOOL file
 * with version 0.
 * The istream is assumed to not include the type and version.
 *
 * Layout:
 *   0x00: file type (0x01)
 *   0x01: file type version (0x01)
 *   0x02: vector of gamestates <- istream pos should start here.
 *   0x??: vector of bools
 *
 * @param in istream to read from
 * @param states vector of bools to write into.
 * @param bools vector of bools to write into.
 * @return 0 if the file was successfully read from.
 *         1 if the file contents were corrupted and could not be interpreted.
 */
[[nodiscard("Return value is error code.")]]
unsigned int read_gamestate_bools_00(
    std::istream &in,
    std::vector<GameState::HexState> &states,
    std::vector<bool> &bools
);

}

#endif // !UTIL_FILE_IO_HPP

