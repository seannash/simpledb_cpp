#pragma once

#include <memory>
#include <string_view>

#include "statement.hpp"

namespace jdbc {

class Connection {
public:

    virtual ~Connection() = default;

    // Statement creation
    virtual std::unique_ptr<Statement> createStatement()=  0;

    // Connection management
    virtual void close() = 0;

    // Transaction management
    virtual void commit() = 0;
    virtual void rollback() = 0;


};

} // namespace simpledb::jdbc 