#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace simpledb::jdbc {

class Properties {
private:
    std::unordered_map<std::string, std::string> d_data;
public:
    std::string getProperty(std::string_view key);
    void setProperty(std::string_view key, std::string_view value);
};

} // namespace simpledb::jdbc 