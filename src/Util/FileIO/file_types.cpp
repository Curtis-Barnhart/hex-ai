/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 * This file is part of hex-ai.
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/details/helpers.hpp>

#include "hex-ai/Util/FileIO/file_types.hpp"
#include "hex-ai/Util/FileIO/file_type_enum.hpp"
#include "hex-ai/GameState/HexState.hpp"

using std::ifstream;
using std::ofstream;
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using GameState::HexState;

/*****************************************
 * All layouts of file types are shown   *
 * in hex-ai/Util/FileIO/file_types.hpp. *
 ****************************************/

/*****************************************
 * GAMESTATE files below                 *
 ****************************************/

unsigned int Util::FileIO::read_gamestate_00(
    istream &in,
    vector<HexState> &states
) {
    try {
        cereal::BinaryInputArchive in_archive(in);
        int32_t count;

        in_archive(count);
        while (count-->0) {
            states.emplace_back();
            in_archive(states.back());
        }
    } catch (cereal::Exception &) {
        return 1;
    }

    return 0;
}

unsigned int Util::FileIO::read_gamestate_01(
    istream &in,
    vector<HexState> &states
) {
    try {
        cereal::BinaryInputArchive in_archive(in);
        in_archive(states);
    } catch (cereal::Exception &) {
        return 1;
    }

    return 0;
}

unsigned int Util::FileIO::write_gamestate_01(
    ostream &out,
    const vector<HexState> &states
) {
    uint8_t file_type = Util::FileIO::GAMESTATE;
    uint8_t file_version = 1;

    try {
        cereal::BinaryOutputArchive out_archive(out);
        out_archive(file_type);
        out_archive(file_version);
        out_archive(states);
    } catch (cereal::Exception &) {
        return 1;
    }

    return 0;
}

/*****************************************
 * BOOL files below                      *
 ****************************************/

unsigned int Util::FileIO::read_bools_00(
    istream &in,
    vector<bool> &bools
) {
    try {
        cereal::BinaryInputArchive in_archive(in);
        char b;

        while (true) {
            in_archive(b);
            if (b != '0' && b != '1') {
                return 1;
            }
            bools.emplace_back(b == '1');
        }
    } catch (cereal::Exception &) {
        // the file ends lol
    }

    return 0;
}

unsigned int Util::FileIO::read_bools_01(
    istream &in,
    vector<bool> &bools
) {
    try {
        cereal::BinaryInputArchive in_archive(in);
        in_archive(bools);
    } catch (cereal::Exception &) {
        return 1;
    }
    
    return 0;
}

unsigned int Util::FileIO::write_bools_01(
    ostream &out,
    const vector<bool> &bools
){
    uint8_t file_type = Util::FileIO::BOOL;
    uint8_t file_version = 1;

    try {
        cereal::BinaryOutputArchive out_archive(out);
        out_archive(file_type);
        out_archive(file_version);
        out_archive(bools);
    } catch (cereal::Exception &) {
        return 1;
    }
    
    return 0;
}

unsigned int Util::FileIO::read_gamestate_bools_00(
    istream &in,
    vector<HexState> &states,
    vector<bool> &bools
) {
    try {
        cereal::BinaryInputArchive in_archive(in);
        in_archive(states);
        in_archive(bools);
    } catch (cereal::Exception &) {
        return 1;
    }

    return 0;
}

unsigned int Util::FileIO::write_gamestate_bools_00(
    ostream &out,
    const vector<HexState> &states,
    const vector<bool> &bools
) {
    uint8_t file_type = Util::FileIO::GAMESTATE_BOOL, file_version = 0;

    try {
        cereal::BinaryOutputArchive out_archive(out);
        out_archive(file_type);
        out_archive(file_version);
        out_archive(states);
        out_archive(bools);
    } catch (cereal::Exception &) {
        return 1;
    }

    return 0;
}

