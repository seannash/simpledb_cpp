#include "simpledb/plan/basic_update_planner.hpp"
#include "simpledb/plan/table_plan.hpp"
#include "simpledb/plan/select_plan.hpp"
#include "simpledb/query/select_scan.hpp"
#include "simpledb/query/update_scan.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/metadata/metadata_manager.hpp"

namespace simpledb::plan {

BasicUpdatePlanner::BasicUpdatePlanner(std::shared_ptr<metadata::MetadataManager> metadata_manager)
    : d_metadata_manager(metadata_manager)
{
}

int BasicUpdatePlanner::executeInsert(const parse::InsertData& data, std::shared_ptr<tx::Transaction> tx) {
    auto p = std::make_shared<TablePlan>(tx, data.table_name(), d_metadata_manager);
    auto us = std::dynamic_pointer_cast<query::UpdateScan>(p->open());
    us->insert();
    auto iter = data.values().begin();
    for (const auto& field : data.fields()) {
        auto val = *iter++;
        us->set_val(field, val);
    }
    us->close();
    return 1;
}

int BasicUpdatePlanner::executeDelete(const parse::DeleteData& data, std::shared_ptr<tx::Transaction> tx) {
    std::shared_ptr<Plan> p = std::make_shared<TablePlan>(tx, data.table_name(), d_metadata_manager);
    p = std::make_shared<SelectPlan>(p, data.predicate());
    auto us = std::dynamic_pointer_cast<query::UpdateScan>(p->open());
    int count = 0;
    while (us->next()) {
        us->delete_record();
        count++;
    }
    us->close();
    return count;
}

int BasicUpdatePlanner::executeModify(const parse::ModifyData& data, std::shared_ptr<tx::Transaction> tx) {
    std::shared_ptr<Plan> p = std::make_shared<TablePlan>(tx, data.table_name(), d_metadata_manager);
    p = std::make_shared<SelectPlan>(p, data.predicate());
    auto us = std::dynamic_pointer_cast<query::UpdateScan>(p->open());
    int count = 0;
    while (us->next()) {
        auto val = data.new_value()->evaluate(*us);
        us->set_val(data.target_field(), val);
        count++;
    }
    us->close();
    return count;
}

int BasicUpdatePlanner::executeCreateTable(const parse::CreateTableData& data, std::shared_ptr<tx::Transaction> tx) {
    // FIXME Change create_table to take a reference
    auto schema_ptr = std::make_shared<simpledb::record::Schema>(data.new_schema());
    d_metadata_manager->create_table(data.table_name(), schema_ptr, tx);
    return 1;
}

int BasicUpdatePlanner::executeCreateView(const parse::CreateViewData& data, std::shared_ptr<tx::Transaction> tx) {
    // TODO: Why is data.view_def a string?
    d_metadata_manager->create_view(data.view_name(), data.view_def(), tx);
    return 1;
}

int BasicUpdatePlanner::executeCreateIndex(const parse::CreateIndexData& data, std::shared_ptr<tx::Transaction> tx) {
    d_metadata_manager->create_index(data.index_name(), data.table_name(), data.field_name(), tx);
    return 1;
}

} // namespace simpledb::plan 