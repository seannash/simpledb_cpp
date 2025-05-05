#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "simpledb/index/Index.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/plan/table_plan.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/record/rid.hpp"
#include "simpledb/server/simpledb.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/query/update_scan.hpp"

using namespace simpledb;

int main() {
    try {
        // Initialize the database
        auto db = std::make_unique<server::SimpleDB>("studentdb");
        auto tx = db->new_tx();
        auto mdm = db->metadata_manager();

        // Open a scan on the data table
        auto studentplan = std::make_unique<plan::TablePlan>(tx, "student", mdm);
        auto studentscan = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(studentplan->open());

        // Open the index on MajorId
        auto indexes = mdm->get_index_info("student", tx);
        auto ii = indexes.at("majorid");
        auto idx = ii.open();

        // Retrieve all index records having a dataval of 20
        auto searchVal = std::make_unique<query::Constant>(20);
        idx->beforeFirst(simpledb::query::Constant(20));
        while (idx->next()) {
            // Use the datarid to go to the corresponding STUDENT record
            auto datarid = idx->getDataRid();
            studentscan->move_to_rid(datarid);
            std::cout << studentscan->get_string("sname") << std::endl;
        }

        // Close the index and the data table
        idx->close();
        studentscan->close();
        tx->commit();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}