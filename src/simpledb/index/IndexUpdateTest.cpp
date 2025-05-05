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
#include "simpledb/query/update_scan.hpp"

using namespace simpledb;

int main() {
    try {
        // Initialize the database
        auto db = std::make_unique<server::SimpleDB>("studentdb");
        auto tx = db->new_tx();
        auto mdm = db->metadata_manager();
        auto studentplan = std::make_unique<plan::TablePlan>(tx, "student", mdm);
        auto studentscan = std::dynamic_pointer_cast<query::UpdateScan>(studentplan->open());

        // Create a map containing all indexes for STUDENT
        std::unordered_map<std::string, std::shared_ptr<index::Index>> indexes;
        auto idxinfo = mdm->get_index_info("student", tx);
        for (auto& [fldname, info] : idxinfo) {
            indexes[fldname] = info.open();
        }

        // Task 1: insert a new STUDENT record for Sam
        // First, insert the record into STUDENT
        studentscan->insert();
        studentscan->set_int("sid", 11);
        studentscan->set_string("sname", "sam");
        studentscan->set_int("gradyear", 2023);
        studentscan->set_int("majorid", 30);

        // Then insert a record into each of the indexes
        auto datarid = studentscan->current_rid(); // FIXME: should current_rid() be renamed getRid()?
        for (const auto& [fldname, idx] : indexes) {
            auto dataval = studentscan->get_val(fldname);
            idx->insert(dataval, datarid);
        }

        // Task 2: find and delete Joe's record
        studentscan->before_first();
        while (studentscan->next()) {
            if (studentscan->get_string("sname") == "joe") {
                // First, delete the index records for Joe
                auto joeRid = studentscan->current_rid(); // FIXME: should current_rid() be renamed getRid()?
                for (const auto& [fldname, idx] : indexes) {
                    auto dataval = studentscan->get_val(fldname);
                    idx->delete_row(dataval, joeRid);
                }

                // Then delete Joe's record in STUDENT
                studentscan->delete_record();
                break;
            }
        }

        // Print the records to verify the updates
        studentscan->before_first();
        while (studentscan->next()) {
            std::cout << studentscan->get_string("sname") << " " 
                     << studentscan->get_int("sid") << std::endl;
        }
        studentscan->close();

        // Close all indexes
        for (const auto& [fldname, idx] : indexes) {
            idx->close();
        }
        tx->commit();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}