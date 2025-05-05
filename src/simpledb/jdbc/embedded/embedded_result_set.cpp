#include "embedded_result_set.hpp"
#include "jdbc/sql_exception.hpp"
#include "embedded_metadata.hpp"

namespace simpledb::jdbc::embedded {

EmbeddedResultSet::EmbeddedResultSet(
    std::shared_ptr<plan::Plan> plan,
    EmbeddedConnection* conn)
    : d_plan(plan)
    , d_scan(plan->open())
    , d_schema(plan->schema())
    , d_conn(conn)
    , d_meta_data(std::make_shared<EmbeddedMetadata>(plan->schema())) {
}

bool EmbeddedResultSet::next() {
    try {
        return d_scan->next();
    } catch (const std::exception& e) {
        d_conn->rollback();
        throw ::jdbc::SQLException(e.what());
    }
}

std::string EmbeddedResultSet::getString(const std::string& fldname) {
    try {
        return d_scan->get_string(fldname);
    } catch (const std::exception& e) {
        d_conn->rollback();
        throw ::jdbc::SQLException(e.what());
    }
}

int EmbeddedResultSet::getInt(const std::string& fldname) {
    try {
        return d_scan->get_int(fldname);
    } catch (const std::exception& e) {
        d_conn->rollback();
        throw ::jdbc::SQLException(e.what());
    }
}

void EmbeddedResultSet::close() {
    d_scan->close();
    d_conn->close();
}

const ::jdbc::MetaData& EmbeddedResultSet::getMetaData() {
    return *d_meta_data;
}

} // namespace simpledb::jdbc::embedded 