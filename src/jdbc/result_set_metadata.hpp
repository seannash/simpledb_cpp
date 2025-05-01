
#pragma once

#include <string>
#include <vector>

#include "column_types.h"

namespace simpledb::jdbc {  

class ResultSetMetaData {

    std::size_t d_columnCount = 0;
    std::vector<std::string> d_columnNames;
    std::vector<ColumnType> d_columnTypes;
    std::vector<std::string> d_columnTypeNames;
    std::vector<int> d_columnDisplaySizes;

public:
    ResultSetMetaData() = default;
    ResultSetMetaData(
        int columnCount,
        std::vector<std::string> columnNames,
        std::vector<ColumnType> columnTypes,
        std::vector<std::string> columnTypeNames,
        std::vector<int> columnDisplaySizes);

    ~ResultSetMetaData() = default;
    std::size_t getColumnCount() const;
    std::string getColumnName(int columnIndex) const;
    ColumnType getColumnType(int columnIndex) const;
    std::string getColumnTypeName(int columnIndex) const;
    int getColumnDisplaySize(int columnIndex) const;
};

} // namespace simpledb::jdbc
