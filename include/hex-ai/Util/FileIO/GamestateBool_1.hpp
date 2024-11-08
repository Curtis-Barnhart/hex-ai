/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef UTIL_FILEIO_GAMESTATEBOOL_1_HPP
#define UTIL_FILEIO_GAMESTATEBOOL_1_HPP

#include <iostream>
#include <istream>
#include <ostream>

#include <cereal/archives/binary.hpp>

#include "hex-ai/GameState/HexState.hpp"

namespace Util::FileIO {

/**
* When error_state is CLEAR, then size will refer to how many more states can
* be read from this object. When error_state is EMPTY, it means that all states
* have been successfully read from this object. When error_state is BAD_READ,
* it means that there was some error reading in data. If error_state is BAD_READ,
* then size is not defined to have any particular meaning.
*
*/
class GamestateBool1Reader {
public:
    explicit GamestateBool1Reader(std::istream &stream);
    
    // make sure copy constructor is indeed automatically deleted or smth

    enum ERRORS { CLEAR, EMPTY, BAD_READ };
    unsigned int pop(GameState::HexState &state, bool &b);
    unsigned int read_err() const;

private:
    // does not have a public default ctor, must be init in ctor
    cereal::BinaryInputArchive stream;
    unsigned int error_state = GamestateBool1Reader::CLEAR;
};

/**
* The ostream is not guaranteed to actually be flushed until destruction of this
* object (due to the similar fact about the underlying cereal::BinaryInputArchive)
*/
class GamestateBool1Writer {
public:
    explicit GamestateBool1Writer(std::ostream &stream);
    ~GamestateBool1Writer();

    // make sure copy constructor is indeed automatically deleted or smth

    enum ERRORS { CLEAR, BAD_WRITE };
    unsigned int push(const GameState::HexState &state, const bool &b);
    unsigned int read_err() const;
    
private:
    // does not have a public default ctor, must be init in ctor
    cereal::BinaryOutputArchive stream;
    unsigned int error_state = GamestateBool1Writer::CLEAR;
};

}

#endif // !UTIL_FILEIO_GAMESTATEBOOL_1_HPP

