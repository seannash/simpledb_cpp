#pragma once

#include "jdbc/statement.hpp"
#include "jdbc/sql_exception.hpp"
#include <memory>
#include <string_view>

namespace simpledb::jdbc {

class StatementAdapter : public ::jdbc::Statement {
public:
    StatementAdapter() = default;
    virtual ~StatementAdapter() = default;

    virtual std::unique_ptr<::jdbc::ResultSet> executeQuery(std::string_view sql) override;
    virtual int executeUpdate(std::string_view sql) override;
    virtual void close() override;
};

} // namespace simpledb::jdbc