#include "simpledb/server/simpledb.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/table_scan.hpp"
#include <iostream>
#include <random>

int main() {
    simpledb::server::SimpleDB simpledb("tabletest", 400, 8);
    auto tx = simpledb.new_tx();

    simpledb::record::Schema sch {};
    sch.add_int_field("A");
    sch.add_string_field("B", 9);
    std::shared_ptr<simpledb::record::Layout> layout = std::make_shared<simpledb::record::Layout>(sch);
    for (const auto& fldname : layout->schema().fields()) {
        int offset = layout->offset(fldname);
        std::cout << fldname << " has offset " << offset << std::endl;
    }

    std::cout << "Filling the table with 50 random records." << std::endl;
    simpledb::record::TableScan ts(tx, "T", layout);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 50);

    for (int i = 0; i < 50; i++) {
        ts.insert();
        int n = dis(gen);
        ts.set_int("A", n);
        ts.set_string("B", "rec" + std::to_string(n));
        std::cout << "inserting into slot " << ts.current_rid().block_number() 
                  << "," << ts.current_rid().slot() << ": {" << n << ", rec" 
                  << n << "}" << std::endl;
    }

    std::cout << "Deleting these records, whose A-values are less than 25." << std::endl;
    int count = 0;
    ts.before_first();
    while (ts.next()) {
        int a = ts.get_int("A");
        std::string b = ts.get_string("B");
        if (a < 25) {
            count++;
            std::cout << "slot " << ts.current_rid().block_number() << "," 
                      << ts.current_rid().slot() << ": {" << a << ", " << b 
                      << "}" << std::endl;
            ts.delete_record();
        }
    }
    std::cout << count << " values under 25 were deleted." << std::endl << std::endl;

    std::cout << "Here are the remaining records." << std::endl;
    ts.before_first();
    while (ts.next()) {
        int a = ts.get_int("A");
        std::string b = ts.get_string("B");
        std::cout << "slot " << ts.current_rid().block_number() << "," 
                  << ts.current_rid().slot() << ": {" << a << ", " << b 
                  << "}" << std::endl;
    }
    ts.close();
    tx->commit();
    
    return 0;
}