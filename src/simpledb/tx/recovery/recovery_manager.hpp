#pragma once

#include "simpledb/tx/transaction.hpp"
#include "simpledb/log/LogMgr.hpp"
#include "simpledb/buffer/BufferMgr.hpp"

#include <memory>
#include <string>

// TODO: Refactor to not need this
namespace simpledb::tx {
class Transaction;
}

namespace simpledb::tx::recovery {

class RecoveryManager {
public:
    RecoveryManager(simpledb::tx::Transaction* tx, int txnum,  std::shared_ptr<simpledb::log::LogMgr> lm, std::shared_ptr<simpledb::buffer::BufferMgr> bm);

    void commit();
    void rollback();
    void recover();

    int set_int(simpledb::buffer::Buffer &buf, int offset, int val);
    int set_string(simpledb::buffer::Buffer &buf, int offset, const std::string &val);

private:
    simpledb::tx::Transaction* d_tx;
    int d_txnum;
    std::shared_ptr<simpledb::log::LogMgr> d_lm;
    std::shared_ptr<simpledb::buffer::BufferMgr> d_bm;

    void do_rollback();
    void do_recover();
};
}
