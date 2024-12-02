#ifndef HEX_AI_IO_FILE_TYPE_ENUM_HPP
#define HEX_AI_IO_FILE_TYPE_ENUM_HPP

#include <cstdint>

namespace Io {

/**
 * HEX_FILE_TYPE is an enum listing every type of file I want to record.
 *
 * END is a special value which marks the highest value in the enum.
 */
enum HEX_FILE_TYPE: uint8_t {
    UNRECOGNIZED,
    GAMESTATE_BOOL,
    END
};

}

#endif

