#include "fake_internal_connection.hpp"

#include <algorithm>

#include "sql_exception.hpp"
#include "result_set.hpp"
#include "in_memory_result_set.hpp"

namespace simpledb::jdbc {

namespace {
    std::string to_lower(std::string_view str) {
        std::string result{str};
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
} // namespace

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
    auto sqlString = to_lower(sql);
    if (sqlString == "select 1;") {
        std::vector<std::string> columnNames {"unknown"};
        std::vector<ColumnType> columnTypes {ColumnType::INT};
        std::vector<std::string> columnTypesNames {"int"};
        std::vector<int> columnSizes {10};
        auto resultSet = std::make_unique<InMemoryResultSet>(
            columnNames,
            columnTypes,
            columnTypesNames,
            columnSizes);
        resultSet->addRow({std::any(1)});
        return resultSet;
    } else if (sqlString == "select name from cat;") {
        std::vector<std::string> columnNames {"name"};
        std::vector<ColumnType> columnTypes {ColumnType::STRING};
        std::vector<std::string> columnTypesNames {"varchar"};
        std::vector<int> columnSizes {10};
        auto resultSet = std::make_unique<InMemoryResultSet>(
              columnNames,
              columnTypes,
              columnTypesNames,
              columnSizes);
        resultSet->addRow({std::any(std::string("gato"))});
        resultSet->addRow({std::any(std::string("perro"))});
        return resultSet;
    }
    throw SQLException("Unsupported query: " + std::string(sql));
}

int FakeInternalConnection::executeUpdate(std::string_view sql) {
    return 0;
}

} // namespace simpledb::jdbc
