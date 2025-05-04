#include "metadata_adapter.hpp"

namespace simpledb::jdbc {

std::size_t MetadataAdapter::getColumnCount() const {
    throw ::jdbc::SQLException("operation not implemented");
}

std::string MetadataAdapter::getColumnName(int columnIndex) const {
    throw ::jdbc::SQLException("operation not implemented");
}

::jdbc::ColumnType MetadataAdapter::getColumnType(int columnIndex) const {
    throw ::jdbc::SQLException("operation not implemented");
}

std::string MetadataAdapter::getColumnTypeName(int columnIndex) const {
    throw ::jdbc::SQLException("operation not implemented");
}

int MetadataAdapter::getColumnDisplaySize(int columnIndex) const {
    throw ::jdbc::SQLException("operation not implemented");
}

} // namespace simpledb::jdbc 