#include "result_set_metadata.hpp"
#include "sql_exception.hpp"

namespace simpledb::jdbc {

ResultSetMetaData::ResultSetMetaData(int columnCount, 
                                   std::vector<std::string> columnNames,
                                   std::vector<std::string> columnTypeNames,
                                   std::vector<int> columnDisplaySizes)
    : d_columnCount(columnCount)
    , d_columnNames(std::move(columnNames))
    , d_columnTypeNames(std::move(columnTypeNames))
    , d_columnDisplaySizes(std::move(columnDisplaySizes))
{
    if (d_columnCount < 0) {
        throw SQLException("Invalid column count: " + std::to_string(d_columnCount));
    }
    if (d_columnNames.size() != d_columnCount ||
        d_columnTypeNames.size() != d_columnCount ||
        d_columnDisplaySizes.size() != d_columnCount) {
        throw SQLException("Column metadata arrays size mismatch");
    }
}

std::size_t ResultSetMetaData::getColumnCount() const {
    return d_columnCount;
}

std::string ResultSetMetaData::getColumnName(int columnIndex) const {
    if (columnIndex < 1 || columnIndex > d_columnCount) {
        throw SQLException("Invalid column index: " + std::to_string(columnIndex));
    }
    return d_columnNames[columnIndex - 1];
}

std::string ResultSetMetaData::getColumnTypeName(int columnIndex) const {
    if (columnIndex < 1 || columnIndex > d_columnCount) {
        throw SQLException("Invalid column index: " + std::to_string(columnIndex));
    }
    return d_columnTypeNames[columnIndex - 1];
}

std::string ResultSetMetaData::getColumnDisplaySize(int columnIndex) const {
    if (columnIndex < 1 || columnIndex > d_columnCount) {
        throw SQLException("Invalid column index: " + std::to_string(columnIndex));
    }
    return std::to_string(d_columnDisplaySizes[columnIndex - 1]);
}

} // namespace simpledb::jdbc
