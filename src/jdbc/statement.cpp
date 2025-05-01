#include "statement.hpp"
#include "sql_exception.hpp"

namespace simpledb::jdbc {

Statement::Statement(std::shared_ptr<InternalConnection> internalConnection)
: d_internalConnection(internalConnection) {}

std::unique_ptr<ResultSet> Statement::executeQuery(std::string_view sql) {
    return d_internalConnection->executeQuery(sql);
}

int Statement::executeUpdate(std::string_view sql) {
    return d_internalConnection->executeUpdate(sql);
}

void Statement::close() {
    throw SQLException("Statement::close not implemented");
}

} // namespace simpledb::jdbc
