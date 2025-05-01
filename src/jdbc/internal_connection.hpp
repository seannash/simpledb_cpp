#pragma once

#include <memory>
#include <string_view>
#include "transaction_isolation.hpp"
#include "result_set.hpp"
namespace simpledb::jdbc {

class InternalConnection {

public:
    virtual ~InternalConnection() = default;

    virtual void close() = 0;

    virtual void setAutoCommit(bool autoCommit) = 0;
    virtual bool getAutoCommit() const = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;

    virtual void setTransactionIsolation(TransactionIsolation level) = 0;
    virtual TransactionIsolation getTransactionIsolation() const = 0;

    virtual std::unique_ptr<ResultSet> executeQuery(std::string_view sql) = 0;
    virtual int executeUpdate(std::string_view sql) = 0;
};

} // namespace simpledb::jdbc 