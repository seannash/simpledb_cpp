#include "simpledb/tx/recovery/recovery_manager.hpp"

namespace simpledb::tx::recovery {

RecoveryManager::RecoveryManager(
    std::shared_ptr<simpledb::tx::Transaction> tx,
    int txnum,
    std::shared_ptr<simpledb::log::LogManager> lm,
    std::shared_ptr<simpledb::buffer::BufferManager> bm)
    : d_tx(tx)
    , d_txnum(txnum)
    , d_lm(lm)
    , d_bm(bm)
{
}

void RecoveryManager::commit() {
}

void RecoveryManager::rollback() {
}

void RecoveryManager::recover() {
}

void RecoveryManager::set_int(simpledb::buffer::Buffer &buf, int offset, int val) {
}

void RecoveryManager::set_string(simpledb::buffer::Buffer &buf, int offset, const std::string &val) {
}

} // namespace simpledb::tx::recovery
