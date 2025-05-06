#include "simpledb/index/planner/IndexUpdatePlanner.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/query/update_scan.hpp"
#include "simpledb/parse/parser.hpp"
#include "simpledb/plan/table_plan.hpp"
#include "simpledb/plan/select_plan.hpp"
#include "simpledb/index/Index.hpp"

namespace simpledb::index::planner {

IndexUpdatePlanner::IndexUpdatePlanner(std::shared_ptr<simpledb::metadata::MetadataManager> mdm)
    : d_mdm(std::move(mdm))
{
}

int IndexUpdatePlanner::executeInsert(const simpledb::parse::InsertData& data, 
                                     std::shared_ptr<simpledb::tx::Transaction> tx)
{
    auto tblname = data.table_name();
    auto p = std::make_shared<simpledb::plan::TablePlan>(tx, tblname, d_mdm);
    
    // first, insert the record
    auto s = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(p->open());
    s->insert();
    auto rid = s->current_rid();
    
    // then modify each field, inserting an index record if appropriate
    auto indexes = d_mdm->get_index_info(tblname, tx);
    auto val_iter = data.values().begin();
    for (const auto& fldname : data.fields()) {
        auto val = *val_iter++;
        s->set_val(fldname, val);
        
        auto ii = indexes.find(fldname);
        if (ii != indexes.end()) {
            auto idx = ii->second.open();
            idx->insert(val, rid);
            idx->close();
        }
    }
    s->close();
    return 1;
}

int IndexUpdatePlanner::executeDelete(const simpledb::parse::DeleteData& data,
                                     std::shared_ptr<simpledb::tx::Transaction> tx)
{
    auto tblname = data.table_name();
    std::shared_ptr<simpledb::plan::Plan> p = std::make_shared<simpledb::plan::TablePlan>(tx, tblname, d_mdm);
    p = std::make_shared<simpledb::plan::SelectPlan>(p, data.predicate());
    auto indexes = d_mdm->get_index_info(tblname, tx);
    
    auto s = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(p->open());
    int count = 0;
    while (s->next()) {
        // first, delete the record's RID from every index
        auto rid = s->current_rid();
        for (auto& [fldname, ii] : indexes) {
            auto val = s->get_val(fldname);
            auto idx = ii.open();
            idx->delete_row(val, rid);
            idx->close();
        }
        // then delete the record
        s->delete_record();
        count++;
    }
    s->close();
    return count;
}

int IndexUpdatePlanner::executeModify(const simpledb::parse::ModifyData& data,
                                     std::shared_ptr<simpledb::tx::Transaction> tx)
{
    auto tblname = data.table_name();
    auto fldname = data.target_field();
    std::shared_ptr<simpledb::plan::Plan> p = std::make_shared<simpledb::plan::TablePlan>(tx, tblname, d_mdm);
    p = std::make_shared<simpledb::plan::SelectPlan>(p, data.predicate());
    
    auto indexes = d_mdm->get_index_info(tblname, tx);
    auto ii = indexes.find(fldname);
    std::shared_ptr<simpledb::index::Index> idx = (ii == indexes.end()) ? nullptr : ii->second.open();
    
    auto s = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(p->open());
    int count = 0;
    while (s->next()) {
        // first, update the record
        auto newval = data.new_value()->evaluate(*s);
        auto oldval = s->get_val(fldname);
        s->set_val(data.target_field(), newval);
        
        // then update the appropriate index, if it exists
        if (idx) {
            auto rid = s->current_rid();
            idx->delete_row(oldval, rid);
            idx->insert(newval, rid);
        }
        count++;
    }
    if (idx) idx->close();
    s->close();
    return count;
}

int IndexUpdatePlanner::executeCreateTable(const simpledb::parse::CreateTableData& data,
                                           std::shared_ptr<simpledb::tx::Transaction> tx)
{
    // FIXME: Useless conversion
    std::shared_ptr<simpledb::record::Schema> sch = std::make_shared<simpledb::record::Schema>(data.new_schema());
    d_mdm->create_table(data.table_name(), sch, tx);
    return 0;
}

int IndexUpdatePlanner::executeCreateView(const simpledb::parse::CreateViewData& data,
                                          std::shared_ptr<simpledb::tx::Transaction> tx)
{
    d_mdm->create_view(data.view_name(), data.view_def(), tx);
    return 0;
}

int IndexUpdatePlanner::executeCreateIndex(const simpledb::parse::CreateIndexData& data,
                                          std::shared_ptr<simpledb::tx::Transaction> tx)
{
    d_mdm->create_index(data.index_name(), data.table_name(), data.field_name(), tx);
    return 0;
}

} // namespace simpledb::index::planner