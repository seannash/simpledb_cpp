#include "in_memory_result_set.hpp"
#include "sql_exception.hpp"
#include <sstream>

#include <iostream>

namespace simpledb::jdbc {

InMemoryResultSet::InMemoryResultSet()
    : d_metadata(std::make_unique<ResultSetMetaData>()),
    d_columnIndices(0),
    d_closed(false),
    d_currentRow(0)
{}

InMemoryResultSet::InMemoryResultSet(std::vector<std::string> columnNames,
                                   std::vector<ColumnType> columnTypes,
                                   std::vector<std::string> columnTypesNames,
                                   std::vector<int> columnSizes)
    : d_metadata(std::make_unique<ResultSetMetaData>(
          columnNames.size(),
          columnNames,
          columnTypes,
          columnTypesNames,
          columnSizes)),
    d_columnIndices(columnNames.size()),
    d_closed(false),
    d_currentRow(0)
{
    // Initialize column indices map
    for (std::size_t i = 0; i < columnNames.size(); ++i) {
        d_columnIndices[columnNames[i]] = i;
    }
}

bool InMemoryResultSet::next() {
    checkClosed();
    if (d_currentRow < d_rows.size()-1) {
        ++d_currentRow;
        return true;
    }
    return false;
}

std::string InMemoryResultSet::getString(const std::string& columnLabel) {
    checkClosed();
    validateColumnLabel(columnLabel);
    if (d_currentRow > d_rows.size()) {
        throw SQLException("No current row");
    }
    return std::any_cast<std::string>(d_rows[d_currentRow ][d_columnIndices[columnLabel]]);
}

std::string InMemoryResultSet::getString(int columnIndex) {
    checkClosed();
    validateColumnIndex(columnIndex);
    if (d_currentRow > d_rows.size()) {
        throw SQLException("No current row");
    }
    return std::any_cast<std::string>(d_rows[d_currentRow][columnIndex - 1]);
}

int InMemoryResultSet::getInt(const std::string& columnLabel) {
    checkClosed();
    validateColumnLabel(columnLabel);
    if (d_currentRow > d_rows.size()) {
        throw SQLException("No current row");
    }
    try {
        return std::any_cast<int>(d_rows[d_currentRow ][d_columnIndices[columnLabel]]);
    } catch (const std::exception& e) {
        throw SQLException("Invalid integer value in column: " + columnLabel);
    }
}
int InMemoryResultSet::getInt(int columnIndex) {
    checkClosed();
    validateColumnIndex(columnIndex);
    if (d_currentRow > d_rows.size()) {
        throw SQLException("No current row");
    }
    auto v = d_rows[d_currentRow][columnIndex - 1];
    return std::any_cast<int>(d_rows[d_currentRow][columnIndex - 1]);
}

void InMemoryResultSet::close() {
    d_closed = true;
    d_rows.clear();
    d_currentRow = 0;
}

ResultSetMetaData InMemoryResultSet::getMetaData() {
    checkClosed();
    return *d_metadata;
}

void InMemoryResultSet::addRow(std::vector<std::any> values) {
    if (values.size() != d_columnIndices.size()) {
        throw SQLException("Row size does not match column count");
    }
    d_rows.push_back(values);
}


void InMemoryResultSet::validateColumnLabel(const std::string& columnLabel) const {
    if (d_columnIndices.find(columnLabel) == d_columnIndices.end()) {
        throw SQLException("Unknown column: " + columnLabel);
    }
}

void InMemoryResultSet::validateColumnIndex(int columnIndex) const {
    if (columnIndex < 1 || columnIndex > d_metadata->getColumnCount()) {
        throw SQLException("Invalid column index: " + std::to_string(columnIndex));
    }
}

void InMemoryResultSet::checkClosed() const {
    if (d_closed) {
        throw SQLException("ResultSet is closed");
    }
}

} // namespace simpledb::jdbc
