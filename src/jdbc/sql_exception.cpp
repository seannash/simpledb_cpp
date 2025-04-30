#include "sql_exception.hpp"

namespace simpledb::jdbc {

SQLException::SQLException(const std::string& message) : message(message) {}

const char* SQLException::what() const noexcept {
    return message.c_str();
}

} // namespace simpledb::jdbc 