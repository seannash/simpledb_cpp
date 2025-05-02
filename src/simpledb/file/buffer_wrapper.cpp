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
    return std::vector<char>(d_data.begin() + offset, d_data.end());
}

void BufferWrapper::setBytes(int offset, std::span<char> value) {
    if (offset + value.size() > d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    std::memcpy(d_data.data() + offset, value.data(), value.size());
}

std::string BufferWrapper::getString(int offset) const {
    if (offset >= d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    // Find null terminator or end of buffer
    const char* start = d_data.data() + offset;
    const char* end = d_data.data() + d_data.size();
    const char* null_pos = static_cast<const char*>(std::memchr(start, '\0', end - start));
    if (null_pos) {
        return std::string(start, null_pos);
    }
    return std::string(start, end);
}

void BufferWrapper::setString(int offset, std::string_view value) {
    if (offset + value.size() + 1 > d_data.size()) {
        throw std::out_of_range("Buffer access out of bounds");
    }
    std::memcpy(d_data.data() + offset, value.data(), value.size());
    d_data[offset + value.size()] = '\0';  // Add null terminator
}

std::vector<char>* BufferWrapper::contents() {
    // Note: This is potentially dangerous as it exposes the internal buffer
    // Consider if this method is really needed
    return nullptr;
}

} // namespace simpledb::file
