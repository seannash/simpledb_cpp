#pragma once

#include "simpledb/jdbc/statement_adapter.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/jdbc/embedded/embedded_connection.hpp"
#include "simpledb/plan/planner.hpp"
#include "simpledb/jdbc/embedded/embedded_result_set.hpp"
#include <memory>
#include <string_view>

namespace simpledb::jdbc::embedded {

class EmbeddedStatement : public simpledb::jdbc::StatementAdapter {
public:
    explicit EmbeddedStatement(EmbeddedConnection* conn, std::shared_ptr<plan::Planner> planner);
    ~EmbeddedStatement() = default;

    std::unique_ptr<::jdbc::ResultSet> executeQuery(std::string_view sql) override;
    int executeUpdate(std::string_view sql) override;
    void close() override;

private:
    EmbeddedConnection* d_conn;
    std::shared_ptr<plan::Planner> d_planner;
};

} // namespace simpledb::jdbc::embedded
