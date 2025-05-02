#include "simpledb/server/simpledb.hpp"
#include "simpledb/metadata/table_manager.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/table_scan.hpp"
using namespace simpledb::metadata;
using namespace simpledb::record;
using namespace simpledb::server;

int main () {
    SimpleDB simpledb("catalogtest", 400, 8);
    auto tx = simpledb.new_tx();
    TableManager tm(true, tx);

    auto sch = std::make_shared<Schema>();
    sch->add_int_field("A");
    sch->add_string_field("B", 9);
    tm.create_table("MyTable", sch, tx);

    std::cout << "All tables have theur lengths: " << std::endl;
    auto layout = tm.get_layout("tblcat", tx);
    TableScan ts(tx, "tblcat", layout);
    while (ts.next()) {
        std::cout << ts.get_string("tblname") << " " << ts.get_int("length") << std::endl;
    }
    ts.close();

    std::cout << "All fields have their offsets: " << std::endl;
    layout = tm.get_layout("fldcat", tx);
    ts = TableScan(tx, "fldcat", layout);
    while (ts.next()) {
        std::cout << ts.get_string("tblname") << " " << ts.get_int("fldname") << " " << ts.get_int("offset") << std::endl;
    }
    ts.close();
}