#pragma once

#include <memory>
#include <string>

namespace simpledb::jdbc {

class ResultSetMetaData {
public:
    virtual ~ResultSetMetaData() = default;
    virtual int getColumnCount() = 0;
    virtual std::string getColumnName(int columnIndex) = 0;
    virtual std::string getColumnTypeName(int columnIndex) = 0;
    virtual std::string getColumnDisplaySize(int columnIndex) = 0;
};

class ResultSet {
public:
    virtual ~ResultSet() = default;
    virtual bool next() = 0;
    virtual std::string getString(const std::string& columnLabel) = 0;
    virtual int getInt(const std::string& columnLabel) = 0;
    virtual void close() = 0;
    virtual ResultSetMetaData getMetaData() = 0;
};

} // namespace simpledb::jdbc 