#include "page.hpp"

namespace simpledb::file {

Page::Page(int pageSize)
: d_data(pageSize) {}

Page::Page(std::vector<char> data)
: d_data(data) {}

int Page::getInt(int offset) const {
    return *reinterpret_cast<const int*>(d_data.data() + offset);
}

void Page::setInt(int offset, int value) {
    *reinterpret_cast<int*>(d_data.data() + offset) = value;
}   

std::vector<char> Page::getBytes(int offset) const {
    return std::vector<char>(d_data.begin() + offset, d_data.begin() + offset + 4);
}   

void Page::setBytes(int offset, const std::vector<char>& value) {
    std::copy(value.begin(), value.end(), d_data.begin() + offset);
}   

std::string Page::getString(int offset) const {
    auto length = getInt(offset);
    return std::string(d_data.begin() + offset + 4, d_data.begin() + offset + 4 + length);
}   

void Page::setString(int offset, std::string_view value) {
    setInt(offset, value.length());
    std::copy(value.begin(), value.end(), d_data.begin() + offset + 4);
}      

std::vector<char>* Page::contents() {
    return &d_data;
}   

} // namespace simpledb::file