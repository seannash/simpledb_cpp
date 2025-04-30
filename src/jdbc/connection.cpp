#include "connection.hpp"
#include "sql_exception.hpp"

namespace simpledb::jdbc {

std::unique_ptr<Statement> Connection::createStatement() {
    throw SQLException("Connection::createStatement not implemented");
}

} // namespace simpledb::jdbc 