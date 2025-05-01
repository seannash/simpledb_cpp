#include "properties.hpp"

namespace simpledb::jdbc {

std::string Properties::getProperty(std::string_view key) {
    return d_data[std::string(key)];
}

void Properties::setProperty(std::string_view key, std::string_view value) {
    d_data.insert_or_assign(std::string(key), std::string(value));
}

} // namespace simpledb::jdbc 