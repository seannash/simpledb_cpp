#include "statement.hpp"
#include "sql_exception.hpp"

namespace simpledb::jdbc {

Statement::Statement(std::shared_ptr<InternalConnection> internalConnection)
: d_internalConnection(internalConnection) {}

std::unique_ptr<ResultSet> Statement::executeQuery(std::string_view sql) {
    throw SQLException("Statement::executeQuery not implemented");
}

int Statement::executeUpdate(std::string_view sql) {
    throw SQLException("Statement::executeUpdate not implemented");
}

void Statement::close() {
    throw SQLException("Statement::close not implemented");
}

} // namespace simpledb::jdbc
