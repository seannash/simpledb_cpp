#pragma once

#include <memory>
#include "statement.hpp"

namespace simpledb::jdbc {

class Connection {
public:
    Connection() = default;
    ~Connection() = default;
    std::unique_ptr<Statement> createStatement();
    void close();
};

} // namespace simpledb::jdbc 