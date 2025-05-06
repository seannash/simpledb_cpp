#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "simpledb/server/simpledb.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/plan/table_plan.hpp"
#include "simpledb/index/hash/HashIndex.hpp"
#include "simpledb/index/planner/IndexSelectPlan.hpp"
#include "simpledb/record/table_scan.hpp"

// Find the grades of student 6.
void use_index_manually(std::shared_ptr<simpledb::metadata::IndexInfo> ii,
                       std::shared_ptr<simpledb::plan::Plan> p,
                       const simpledb::query::Constant& c)
{
    // Open a scan on the table.
    auto s = std::dynamic_pointer_cast<simpledb::record::TableScan>(p->open());  // must be a table scan
    auto idx = ii->open();

    // Retrieve all index records having the specified dataval.
    idx->beforeFirst(c);
    while (idx->next()) {
        // Use the datarid to go to the corresponding Enroll record.
        auto datarid = idx->getDataRid();
        s->move_to_rid(datarid);  // table scans can move to a specified RID.
        std::cout << s->get_string("grade") << std::endl;
    }
    idx->close();
    s->close();
}

void use_index_scan(std::shared_ptr<simpledb::metadata::IndexInfo> ii,
                   std::shared_ptr<simpledb::plan::Plan> p,
                   const simpledb::query::Constant& c)
{
    // Open an index select scan on the enroll table.
    auto idxplan = std::make_shared<simpledb::index::planner::IndexSelectPlan>(p, ii, c);
    auto s = idxplan->open();
    
    while (s->next()) {
        std::cout << s->get_string("grade") << std::endl;
    }
    s->close();
}

int main()
{
    auto db = std::make_shared<simpledb::server::SimpleDB>("studentdb");
    auto mdm = db->metadata_manager();
    auto tx = db->new_tx();

    // Find the index on StudentId.
    auto indexes = mdm->get_index_info("enroll", tx);
    for (const auto& [key, value] : indexes) {
        std::cout << "Index: " <<key << std::endl;
    }
    auto sid_idx = indexes.at("studentid");
    // FIXME: Making shared_ptr
    auto sid_idx_ptr = std::make_shared<simpledb::metadata::IndexInfo>(sid_idx);
    // Get the plan for the Enroll table
    auto enrollplan = std::make_shared<simpledb::plan::TablePlan>(tx, "enroll", mdm);
    
    // Create the selection constant
    simpledb::query::Constant c(6);
    
    // Two different ways to use the index in simpledb:
    use_index_manually(sid_idx_ptr, enrollplan, c);       
    use_index_scan(sid_idx_ptr, enrollplan, c);
    
    tx->commit();
    return 0;
}