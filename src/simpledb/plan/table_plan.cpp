#include "simpledb/plan/table_plan.hpp"

namespace simpledb::plan {

TablePlan::TablePlan(std::shared_ptr<simpledb::tx::Transaction> tx, std::string_view table_name, std::shared_ptr<simpledb::metadata::MetadataManager> metadata_manager)
    : d_tx(tx)
    , d_table_name(table_name)
    , d_layout(metadata_manager->get_layout(table_name, tx))
    , d_stat_info(metadata_manager->get_stat_info(table_name, d_layout, tx))
{
}

TablePlan::~TablePlan() = default;

std::shared_ptr<simpledb::query::Scan> TablePlan::open() {
    return std::make_shared<simpledb::record::TableScan>(d_tx, d_table_name, d_layout);
}

int TablePlan::blocks_accessed() {
    return d_stat_info.blocks_accessed();
}

int TablePlan::records_output() {
    return d_stat_info.records_output();
}

int TablePlan::distinct_values(std::string_view field) {
    return d_stat_info.distinct_values(field);
}

record::Schema& TablePlan::schema() {
    return d_layout->schema();
}

} // namespace simpledb::plan 