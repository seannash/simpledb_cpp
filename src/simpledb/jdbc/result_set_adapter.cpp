#include "result_set_adapter.hpp"

namespace simpledb::jdbc {

bool ResultSetAdapter::next() {
    throw ::jdbc::SQLException("operation not implemented");
}

std::string ResultSetAdapter::getString(const std::string& columnLabel) {
    throw ::jdbc::SQLException("operation not implemented");
}

std::string ResultSetAdapter::getString(int columnIndex) {
    throw ::jdbc::SQLException("operation not implemented");
}

int ResultSetAdapter::getInt(const std::string& columnLabel) {
    throw ::jdbc::SQLException("operation not implemented");
}

int ResultSetAdapter::getInt(int columnIndex) {
    throw ::jdbc::SQLException("operation not implemented");
}

void ResultSetAdapter::close() {
    throw ::jdbc::SQLException("operation not implemented");
}

const ::jdbc::MetaData& ResultSetAdapter::getMetaData() {
    throw ::jdbc::SQLException("operation not implemented");
}

} // namespace simpledb::jdbc 