#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/metadata/stat_info.hpp"
#include "simpledb/record/table_scan.hpp"

#include <string>
#include <memory>

namespace simpledb::plan {

class TablePlan: public Plan {
public:
    TablePlan(std::shared_ptr<simpledb::tx::Transaction> tx, std::string_view table_name, std::shared_ptr<simpledb::metadata::MetadataManager> metadata_manager);
    ~TablePlan();

    std::shared_ptr<simpledb::query::Scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(std::string_view field) override;
    record::Schema& schema() override;

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::string_view d_table_name;
    std::shared_ptr<record::Layout> d_layout;
    metadata::StatInfo d_stat_info;
};

} // namespace simpledb::plan