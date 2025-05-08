#include "simpledb/tx/transaction.hpp"
#include <sstream>
namespace simpledb::tx {

Transaction::Transaction(std::shared_ptr<simpledb::file::FileMgr> file_manager,
                       std::shared_ptr<simpledb::log::LogMgr> log_manager,
                       std::shared_ptr<simpledb::buffer::BufferMgr> buffer_manager)
    : d_fileMgr(file_manager)
    , d_logMgr(log_manager)
    , d_bufferMgr(buffer_manager)
    , d_txnum(0)
{
    d_txnum = next_txnum();
    d_concurrency_manager = std::make_shared<concurrency::ConcurrencyManager>();
    d_recovery_manager = std::make_shared<recovery::RecoveryManager>(this, d_txnum, d_logMgr, d_bufferMgr);
    d_my_buffers = std::make_shared<BufferList>(d_bufferMgr);
}

Transaction::~Transaction() {
    // TODO: Implement cleanup
}

void Transaction::commit() {
    d_recovery_manager->commit();
    d_concurrency_manager->release();
    d_my_buffers->unpinAll();
}

void Transaction::rollback() {
    d_recovery_manager->rollback();
    d_concurrency_manager->release();
    d_my_buffers->unpinAll();
}

void Transaction::recover() {
    d_bufferMgr->flushAll(d_txnum);
    d_recovery_manager->recover();
}

void Transaction::pin(const simpledb::file::BlockId& blk) {
    d_my_buffers->pin(blk);
}

void Transaction::unpin(const simpledb::file::BlockId& blk) {
    d_my_buffers->unpin(blk);
}

int Transaction::getInt(const simpledb::file::BlockId& blk, int offset) {
    d_concurrency_manager->s_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    return buffer->contents().getInt(offset);
}

std::string Transaction::getString(const simpledb::file::BlockId& blk, int offset) {
    d_concurrency_manager->s_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    return buffer->contents().getString(offset);
}

void Transaction::setInt(const simpledb::file::BlockId& blk, int offset, int val, bool ok_to_log) {
    d_concurrency_manager->x_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    int lsn = -1;
    if (ok_to_log) {
        lsn = d_recovery_manager->set_int(*buffer, offset, val);
    }
    auto& page = buffer->contents();
    page.setInt(offset, val);
    buffer->setModified(d_txnum, lsn);
}

void Transaction::setString(const simpledb::file::BlockId& blk, int offset, const std::string& val, bool ok_to_log) {
    d_concurrency_manager->x_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    int lsn = -1;
    if (ok_to_log) {
        lsn = d_recovery_manager->set_string(*buffer, offset, val);
    }
    auto& page = buffer->contents();
    page.setString(offset, val);
    buffer->setModified(d_txnum, lsn);
}

int Transaction::availableBuffers() {
    return d_bufferMgr->available();
}

int Transaction::size(const std::string& filename) {
    return d_fileMgr->length(filename);
}

simpledb::file::BlockId Transaction::append(const std::string& filename) {
    return d_fileMgr->append(filename);
}

int Transaction::blockSize() {
    return d_fileMgr->blockSize();
}

int Transaction::next_txnum() {
    static int next_txnum = 0;
    static std::mutex s_txnum_mutex;
    std::lock_guard<std::mutex> lock(s_txnum_mutex);
    return next_txnum++;
}

} // namespace simpledb::tx 