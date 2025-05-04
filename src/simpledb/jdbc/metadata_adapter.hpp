#pragma once

#include "jdbc/metadata.hpp"
#include "jdbc/sql_exception.hpp"
#include <memory>
#include <string>
#include <vector>

namespace simpledb::jdbc {

class MetadataAdapter : public ::jdbc::MetaData {
public:
    MetadataAdapter() = default;
    virtual ~MetadataAdapter() = default;

    // Column information
    std::size_t getColumnCount() const override;
    std::string getColumnName(int columnIndex) const override;
    ::jdbc::ColumnType getColumnType(int columnIndex) const override;
    std::string getColumnTypeName(int columnIndex) const override;
    int getColumnDisplaySize(int columnIndex) const override;

};

} // namespace simpledb::jdbc
