#include "statement_adapter.hpp"

namespace simpledb::jdbc {

std::unique_ptr<::jdbc::ResultSet> StatementAdapter::executeQuery(std::string_view sql) {
    throw ::jdbc::SQLException("operation not implemented");
}

int StatementAdapter::executeUpdate(std::string_view sql) {
    throw ::jdbc::SQLException("operation not implemented");
}

void StatementAdapter::close() {
    throw ::jdbc::SQLException("operation not implemented");
}

} // namespace simpledb::jdbc 