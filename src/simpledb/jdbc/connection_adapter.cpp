#include "connection_adapter.hpp"

namespace simpledb::jdbc {

std::unique_ptr<::jdbc::Statement> ConnectionAdapter::createStatement() {
    throw ::jdbc::SQLException("operation not implemented");
}

void ConnectionAdapter::close() {
    throw ::jdbc::SQLException("operation not implemented");
}

void ConnectionAdapter::commit() {
    throw ::jdbc::SQLException("operation not implemented");
}

void ConnectionAdapter::rollback() {
    throw ::jdbc::SQLException("operation not implemented");
}

} // namespace simpledb::jdbc
