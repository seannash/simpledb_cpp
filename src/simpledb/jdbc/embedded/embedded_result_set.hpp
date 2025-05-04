#pragma once

#include "simpledb/jdbc/result_set_adapter.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/plan/plan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/jdbc/embedded/embedded_connection.hpp"
#include "jdbc/metadata.hpp"
#include <memory>
#include <string>

namespace simpledb::jdbc::embedded {

class EmbeddedResultSet : public simpledb::jdbc::ResultSetAdapter {
public:
    // FIXME: Convert to unique_ptr
    explicit EmbeddedResultSet(std::shared_ptr<plan::Plan> plan, EmbeddedConnection* conn);
    ~EmbeddedResultSet() = default;

    bool next() override;
    std::string getString(const std::string& columnLabel) override;
    int getInt(const std::string& columnLabel) override;
    void close() override;
    const ::jdbc::MetaData& getMetaData() override;

private:
    std::shared_ptr<plan::Plan> d_plan; // FIXME: Only need Schema to be returned as something with a longer lifetime
    std::shared_ptr<query::Scan> d_scan;
    record::Schema& d_schema;
    simpledb::jdbc::embedded::EmbeddedConnection *d_conn;
    std::shared_ptr<::jdbc::MetaData> d_meta_data;
};

} // namespace simpledb::jdbc::embedded
