#include "page.hpp"

namespace simpledb::file {

Page::Page(int pageSize)
: d_data(pageSize), d_buffer_wrapper(d_data) {}

int Page::getInt(int offset) const {
    return *reinterpret_cast<const int*>(d_data.data() + offset);
}

void Page::setInt(int offset, int value) {
    d_buffer_wrapper.setInt(offset, value);
}   

std::vector<char> Page::getBytes(int offset) const {
    return d_buffer_wrapper.getBytes(offset);
}   

void Page::setBytes(int offset, std::span<char> value) {

    d_buffer_wrapper.setBytes(offset, value);
}   

std::string Page::getString(int offset) const {
    return d_buffer_wrapper.getString(offset);
}   

void Page::setString(int offset, std::string_view value) {
    d_buffer_wrapper.setString(offset, value);
}      

std::vector<char>* Page::contents() {
    return &d_data;
}   

} // namespace simpledb::file