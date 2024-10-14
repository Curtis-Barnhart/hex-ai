#include "hex-ai/Util/FileIO/basic_serialization.hpp"

void Util::FileIO::serialized_write(std::ostream out, uint8_t val) {
    out.put(val);
}

void Util::FileIO::serialized_write(std::ostream out, uint32_t val) {
    for (int x = 4; x-->0;) {
        out.put((val >> (8 * x)) & 0xff);
    }
}

void Util::FileIO::serialized_read(std::istream in, uint8_t &val) {
    val = in.get();
}

void Util::FileIO::serialized_read(std::istream in, uint32_t &val) {
    val = 0;
    for (int x = 4; x-->0;) {
        val |= in.get();
        val <<= 8;
    }
}

