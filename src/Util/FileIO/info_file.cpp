#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

#include "hex-ai/Util/FileIO/info_file.hpp"
#include "hex-ai/Util/FileIO/file_types.hpp"
#include "hex-ai/Util/FileIO/GamestateBool_1.hpp"
#include "hex-ai/Util/FileIO/file_type_enum.hpp"
#include "hex-ai/GameState/HexState.hpp"

using std::string;
using std::ifstream;
using std::vector;
using GameState::HexState;
using Util::FileIO::GamestateBool1Reader;

unsigned int analyze_gamestatebool1(ifstream &stream) {
    HexState h;
    bool b;
    int x = 0;
    GamestateBool1Reader reader(stream);
    while (reader.read_err() == GamestateBool1Reader::CLEAR) {
        reader.pop(h, b);
        x++;
    }
    // if the reader was fully emptied without any other error
    if (reader.read_err() == GamestateBool1Reader::EMPTY) {
        std::printf("    type: GAMESTATE_BOOL\n");
        std::printf("    version: 1\n");
        std::printf("    data:\n");
        std::printf("        states: %d\n", x);
        std::printf("        bools: %d\n", x);
        return 0;
    }
    return 1;
}

void Util::FileIO::info_file(
    const string &filename
) {
    // TODO: Yeah you can make this nicer you don't need three you need 1
    enum { VERSIONED_FILE = 1, UNVERSIONED_FILE = 2, NOT_A_FILE = 4 };
    uint8_t claimed_filetype, claimed_version;
    unsigned int real_filetype;
    ifstream in_stream(filename, ifstream::binary);

    std::printf("file '%s':\n", filename.c_str());

    // verify file could even be opened
    if (!in_stream.good()) {
        std::printf("    ERROR: file could not be opened for reading.\n");
        return;
    }

    {
        cereal::BinaryInputArchive cereal_in(in_stream);
        // look for the filetype and version number
        try {
            cereal_in(claimed_filetype);
            if (claimed_filetype >= Util::FileIO::HEX_FILE_TYPE::END) {
                real_filetype = UNVERSIONED_FILE | NOT_A_FILE;
            } else {
                real_filetype = VERSIONED_FILE | UNVERSIONED_FILE | NOT_A_FILE;
            }
            cereal_in(claimed_version);
        } catch (cereal::Exception &) {
            real_filetype = UNVERSIONED_FILE | NOT_A_FILE;
        }
    }

    // try to interpret the file as though it is a versioned file
    if (real_filetype & VERSIONED_FILE) {
        vector<HexState> states;
        vector<bool> bools;

        switch (claimed_filetype) {
            case Util::FileIO::HEX_FILE_TYPE::GAMESTATE:
                switch (claimed_version) {
                    case 1:
                        if (!read_gamestate_01(in_stream, states)) {
                            std::printf("    type: GAMESTATE\n");
                            std::printf("    version: 1\n");
                            std::printf("    data:\n");
                            std::printf("         states: %zu\n", states.size());
                            return;
                        }
                        break;
                }
                break;
            case Util::FileIO::HEX_FILE_TYPE::BOOL:
                switch (claimed_version) {
                    case 1:
                        if (!read_bools_01(in_stream, bools)) {
                            std::printf("    type: BOOL\n");
                            std::printf("    version: 1\n");
                            std::printf("    data:\n");
                            std::printf("         bools: %zu\n", bools.size());
                            return;
                        }
                }
                break;
            case Util::FileIO::HEX_FILE_TYPE::GAMESTATE_BOOL:
                switch (claimed_version) {
                    case 0:
                        if (!read_gamestate_bools_00(in_stream, states, bools)) {
                            std::printf("    type: GAMESTATE_BOOL\n");
                            std::printf("    version: 0\n");
                            std::printf("    data:\n");
                            std::printf("        states: %zu\n", states.size());
                            std::printf("        bools: %zu\n", bools.size());
                            return;
                        }
                        break;
                    case 1:
                        if (!analyze_gamestatebool1(in_stream)) {
                            return;
                        }
                        break;
                }
                break;
        }
    }

    // at this point, we know the file is not versioned,
    // which means it is either versioned or not a good file at all
    // we will just trial and error through all unversioned file types we know.
    { // GAMESTATE VERSION 0
        in_stream.clear();
        in_stream.seekg(0);
        vector<HexState> states;
        unsigned int error = read_gamestate_00(in_stream, states);

        switch (error) {
            case 0:
                std::printf("    type: GAMESTATE\n");
                std::printf("    version: 0\n");
                std::printf("    data:\n");
                std::printf("        states: %zu\n", states.size());
                return;
            case 2:
                // it's not a gamestate version 0 file...
                break;
        }
    }
    { // BOOL VERSION 0
        in_stream.clear();
        in_stream.seekg(0);
        vector<bool> bools;
        unsigned int error = read_bools_00(in_stream, bools);

        switch (error) {
            case 0:
                std::printf("    type: BOOL\n");
                std::printf("    version: 0\n");
                std::printf("    data:\n");
                std::printf("        bools: %zu\n", bools.size());
                return;
            case 2:
                // it's not a gamestate version 0 file...
                break;
        }
    }

    // At this point, it's not a versioned file or an unersioned file
    // It's not anything we recognize, so we give up
    std::printf("    ERROR: file content is corrupted and cannot be interpreted\n");
}

