/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HEX_AI_IO_GAMESTATEBOOL_1_HPP
#define HEX_AI_IO_GAMESTATEBOOL_1_HPP

#include <cstdint>
#include <iostream>
#include <istream>
#include <ostream>

#include <cereal/archives/binary.hpp>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/Io/io_enums.hpp"

namespace Io {

/**
* GamestateBool0Reader<bsize> is a class that allows reading from a
* GamestateBool stream of version 0 with HexState board size bsize.
*/
template<int bsize>
class GamestateBool0Reader {
public:
    enum ERRORS { CLEAR, EMPTY, BAD_READ };

    /**
     * Constructor takes in a stream from which it will attempt to read.
     * The error status of this object will be immediately set.
     *
     * @param stream the stream to read from.
     */
    explicit GamestateBool0Reader(std::istream &stream): stream(stream) {
        uint8_t more_left;
        try {
            this->stream(more_left);
            if (!more_left) {
                this->error_state = GamestateBool0Reader::EMPTY;
            }
        } catch (cereal::Exception &) {
            this->error_state = GamestateBool0Reader::BAD_READ;
        }
    }
    
    unsigned int pop(GameState::HexState<bsize> &state, bool &b) {
        uint8_t more_left;
        if (this->error_state) {
            return this->error_state;
        }

        try {
            this->stream(state);
            this->stream(b);
            this->stream(more_left);
        } catch (cereal::Exception &) {
            this->error_state = BAD_READ;
            return this->error_state;
        }

        if (!more_left) {
            this->error_state = EMPTY;
        } 

        return CLEAR;
    }

    unsigned int read_err() const;

private:
    // does not have a public default ctor, must be init in ctor
    cereal::BinaryInputArchive stream;
    unsigned int error_state = CLEAR;
};

/**
* The ostream is not guaranteed to actually be flushed until destruction of this
* object (due to the similar fact about the underlying cereal::BinaryInputArchive)
*/
template<int bsize>
class GamestateBool1Writer {
public:
    enum ERRORS { CLEAR, BAD_WRITE };

    explicit GamestateBool1Writer(std::ostream &stream) : stream(stream) {
        uint8_t file_type = Io::GAMESTATE_BOOL, file_version = 1, board_size = bsize;
        
        try {
            this->stream(file_type);
            this->stream(file_version);
            this->stream(board_size);
        } catch (cereal::Exception &) {
            this->error_state = BAD_WRITE;
        }
    }

    ~GamestateBool1Writer() {
        uint8_t more_left = 0;
        this->stream(more_left);
    }

    unsigned int push(const GameState::HexState<bsize> &state, const bool &b) {
        if (this->error_state) {
            return this->error_state;
        }

        uint8_t more_left = 1;
        try {
            this->stream(more_left);
            this->stream(state);
            this->stream(b);
        } catch (cereal::Exception &) {
            this->error_state = BAD_WRITE;
            return this->error_state;
        }

        return CLEAR;
    }
    unsigned int read_err() const {
        return this->error_state;
    }
    
private:
    // does not have a public default ctor, must be init in ctor
    cereal::BinaryOutputArchive stream;
    unsigned int error_state = GamestateBool1Writer::CLEAR;
};

}

#endif // !HEX_AI_IO_GAMESTATEBOOL_1_HPP

