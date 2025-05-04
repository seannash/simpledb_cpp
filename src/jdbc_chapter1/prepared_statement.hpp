#pragma once

#include <string>
#include <memory>
#include <vector>

#include "internal_connection.hpp"
#include "result_set.hpp"

namespace simpledb::jdbc {

class PreparedStatement {
public:
    void setString(int parameterIndex, const std::string& value);
    void setInt(int parameterIndex, int value);

    void clearParameters();

    std::unique_ptr<ResultSet> executeQuery();
    int executeUpdate();

    void close();

    PreparedStatement(std::shared_ptr<InternalConnection> internalConnection, std::string_view sql);
    friend class Connection;

private:
    std::shared_ptr<InternalConnection> d_internalConnection;
    std::string d_sql;
    std::vector<std::string> d_tokens;
    std::vector<std::string> d_parameters;

    std::string buildSql() const;
};

} // namespace simpledb::jdbc
