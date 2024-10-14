#ifndef UTIL_FILE_IO_BASIC_SERIALIZATION_HPP
#define UTIL_FILE_IO_BASIC_SERIALIZATION_HPP

#include <cstdint>
#include <ostream>
#include <istream>

namespace Util::FileIO {

/**
* Writes an 8 bit value to an ostream
* such that serialized_read will read back in the original value.
* THERE IS NO ERROR CHECKING on the ostream.
*
* @param out ostream to write to.
* @param val value to write to the ostream.
*/
void serialized_write(std::ostream out, uint8_t val);

/**
* Writes a 32 bit value to an ostream
* such that serialized_read will read back in the original value.
* THERE IS NO ERROR CHECKING on the ostream.
*
* @param out ostream to write to.
* @param val value to write to the ostream.
*/
void serialized_write(std::ostream out, uint32_t val);

/**
* Reads an 8 bit value from an istream
* so that it is guaranteed to read in the same value
* that would passed to serialized_write to write.
* THERE IS NO ERROR CHECKING on the istream.
*
* @param in istream to read from.
* @return value read from the stream.
*/
void serialized_read(std::istream in, uint8_t &val);

/**
* Reads a 32 bit value from an istream
* so that it is guaranteed to read in the same value
* that would passed to serialized_write to write.
* THERE IS NO ERROR CHECKING on the istream.
*
* @param in istream to read from.
* @return value read from the stream.
*/
void serialized_read(std::istream in, uint32_t &val);

}

#endif // !UTIL_FILE_IO_BASIC_SERIALIZATION_HPP

