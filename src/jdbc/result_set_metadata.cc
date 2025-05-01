#include "result_set_metadata.hpp"
#include "sql_exception.hpp"
#include <stdexcept>

namespace simpledb::jdbc {

class DefaultResultSetMetaData : public ResultSetMetaData {
public:
    DefaultResultSetMetaData() = default;
    ~DefaultResultSetMetaData() override = default;

    int getColumnCount() const override {
        return 0;  // Default implementation returns no columns
    }

    std::string getColumnName(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return "column" + std::to_string(columnIndex);
    }

    std::string getColumnTypeName(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return "VARCHAR";  // Default type
    }

    std::string getColumnDisplaySize(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return "255";  // Default display size
    }

    bool isNullable(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return true;  // Default to nullable
    }

    bool isAutoIncrement(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return false;  // Default to not auto-increment
    }

    bool isCaseSensitive(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return true;  // Default to case sensitive
    }

    bool isSearchable(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return true;  // Default to searchable
    }

    bool isSigned(int columnIndex) const override {
        if (columnIndex < 1) {
            throw SQLException("Invalid column index: " + std::to_string(columnIndex));
        }
        return false;  // Default to unsigned
    }
};

} // namespace simpledb::jdbc
