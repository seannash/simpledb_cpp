#pragma once

#include "simpledb/tx/transaction.hpp"
#include "simpledb/log/log_manager.hpp"
#include "simpledb/buffer/buffer_manager.hpp"

#include <memory>
#include <string>

// TODO: Refactor to not need this
namespace simpledb::tx {
class Transaction;
}

namespace simpledb::tx::recovery {

class RecoveryManager {
public:
    RecoveryManager(simpledb::tx::Transaction* tx, int txnum,  std::shared_ptr<simpledb::log::LogManager> lm, std::shared_ptr<simpledb::buffer::BufferManager> bm);

    void commit();
    void rollback();
    void recover();

    int set_int(simpledb::buffer::Buffer &buf, int offset, int val);
    int set_string(simpledb::buffer::Buffer &buf, int offset, const std::string &val);

private:
    simpledb::tx::Transaction* d_tx;
    int d_txnum;
    std::shared_ptr<simpledb::log::LogManager> d_lm;
    std::shared_ptr<simpledb::buffer::BufferManager> d_bm;

    void do_rollback();
    void do_recover();
};
}
