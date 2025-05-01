#pragma once

#include "result_set.hpp"
#include "result_set_metadata.hpp"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace simpledb::jdbc {

class InMemoryResultSet : public ResultSet {
public:
    // Constructor takes column names and types
    InMemoryResultSet();
    InMemoryResultSet(std::vector<std::string> columnNames,
                     std::vector<std::string> columnTypes,
                     std::vector<int> columnSizes);

    // ResultSet interface implementation
    bool next() override;
    std::string getString(const std::string& columnLabel) override;
    int getInt(const std::string& columnLabel) override;
    void close() override;
    ResultSetMetaData getMetaData() override;

    // Methods to add rows
    void addRow(const std::vector<std::string>& values);
    void addRow(const std::vector<int>& values);
    void addRow(const std::unordered_map<std::string, std::string>& values);

private:
    std::vector<std::vector<std::string>> d_rows;
    std::size_t d_currentRow = 0;
    bool d_closed = false;
    std::unique_ptr<ResultSetMetaData> d_metadata;
    std::unordered_map<std::string, std::size_t> d_columnIndices;

    void validateColumnLabel(const std::string& columnLabel) const;
    void checkClosed() const;
};

} // namespace simpledb::jdbc
