
#pragma once

#include <string>
#include <vector>

#include "jdbc/metadata.hpp"
#include "jdbc/column_types.hpp"

namespace jdbc {  

class MetaData {
public:
    virtual ~MetaData() = default;

    virtual std::size_t getColumnCount() const = 0;
    virtual std::string getColumnName(int columnIndex) const = 0;
    virtual ColumnType getColumnType(int columnIndex) const = 0;
    virtual std::string getColumnTypeName(int columnIndex) const = 0;
    virtual int getColumnDisplaySize(int columnIndex) const = 0;
};

} // namespace jdbc
