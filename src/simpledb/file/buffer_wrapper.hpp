#pragma once

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace simpledb::file {

class BufferWrapper {
public:
    BufferWrapper(std::span<char> data);

    int getInt(int offset) const;
    void setInt(int offset, int value);

    std::vector<char> getBytes(int offset) const;
    void setBytes(int offset, std::span<char> value);

    std::string getString(int offset) const;
    void setString(int offset, std::string_view value);

    int maxLength(std::string_view str) const;

private:
    std::span<char> d_data;
};

} // namespace simpledb::file