#include "fake_internal_connection.hpp"
#include "sql_exception.hpp"
#include "result_set.hpp"
#include "in_memory_result_set.hpp"

namespace simpledb::jdbc {

void FakeInternalConnection::close() {
    d_closed = true;
}

void FakeInternalConnection::setAutoCommit(bool autoCommit) {
    d_autoCommit = autoCommit;
}

bool FakeInternalConnection::getAutoCommit() const {
    return d_autoCommit;
}

void FakeInternalConnection::commit() {
    // Mock implementation - do nothing
}

void FakeInternalConnection::rollback() {
    // Mock implementation - do nothing
}

void FakeInternalConnection::setTransactionIsolation(TransactionIsolation level) {
    d_transactionIsolation = level;
}

TransactionIsolation FakeInternalConnection::getTransactionIsolation() const {
    return d_transactionIsolation;
}

std::unique_ptr<ResultSet> FakeInternalConnection::executeQuery(std::string_view sql) {
    return std::make_unique<InMemoryResultSet>();
}

int FakeInternalConnection::executeUpdate(std::string_view sql) {
    return 0;
}

} // namespace simpledb::jdbc
