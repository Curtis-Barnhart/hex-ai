#ifndef HEX_AI_UTIL_FILE_IO_FILE_TYPE_ENUM_HPP
#define HEX_AI_UTIL_FILE_IO_FILE_TYPE_ENUM_HPP

#include <cstdint>

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
    GAMESTATE_BOOL,
    END
};

}

#endif

