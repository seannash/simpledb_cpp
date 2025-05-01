#include "in_memory_result_set.hpp"
#include "sql_exception.hpp"
#include <sstream>

namespace simpledb::jdbc {

InMemoryResultSet::InMemoryResultSet()
    : d_metadata(),
    d_columnIndices(0),
    d_closed(false),
    d_currentRow(0)
{}

InMemoryResultSet::InMemoryResultSet(std::vector<std::string> columnNames,
                                   std::vector<std::string> columnTypes,
                                   std::vector<int> columnSizes)
    : d_metadata(std::make_unique<ResultSetMetaData>(
          columnNames.size(), columnNames, columnTypes, columnSizes)),
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
    if (d_currentRow < d_rows.size()) {
        ++d_currentRow;
        return true;
    }
    return false;
}

std::string InMemoryResultSet::getString(const std::string& columnLabel) {
    checkClosed();
    validateColumnLabel(columnLabel);
    if (d_currentRow == 0 || d_currentRow > d_rows.size()) {
        throw SQLException("No current row");
    }
    return d_rows[d_currentRow - 1][d_columnIndices[columnLabel]];
}

int InMemoryResultSet::getInt(const std::string& columnLabel) {
    checkClosed();
    validateColumnLabel(columnLabel);
    if (d_currentRow == 0 || d_currentRow > d_rows.size()) {
        throw SQLException("No current row");
    }
    try {
        return std::stoi(d_rows[d_currentRow - 1][d_columnIndices[columnLabel]]);
    } catch (const std::exception& e) {
        throw SQLException("Invalid integer value in column: " + columnLabel);
    }
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

void InMemoryResultSet::addRow(const std::vector<std::string>& values) {
    checkClosed();
    if (values.size() != d_columnIndices.size()) {
        throw SQLException("Row size does not match column count");
    }
    d_rows.push_back(values);
}

void InMemoryResultSet::addRow(const std::vector<int>& values) {
    checkClosed();
    if (values.size() != d_columnIndices.size()) {
        throw SQLException("Row size does not match column count");
    }
    std::vector<std::string> stringValues;
    stringValues.reserve(values.size());
    for (int value : values) {
        stringValues.push_back(std::to_string(value));
    }
    d_rows.push_back(stringValues);
}

void InMemoryResultSet::addRow(const std::unordered_map<std::string, std::string>& values) {
    checkClosed();
    std::vector<std::string> row(d_columnIndices.size());
    for (const auto& [column, value] : values) {
        if (d_columnIndices.find(column) == d_columnIndices.end()) {
            throw SQLException("Unknown column: " + column);
        }
        row[d_columnIndices[column]] = value;
    }
    d_rows.push_back(row);
}

void InMemoryResultSet::validateColumnLabel(const std::string& columnLabel) const {
    if (d_columnIndices.find(columnLabel) == d_columnIndices.end()) {
        throw SQLException("Unknown column: " + columnLabel);
    }
}

void InMemoryResultSet::checkClosed() const {
    if (d_closed) {
        throw SQLException("ResultSet is closed");
    }
}

} // namespace simpledb::jdbc
