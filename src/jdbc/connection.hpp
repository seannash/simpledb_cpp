#pragma once

#include <memory>
#include <string_view>
#include "statement.hpp"
#include "prepared_statement.hpp"
#include "transaction_isolation.hpp"
#include "internal_connection.hpp"

namespace simpledb::jdbc {

class Connection {
    std::shared_ptr<InternalConnection> d_internalConnection;

public:
    Connection() = default;
    virtual ~Connection() = default;

    // Statement creation
    virtual std::unique_ptr<Statement> createStatement();
    virtual std::unique_ptr<PreparedStatement> prepareStatement(std::string_view sql);

    // Connection management
    virtual void close();

    // Transaction management
    virtual void setAutoCommit(bool autoCommit);
    virtual bool getAutoCommit() const;
    virtual void commit();
    virtual void rollback();

    // Transaction isolation
    virtual void setTransactionIsolation(TransactionIsolation level);
    virtual TransactionIsolation getTransactionIsolation() const;

protected:
    // Internal connection access
    std::shared_ptr<InternalConnection> getInternalConnection() const { return d_internalConnection; }
    void setInternalConnection(std::shared_ptr<InternalConnection> conn) { d_internalConnection = conn; }
};

} // namespace simpledb::jdbc 