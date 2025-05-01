#pragma once

#include <memory>
#include <string_view>
#include "statement.hpp"
#include "prepared_statement.hpp"
#include "transaction_isolation.hpp"
#include "internal_connection.hpp"

namespace simpledb::jdbc {

class FakeInternalConnection : public InternalConnection {

public:
    ~FakeInternalConnection() = default;

    void close();

    void setAutoCommit(bool autoCommit) override;
    bool getAutoCommit() const override;
    void commit() override;
    void rollback() override;

    void setTransactionIsolation(TransactionIsolation level) override;
    TransactionIsolation getTransactionIsolation() const override;

    std::unique_ptr<ResultSet> executeQuery(std::string_view sql) override;
    int executeUpdate(std::string_view sql) override;

private:
    bool d_closed = false;
    bool d_autoCommit = true;
    TransactionIsolation d_transactionIsolation = TransactionIsolation::READ_COMMITTED;

};

} // namespace simpledb::jdbc 