#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <span>
#include "buffer_wrapper.hpp"
namespace simpledb::file {

class Page {
public:
    Page(int pageSize);

    virtual ~Page() = default;
  
    int getInt(int offset) const;
    void setInt(int offset, int value);

    std::vector<char> getBytes(int offset) const;
    void setBytes(int offset, const std::span<char>& value);

    std::string getString(int offset) const;
    void setString(int offset, std::string_view value);

    std::vector<char>* contents();    

private:
    // TODO: Change to non-paged memory
    std::vector<char> d_data;
    BufferWrapper d_buffer_wrapper;
 
};

} // namespace simpledb::file