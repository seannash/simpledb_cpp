#pragma once

#include <memory>
#include <string_view>
#include "internal_connection.hpp"
#include "result_set.hpp"

namespace simpledb::jdbc {

class Statement {
public:
    Statement(std::shared_ptr<InternalConnection> internalConnection);

    ~Statement() = default;
    std::unique_ptr<ResultSet> executeQuery(std::string_view sql);
    int executeUpdate(std::string_view sql);
    void close();

private:
    std::shared_ptr<InternalConnection> d_internalConnection;
};

} // namespace simpledb::jdbc 