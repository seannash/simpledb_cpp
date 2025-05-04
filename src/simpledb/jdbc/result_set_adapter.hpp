#pragma once

#include "jdbc/result_set.hpp"
#include "jdbc/sql_exception.hpp"
#include <memory>
#include <string>

namespace simpledb::jdbc {

class ResultSetAdapter : public ::jdbc::ResultSet {
public:
    ResultSetAdapter() = default;
    virtual ~ResultSetAdapter() = default;

    // Navigation
    virtual bool next() override;

    // Data access
    virtual std::string getString(const std::string& columnLabel) override;
    virtual std::string getString(int columnIndex) override;
    virtual int getInt(const std::string& columnLabel) override;
    virtual int getInt(int columnIndex) override;

    // Resource management
    virtual void close() override;

    // Metadata
    virtual const ::jdbc::MetaData& getMetaData() override;
};

} // namespace simpledb::jdbc
