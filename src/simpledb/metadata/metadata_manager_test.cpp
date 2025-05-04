#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/server/simpledb.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/tx/transaction.hpp"

#include <iostream>
#include <random>
#include <map>

using namespace simpledb::server;
using namespace simpledb::metadata;
using namespace simpledb::record;
using namespace simpledb::tx;

int main() {
    SimpleDB db("metadatamgrtest", 400, 8);
    auto mdm = db.metadata_manager();
    auto tx = db.new_tx();

    auto sch = std::make_shared<Schema>();
    sch->add_int_field("A");
    sch->add_string_field("B", 9);

    // Part 1: Table Metadata
    mdm->create_table("MyTable", sch, tx);
    auto layout = mdm->get_layout("MyTable", tx);
    int size = layout->slot_size();
    auto& sch2 = layout->schema();
    std::cout << "MyTable has slot size " << size << std::endl;
    std::cout << "Its fields are:" << std::endl;
    for (const auto& fldname : sch2.fields()) {
        std::string type;
        if (sch2.get_type(fldname) == ::jdbc::ColumnType::INT)
            type = "int";
        else {
            int strlen = sch2.get_length(fldname);
            type = "varchar(" + std::to_string(strlen) + ")";
        }
        std::cout << fldname << ": " << type << std::endl;
    }

    // Part 2: Statistics Metadata
    TableScan ts(tx, "MyTable", layout);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 50);
    
    for (int i = 0; i < 50; i++) {
        ts.insert();
        int n = dis(gen);
        ts.set_int("A", n);
        ts.set_string("B", "rec" + std::to_string(n));
    }
        
    auto si = mdm->get_stat_info("MyTable", layout, tx);
    std::cout << "B(MyTable) = " << si.blocks_accessed() << std::endl;
    std::cout << "R(MyTable) = " << si.records_output() << std::endl;
    std::cout << "V(MyTable,A) = " << si.distinct_values("A") << std::endl;
    std::cout << "V(MyTable,B) = " << si.distinct_values("B") << std::endl;

    // Part 3: View Metadata
    std::string viewdef = "select B from MyTable where A = 1";
    mdm->create_view("viewA", viewdef, tx);
    std::string v = mdm->get_view_def("viewA", tx);
    std::cout << "View def = " << v << std::endl;

    // Part 4: Index Metadata
    mdm->create_index("indexA", "MyTable", "A", tx);
    mdm->create_index("indexB", "MyTable", "B", tx);
 
    auto idxmap = mdm->get_index_info("MyTable", tx);

    auto ii = idxmap.at("A");

    std::cout << "B(indexA) = " << ii.blocks_accessed() << std::endl;
    std::cout << "R(indexA) = " << ii.records_output() << std::endl;
    std::cout << "V(indexA,A) = " << ii.distinct_values("A") << std::endl;
    std::cout << "V(indexA,B) = " << ii.distinct_values("B") << std::endl;

    ii = idxmap.at("B");
    std::cout << "B(indexB) = " << ii.blocks_accessed() << std::endl;
    std::cout << "R(indexB) = " << ii.records_output() << std::endl;
    std::cout << "V(indexB,A) = " << ii.distinct_values("A") << std::endl;
    std::cout << "V(indexB,B) = " << ii.distinct_values("B") << std::endl;

    tx->commit();

    return 0;
}
