#include "simpledb/server/simpledb.hpp"
#include "simpledb/metadata/table_manager.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"
#include <iostream>

int main() {
    simpledb::server::SimpleDB simpledb("tblmgrtest", 400, 8);
    auto tx = simpledb.new_tx();
    simpledb::metadata::TableManager tm(true, tx);

    auto sch = std::make_shared<simpledb::record::Schema>();
    sch->add_int_field("A");
    sch->add_string_field("B", 9);
    tm.create_table("MyTable", sch, tx);

    auto layout = tm.get_layout("MyTable", tx);
    int size = layout->slot_size();
    auto sch2 = layout->schema();
    std::cout << "MyTable has slot size " << size << std::endl;
    std::cout << "Its fields are:" << std::endl;
    
    for (const auto& fldname : sch2.fields()) {
        std::string type;
        if (sch2.get_type(fldname) == simpledb::record::FieldType::INT) {
            type = "int";
        } else {
            int strlen = sch2.length(fldname);
            type = "varchar(" + std::to_string(strlen) + ")";
        }
        std::cout << fldname << ": " << type << std::endl;
    }
    
    tx->commit();
    return 0;
}
