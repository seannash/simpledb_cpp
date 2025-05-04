#pragma once

#include "jdbc/connection.hpp"
#include "jdbc/sql_exception.hpp"
#include <memory>

namespace simpledb::jdbc {

class ConnectionAdapter : public ::jdbc::Connection {
public:
    ConnectionAdapter() = default;
    virtual ~ConnectionAdapter() = default;

    // Statement creation
    virtual std::unique_ptr<::jdbc::Statement> createStatement() override;

    // Connection management
    virtual void close() override;

    // Transaction management
    virtual void commit() override;
    virtual void rollback() override;
};

} // namespace simpledb::jdbc 