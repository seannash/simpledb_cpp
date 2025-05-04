#include "embedded_metadata.hpp"
#include "jdbc/sql_exception.hpp"
#include "jdbc/column_types.hpp"

#include <algorithm>

namespace simpledb::jdbc::embedded {

EmbeddedMetadata::EmbeddedMetadata(const record::Schema schema)
    : d_schema(schema) {
}

std::size_t EmbeddedMetadata::getColumnCount() const {
    return d_schema.fields().size();
}

std::string EmbeddedMetadata::getColumnName(int columnIndex) const {
    return d_schema.fields()[columnIndex - 1 ];
}

::jdbc::ColumnType EmbeddedMetadata::getColumnType(int columnIndex) const {
    std::string fldname = getColumnName(columnIndex);
    return d_schema.type(fldname);
}

std::string EmbeddedMetadata::getColumnTypeName(int columnIndex) const {
    if (columnIndex < 1 || columnIndex > static_cast<int>(d_columnTypeNames.size())) {
        throw ::jdbc::SQLException("Invalid column index");
    }
    return d_columnTypeNames[columnIndex - 1];
}

int EmbeddedMetadata::getColumnDisplaySize(int columnIndex) const {
    std::string fldname = getColumnName(columnIndex);
    auto fldtype = d_schema.get_type(fldname);
    auto fldlength = (fldtype == ::jdbc::ColumnType::INT) ? 6 : d_schema.get_length(fldname);
    return std::max(fldname.length(), fldlength);
}

} // namespace simpledb::jdbc::embedded 