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
    uint8_t more_left;
    try {
        this->stream(more_left);
        if (!more_left) {
            this->error_state = Reader::EMPTY;
        }
    } catch (cereal::Exception &) {
        this->error_state = Reader::BAD_READ;
    }
}

unsigned int Reader::pop(HexState &state, bool &b) {
    uint8_t more_left;
    if (this->error_state) {
        return this->error_state;
    }

    try {
        this->stream(state);
        this->stream(b);
        this->stream(more_left);
    } catch (cereal::Exception &) {
        this->error_state = Reader::BAD_READ;
        return this->error_state;
    }

    if (!more_left) {
        this->error_state = Reader::EMPTY;
    } 

    return Reader::CLEAR;
}

unsigned int Reader::read_err() const {
    return this->error_state;
}

Writer::GamestateBool1Writer(ostream &stream) : stream(stream) {
    uint8_t file_type = Util::FileIO::GAMESTATE_BOOL, file_version = 1;
    try {
        this->stream(file_type);
        this->stream(file_version);
    } catch (cereal::Exception &) {
        this->error_state = Writer::BAD_WRITE;
    }
}

Writer::~GamestateBool1Writer() {
    uint8_t more_left = 0;
    this->stream(more_left);
}

unsigned int Writer::push(const HexState &state, const bool &b) {
    if (this->error_state) {
        return this->error_state;
    }

    uint8_t more_left = 1;

    try {
        this->stream(more_left);
        this->stream(state);
        this->stream(b);
    } catch (cereal::Exception &) {
        this->error_state = Writer::BAD_WRITE;
        return this->error_state;
    }

    return Writer::CLEAR;
}

unsigned int Writer::read_err() const {
    return this->error_state;
}

