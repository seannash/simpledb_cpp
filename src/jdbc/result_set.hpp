#pragma once

#include <memory>
#include <string>

#include "result_set_metadata.hpp"

namespace simpledb::jdbc {


class ResultSet {
public:

    ~ResultSet() = default;
    virtual bool next() = 0;
    virtual std::string getString(const std::string& columnLabel) = 0;
    virtual int getInt(const std::string& columnLabel) = 0;
    virtual void close() = 0;
    virtual ResultSetMetaData getMetaData() = 0;
};

} // namespace simpledb::jdbc 