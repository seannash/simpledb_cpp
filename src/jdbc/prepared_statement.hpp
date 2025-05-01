#pragma once

#include <string>
#include <memory>

#include "internal_connection.hpp"
#include "result_set.hpp"

namespace simpledb::jdbc {

class PreparedStatement {
public:
    void setString(int parameterIndex, const std::string& value);
    void setInt(int parameterIndex, int value);
    void setLong(int parameterIndex, long value);
    void setDouble(int parameterIndex, double value);
    void setBoolean(int parameterIndex, bool value);
    void setNull(int parameterIndex);

    void clearParameters();

    std::unique_ptr<ResultSet> executeQuery();
    int executeUpdate();

    void close();

    PreparedStatement(std::shared_ptr<InternalConnection> internalConnection, std::string_view sql)
    : d_internalConnection(internalConnection), d_sql(sql) {}

    friend class Connection;

private:
    std::shared_ptr<InternalConnection> d_internalConnection;
    std::string d_sql;
};

} // namespace simpledb::jdbc
