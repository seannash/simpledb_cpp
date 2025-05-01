#include "block_id.hpp"
#include <fmt/format.h>
namespace simpledb::file {

BlockId::BlockId(std::string_view filename, int blockNumber)
: d_filename(filename), d_blockNumber(blockNumber) {}

std::string BlockId::fileName() const {
    return d_filename;
}   

int BlockId::number() const {
    return d_blockNumber;
}

bool BlockId::operator==(const BlockId& other) const {
    return d_filename == other.d_filename && d_blockNumber == other.d_blockNumber;
}   

bool BlockId::operator!=(const BlockId& other) const {
    return !(*this == other);
}   

std::string BlockId::toString() const {
    return fmt::format("BlockId(filename={}, blockNumber={})", d_filename, d_blockNumber);
}   

} // namespace simpledb::file

