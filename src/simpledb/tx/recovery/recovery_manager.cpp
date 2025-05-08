#include "simpledb/tx/recovery/recovery_manager.hpp"

#include <algorithm>

#include "simpledb/tx/recovery/start_log_record.hpp"
#include "simpledb/tx/recovery/commit_log_record.hpp"
#include "simpledb/tx/recovery/rollback_log_record.hpp"
#include "simpledb/tx/recovery/set_int_log_record.hpp"
#include "simpledb/tx/recovery/set_string_log_record.hpp"
#include "simpledb/tx/recovery/checkpoint_log_record.hpp"
#include "simpledb/buffer/Buffer.hpp"
#include "simpledb/file/BlockId.hpp"
#include "simpledb/log/LogIterator.hpp"
#include "simpledb/tx/recovery/log_record_builder.hpp"

namespace simpledb::tx::recovery {

RecoveryManager::RecoveryManager(
    Transaction* tx,
    int txnum,
    std::shared_ptr<simpledb::log::LogMgr> lm,
    std::shared_ptr<simpledb::buffer::BufferMgr> bm)
    : d_tx(tx)
    , d_txnum(txnum)
    , d_lm(lm)
    , d_bm(bm)
{
    int lsn =StartLogRecord::write_to_log(lm, txnum);
    d_lm->flush(lsn);
}

void RecoveryManager::commit() {
    d_bm->flushAll(d_txnum);
    int lsn = CommitLogRecord::write_to_log(d_lm, d_txnum);
    d_lm->flush(lsn);
}

void RecoveryManager::rollback() {
    do_rollback();
    d_bm->flushAll(d_txnum);
    int lsn = RollbackLogRecord::write_to_log(d_lm, d_txnum);
    d_lm->flush(lsn);
}

void RecoveryManager::recover() {
    do_recover();
    d_bm->flushAll(d_txnum);
    int lsn = CheckpointLogRecord::write_to_log(d_lm, d_txnum);
    d_lm->flush(lsn);
}

int RecoveryManager::set_int(simpledb::buffer::Buffer &buf, int offset, int val) {
    int oldval = buf.contents().getInt(offset);
    simpledb::file::BlockId blk = buf.block();
    return SetIntLogRecord::write_to_log(d_lm, d_txnum, blk, offset, oldval);
}

int RecoveryManager::set_string(simpledb::buffer::Buffer &buf, int offset, const std::string &val) {
    std::string oldval = buf.contents().getString(offset);
    simpledb::file::BlockId blk = buf.block();
    return SetStringLogRecord::write_to_log(d_lm, d_txnum, blk, offset, oldval);
}

void RecoveryManager::do_rollback() {
    simpledb::log::LogIterator it = d_lm->iterator();
    while (it.hasNext()) {
        std::vector<char>  record = it.next();
        auto rec = LogRecordBuilder::build(record);
        if (rec->txnum() == d_txnum) {
            if (rec->op() == LogRecord::START) 
                break;
            rec->undo(*d_tx);
        }
    }
}

void RecoveryManager::do_recover() {
    std::vector<char> finnished_txs{};
    simpledb::log::LogIterator it = d_lm->iterator();
    while (it.hasNext()) {
        std::vector<char>  bytes = it.next();
        auto rec = LogRecordBuilder::build(bytes);
        if (rec->op() == LogRecord::CHECKPOINT)
            return;
        if (rec->op() == LogRecord::COMMIT || rec->op() == LogRecord::ROLLBACK) {
            finnished_txs.push_back(rec->txnum());
        } else if (std::find(finnished_txs.begin(), finnished_txs.end(), rec->txnum()) == finnished_txs.end()) {
            rec->undo(*d_tx);
        }
    }
}

} // namespace simpledb::tx::recovery
