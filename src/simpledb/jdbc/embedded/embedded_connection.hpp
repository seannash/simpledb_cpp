#pragma once

#include "simpledb/jdbc/connection_adapter.hpp"
#include "simpledb/server/simpledb.hpp"
#include "simpledb/tx/transaction.hpp"
#include "jdbc/statement.hpp"
#include <memory>

namespace simpledb::jdbc::embedded {

class EmbeddedConnection : public simpledb::jdbc::ConnectionAdapter {
public:
    explicit EmbeddedConnection(std::unique_ptr<server::SimpleDB> db);
    ~EmbeddedConnection() = default;

    // Statement creation
    std::unique_ptr<::jdbc::Statement> createStatement() override;

    // Connection management
    void close() override;

    // Transaction management
    void commit() override;
    void rollback() override;

    std::shared_ptr<simpledb::tx::Transaction> get_transaction() const;

private:
    std::unique_ptr<simpledb::server::SimpleDB> d_db;
    // FIXME: Change to unique_ptr
    std::shared_ptr<simpledb::tx::Transaction> d_current_tx;
};

} // namespace simpledb::jdbc::embedded
