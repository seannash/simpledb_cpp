#include "connection.hpp"
#include "sql_exception.hpp"

namespace simpledb::jdbc {

std::unique_ptr<Statement> Connection::createStatement() {
    return std::make_unique<Statement>(d_internalConnection);
}

std::unique_ptr<PreparedStatement> Connection::prepareStatement(std::string_view sql) {
    return std::make_unique<PreparedStatement>(d_internalConnection, sql);
}

void Connection::close() {
    d_internalConnection->close();
}

void Connection::setAutoCommit(bool autoCommit) {
    d_internalConnection->setAutoCommit(autoCommit);
}

bool Connection::getAutoCommit() const {
    return d_internalConnection->getAutoCommit();
}

void Connection::commit() { 
    d_internalConnection->commit();
}

void Connection::rollback() {
    d_internalConnection->rollback();
}

void Connection::setTransactionIsolation(TransactionIsolation level) {
    d_internalConnection->setTransactionIsolation(level);
}

TransactionIsolation Connection::getTransactionIsolation() const {
    return d_internalConnection->getTransactionIsolation();
}

} // namespace simpledb::jdbc 