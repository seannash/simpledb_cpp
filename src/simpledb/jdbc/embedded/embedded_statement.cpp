
#include "embedded_statement.hpp"
#include "simpledb/jdbc/embedded/embedded_result_set.hpp"

namespace simpledb::jdbc::embedded {

EmbeddedStatement::EmbeddedStatement(EmbeddedConnection* conn, std::shared_ptr<plan::Planner> planner)
: d_conn(conn)
, d_planner(planner)
{}

std::unique_ptr<::jdbc::ResultSet> EmbeddedStatement::executeQuery(std::string_view sql) {
    try {
        auto tx = d_conn->get_transaction();
        // FIXME: Convert to string_view
        auto plan = d_planner->createQueryPlan(std::string(sql), tx);
        return std::make_unique<simpledb::jdbc::embedded::EmbeddedResultSet>(plan, d_conn);
    } catch (const std::exception& e) {
        d_conn->rollback();
        throw ::jdbc::SQLException(e.what());
    }
}

int EmbeddedStatement::executeUpdate(std::string_view sql) {
    try {
        auto tx = d_conn->get_transaction();
        // FIXME: Convert to string_view
        auto result = d_planner->executeUpdate(std::string(sql), tx);
        d_conn->commit();
        return result;
    } catch (const std::exception& e) {
        d_conn->rollback();
        throw ::jdbc::SQLException(e.what());
    }
}

void EmbeddedStatement::close() {
};

} // namespace simpledb::jdbc::embedded
