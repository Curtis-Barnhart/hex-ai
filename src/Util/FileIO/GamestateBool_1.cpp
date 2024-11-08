/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdint>
#include <istream>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/details/helpers.hpp>

#include "hex-ai/Util/FileIO/GamestateBool_1.hpp"
#include "hex-ai/Util/FileIO/file_type_enum.hpp"

using std::istream;
using std::ostream;
using BIArc = cereal::BinaryInputArchive;
using BOArc = cereal::BinaryOutputArchive;
using Reader = Util::FileIO::GamestateBool1Reader;
using Writer = Util::FileIO::GamestateBool1Writer;
using GameState::HexState;

Reader::GamestateBool1Reader(istream &stream) : stream(stream) {
    uint32_t size;
    try {
        this->stream(size);
        this->size = size;
        if (size == 0) {
            this->error_state = Reader::EMPTY;
        }
    } catch (cereal::Exception &) {
        this->error_state = Reader::BAD_READ;
    }
}

unsigned int Reader::left() const {
    return this->size;
}

unsigned int Reader::pop(HexState &state, bool &b) {
    if (this->error_state) {
        return this->error_state;
    }

    try {
        this->stream(state);
        this->stream(b);
        this->size--;
    } catch (cereal::Exception &) {
        this->error_state = Reader::BAD_READ;
        return this->error_state;
    }

    if (this->size == 0) {
        this->error_state = Reader::EMPTY;
    } 

    return Reader::CLEAR;
}

unsigned int Reader::read_err() const {
    return this->error_state;
}

Writer::GamestateBool1Writer(ostream &stream, unsigned int reserved)
    : stream(stream), size(reserved)
{
    uint8_t file_type = Util::FileIO::GAMESTATE_BOOL, file_version = 1;
    uint32_t size_to_write = reserved;
    try {
        this->stream(file_type);
        this->stream(file_version);
        this->stream(size_to_write);
        if (reserved == 0) {
            this->error_state = Writer::FULL;
        }
    } catch (cereal::Exception &) {
        this->error_state = Writer::BAD_WRITE;
    }
}

unsigned int Writer::left() const {
    return this->size;
}

unsigned int Writer::push(const HexState &state, const bool &b) {
    if (this->error_state) {
        return this->error_state;
    }

    try {
        this->stream(state);
        this->stream(b);
        this->size--;
    } catch (cereal::Exception &) {
        this->error_state = Writer::BAD_WRITE;
        return this->error_state;
    }

    if (this->size == 0) {
        this->error_state = Writer::CLEAR;
    }

    return Writer::CLEAR;
}

unsigned int Writer::read_err() const {
    return this->error_state;
}

