#include "buffer_wrapper.hpp"
#include <cstring>
#include <stdexcept>

namespace simpledb::file {

BufferWrapper::BufferWrapper(std::span<char> data)
    : d_data(data)
{
}

int BufferWrapper::getInt(int offset) const {
    if (offset + sizeof(int) > d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    int value;
    std::memcpy(&value, d_data.data() + offset, sizeof(int));
    return value;
}

void BufferWrapper::setInt(int offset, int value) {
    if (offset + sizeof(int) > d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    std::memcpy(d_data.data() + offset, &value, sizeof(int));
}

std::vector<char> BufferWrapper::getBytes(int offset) const {
    if (offset >= d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    int size = getInt(offset);
    return std::vector<char>(d_data.begin() + offset+sizeof(int),
                             d_data.begin() + offset+sizeof(int)+size);
}

void BufferWrapper::setBytes(int offset, std::span<char> value) {
    if (offset + value.size() > d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    setInt(offset, value.size());
    std::memcpy(d_data.data() + offset+sizeof(int), value.data(), value.size());
}

std::string BufferWrapper::getString(int offset) const {
    if (offset >= d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    // Find null terminator or end of buffer
    int size = getInt(offset);
    const char* start = d_data.data() + offset+sizeof(int);
    const char* end = d_data.data() + offset+sizeof(int)+size;
    return std::string(start, end);
}

void BufferWrapper::setString(int offset, std::string_view value) {
    if (offset + value.size() + 1 > d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    setInt(offset, value.size());
    std::memcpy(d_data.data() + offset+sizeof(int), value.data(), value.size());
}

int BufferWrapper::maxLength(std::string_view str) const {
    return str.size() + sizeof(int);
}

} // namespace simpledb::file
