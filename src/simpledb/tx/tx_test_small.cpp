#include "simpledb/server/simpledb.hpp"
#include "simpledb/tx/transaction.hpp" 
#include "simpledb/tx/recovery/log_record_builder.hpp"
#include <iostream>

int main() {
    simpledb::server::SimpleDB simpledb("testdb", 4096, 10);
    auto file_manager = simpledb.file_manager();
    auto log_manager = simpledb.log_manager();
    auto buffer_manager = simpledb.buffer_manager();


    auto tx2 = std::make_shared<simpledb::tx::Transaction>(file_manager, log_manager, buffer_manager);
    simpledb::file::BlockId blk = simpledb::file::BlockId("testfile", 1);
    tx2->pin(blk);
    int ival = tx2->get_int(blk, 80);
    std::string sval = tx2->get_string(blk, 40);
    std::cout << "TX1:Starting ival: " << ival << ", sval: " << sval << std::endl;
    int newival = ival + 1;
    std::string newsval = "new_string";
    std::string newsval2 = "new_string2";
    tx2->set_int(blk, 80, newival, true);
    tx2->set_string(blk, 40, newsval, true);
    tx2->set_string(blk, 40, newsval2, true);
    tx2->rollback();

    auto tx3 = std::make_shared<simpledb::tx::Transaction>(file_manager, log_manager, buffer_manager);
    blk = simpledb::file::BlockId("testfile", 1);
    tx2->pin(blk);
    ival = tx2->get_int(blk, 80);
    sval = tx2->get_string(blk, 40);
    std::cout << "TX1:Starting ival: " << ival << ", sval: " << sval << std::endl;
    tx3->commit();

    std::cout << "Printing log records" << std::endl;
    auto log_iterator = log_manager->iterator();
    while (log_iterator.hasNext()) {
        auto bytes = log_iterator.next();
        auto record = simpledb::tx::recovery::LogRecordBuilder::build(bytes);
        std::cout << "Record: " << record->to_string() << std::endl;
    }

    return 0;
}