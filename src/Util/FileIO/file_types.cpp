#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>

#include "hex-ai/Util/FileIO/file_types.hpp"
#include "cereal/details/helpers.hpp"
#include "hex-ai/GameState/HexState.hpp"

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using GameState::HexState;

/*****************************************
 * All layouts of file types are shown   *
 * in hex-ai/Util/FileIO/file_types.hpp. *
 ****************************************/

void Util::FileIO::info_file(
    const string &filename
) {
    enum { VERSIONED_FILE = 1, UNVERSIONED_FILE = 2, NOT_A_FILE = 4 };
    uint8_t claimed_filetype, claimed_version;
    unsigned int real_filetype;
    ifstream infile(filename);

    std::printf("file '%s':\n", filename.c_str());

    // verify file could even be opened
    if (!infile.good()) {
        std::printf("    ERROR: file could not be opened for reading.\n");
        return;
    }

    cereal::BinaryInputArchive cereal_in(infile);
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

    // try to interpret the file as though it is a versioned file
    if (real_filetype & VERSIONED_FILE) {
        vector<HexState> states;
        vector<bool> bools;
        try {
            switch (claimed_filetype) {
/*GAMESTATE*/       case Util::FileIO::HEX_FILE_TYPE::GAMESTATE:
                    switch (claimed_version) {
/*VERSION 1*/               case 1:
                            read_gamestate_01(filename, states);
                            std::printf("    type: GAMESTATE\n");
                            std::printf("    version: 1\n");
                            std::printf("    data:\n");
                            std::printf("         states : %zu\n", states.size());
                            return;
/*BAD VERSION*/         default:
                            // file version doesn't exist
                            states.clear();
                            break;
                    }
                    break;
/*BOOL*/            case Util::FileIO::HEX_FILE_TYPE::BOOL:
                    switch (claimed_version) {
/*VERSION 1*/               case 1:
                            read_bools_01(filename, bools);
                            std::printf("    type: BOOL\n");
                            std::printf("    version: 1\n");
                            std::printf("    data:\n");
                            std::printf("         bools : %zu\n", states.size());
                            return;
/*BAD VERSION*/         default:
                            // file version doesn't exist
                            bools.clear();
                            break;
                    }
                    break;
            }
        } catch (cereal::Exception &) {
            // oops, looks like it wasn't actually a versioned file.
            // It's still possible that it is unversioned.
        }
    }

    // at this point, we know the file is not versioned,
    // which means it is either versioned or not a good file at all
    // we will just trial and error through all unversioned file types we know.
    { // GAMESTATE VERSION 0
        vector<HexState> states;
        unsigned int error = read_gamestate_00(filename, states);

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
        vector<bool> bools;
        unsigned int error = read_bools_00(filename, bools);

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

unsigned int Util::FileIO::read_gamestate_00(
    const string &filename,
    vector<HexState> &states
) {
    ifstream ifile(filename);

    if (!ifile.good()) {
        return 1;
    }

    cereal::BinaryInputArchive in_archive(ifile);
    int32_t count;

    try {
        in_archive(count);
        while (count-->0) {
            states.emplace_back();
            in_archive(states.back());
            if (!states.back().verify_board_state()) {
                return 2;
            }
        }
    } catch (cereal::Exception &) {
        return 2;
    }

    return 0;
}

unsigned int Util::FileIO::read_bools_00(
    const string &filename,
    vector<bool> &bools
) {
    ifstream ifile(filename);

    if (!ifile.good()) {
        return 1;
    }

    cereal::BinaryInputArchive in_archive(ifile);
    
    char b;
    try {
        while (true) {
            in_archive(b);
            if (b != '0' && b != '1') {
                return 2;
            }
            bools.emplace_back(b == '1');
        }
    } catch (cereal::Exception &) {
        // the file ends lol
    }

    return 0;
}

unsigned int Util::FileIO::read_gamestate_01(
    const string &filename,
    vector<HexState> &states
) {
    ifstream ifile(filename);

    if (!ifile.good()) {
        return 1;
    }

    uint8_t file_type;
    uint8_t file_version;

    cereal::BinaryInputArchive in_archive(ifile);

    try {
        in_archive(file_type);
        if (file_type != Util::FileIO::GAMESTATE) {
            return 2;
        }
        in_archive(file_version);
        if (file_version != 1) {
            return 3;
        }
        in_archive(states);
    } catch (cereal::Exception &) {
        return 4;
    }
    
    return 0;
}

unsigned int Util::FileIO::write_gamestate_01(
    const string &filename,
    const vector<HexState> &states
) {
    ofstream ofile(filename);

    if (!ofile.good()) {
        return 1;
    }

    uint8_t file_type = Util::FileIO::GAMESTATE;
    uint8_t file_version = 1;

    cereal::BinaryOutputArchive out_archive(ofile);

    out_archive(file_type);
    out_archive(file_version);

    out_archive(states);
    
    return 0;
}

unsigned int Util::FileIO::read_bools_01(
    const string &filename,
    vector<bool> &bools
) {
    ifstream ifile(filename);

    if (!ifile.good()) {
        return 1;
    }

    uint8_t file_type;
    uint8_t file_version;

    cereal::BinaryInputArchive in_archive(ifile);

    try {
        in_archive(file_type);
        if (file_type != Util::FileIO::BOOL) {
            return 2;
        }
        in_archive(file_version);
        if (file_version != 1) {
            return 3;
        }
        in_archive(bools);
    } catch (cereal::Exception &) {
        return 4;
    }
    
    return 0;
}

unsigned int Util::FileIO::write_bools_01(
    const string &filename,
    const vector<bool> &bools
){
    ofstream ofile(filename);

    if (!ofile.good()) {
        return 1;
    }

    uint8_t file_type = Util::FileIO::GAMESTATE;
    uint8_t file_version = 1;

    cereal::BinaryOutputArchive out_archive(ofile);

    out_archive(file_type);
    out_archive(file_version);
    out_archive(bools);
    
    return 0;
}

