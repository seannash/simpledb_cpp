#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace simpledb::file {

class Page {
public:
    Page(int pageSize);
    Page(std::vector<char> data);
    virtual ~Page() = default;
  
    int getInt(int offset) const;
    void setInt(int offset, int value);

    std::vector<char> getBytes(int offset) const;
    void setBytes(int offset, const std::vector<char>& value);

    std::string getString(int offset) const;
    void setString(int offset, std::string_view value);

    std::vector<char>* contents();    

private:
    std::vector<char> d_data;
 
};

} // namespace simpledb::file