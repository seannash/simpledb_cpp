#include "simpledb/server/simpledb.hpp"
#include "simpledb/tx/transaction.hpp" 
#include "simpledb/tx/recovery/log_record_builder.hpp"
#include <iostream>

void print_log_records(std::shared_ptr<simpledb::log::LogManager> lm) {
    std::cout << "Printing log records" << std::endl;
    auto log_iterator = lm->iterator();
    while (log_iterator.hasNext()) {
        auto bytes = log_iterator.next();
        auto record = simpledb::tx::recovery::LogRecordBuilder::build(bytes);
        std::cout << "Record: " << record->to_string() << std::endl;
    }
    std::cout << "Done printing log records" << std::endl;
}
int main() {
    simpledb::server::SimpleDB simpledb("testdb", 4096, 10);
    auto file_manager = simpledb.file_manager();
    auto log_manager = simpledb.log_manager();
    auto buffer_manager = simpledb.buffer_manager();
    
    auto tx1 = std::make_shared<simpledb::tx::Transaction>(file_manager, log_manager, buffer_manager);
    simpledb::file::BlockId blk = simpledb::file::BlockId("testfile", 1);
    tx1->pin(blk);
    // Don't log initial values
    tx1->set_int(blk, 80, 1, false);
    tx1->set_string(blk, 40, "one", false);
    tx1->commit();
    //print_log_records(log_manager);

    auto tx2 = std::make_shared<simpledb::tx::Transaction>(file_manager, log_manager, buffer_manager);
    tx2->pin(blk);
    int ival = tx2->get_int(blk, 80);
    std::string sval = tx2->get_string(blk, 40);
    std::cout << "TX1:Starting ival: " << ival << ", sval: " << sval << std::endl;
    int newival = ival + 1;
    std::string newsval = sval + "!";
    print_log_records(log_manager);
    tx2->set_int(blk, 80, newival, true);
    print_log_records(log_manager);
    tx2->set_string(blk, 40, newsval, true);
    tx2->commit();

    auto tx3 = std::make_shared<simpledb::tx::Transaction>(file_manager, log_manager, buffer_manager);
    tx3->pin(blk);
    ival = tx3->get_int(blk, 80);
    sval = tx3->get_string(blk, 40);
    std::cout << "Starting TX2: ival: " << ival << ", sval: " << sval << std::endl;
    tx3->set_int(blk, 80, 9999, true);
    ival = tx3->get_int(blk, 80);
    std::cout << "pre-rollback ival: " << ival << std::endl;
    tx3->rollback();

    auto tx4 = std::make_shared<simpledb::tx::Transaction>(file_manager, log_manager, buffer_manager);
    tx4->pin(blk);
    ival = tx4->get_int(blk, 80);
    sval = tx4->get_string(blk, 40);
    std::cout << "Starting TX3: ival: " << ival << ", sval: " << sval << std::endl;
    tx4->commit();

    return 0;
}