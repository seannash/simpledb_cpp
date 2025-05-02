#include "simpledb/tx/transaction.hpp"
#include <sstream>
namespace simpledb::tx {

Transaction::Transaction(std::shared_ptr<simpledb::file::FileManager> file_manager,
                       std::shared_ptr<simpledb::log::LogManager> log_manager,
                       std::shared_ptr<simpledb::buffer::BufferManager> buffer_manager)
    : d_file_manager(file_manager)
    , d_log_manager(log_manager)
    , d_buffer_manager(buffer_manager)
    , d_txnum(0)
{
    d_txnum = next_txnum();
    d_concurrency_manager = std::make_shared<concurrency::ConcurrencyManager>();
    d_recovery_manager = std::make_shared<recovery::RecoveryManager>(this, d_txnum, d_log_manager, d_buffer_manager);
    d_my_buffers = std::make_shared<BufferList>(d_buffer_manager);
}

Transaction::~Transaction() {
    // TODO: Implement cleanup
}

void Transaction::commit() {
    d_recovery_manager->commit();
    d_concurrency_manager->release();
    d_my_buffers->unpinAll();
    std::cout << "Transaction " << d_txnum << " committed" << std::endl;
}

void Transaction::rollback() {
    d_recovery_manager->rollback();
    d_concurrency_manager->release();
    d_my_buffers->unpinAll();
    std::cout << "Transaction " << d_txnum << " rolled back" << std::endl;
}

void Transaction::recover() {
    d_buffer_manager->flush_all(d_txnum);
    d_recovery_manager->recover();
}

void Transaction::pin(const simpledb::file::BlockId& blk) {
    d_my_buffers->pin(blk);
}

void Transaction::unpin(const simpledb::file::BlockId& blk) {
    d_my_buffers->unpin(blk);
}

int Transaction::get_int(const simpledb::file::BlockId& blk, int offset) {
    d_concurrency_manager->s_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    return buffer->contents().getInt(offset);
}

std::string Transaction::get_string(const simpledb::file::BlockId& blk, int offset) {
    d_concurrency_manager->s_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    return buffer->contents().getString(offset);
}

void Transaction::set_int(const simpledb::file::BlockId& blk, int offset, int val, bool ok_to_log) {
    d_concurrency_manager->x_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    int lsn = -1;
    if (ok_to_log) {
        lsn = d_recovery_manager->set_int(*buffer, offset, val);
    }
    auto& page = buffer->contents();
    page.setInt(offset, val);
    buffer->set_modified(true, lsn);
}

void Transaction::set_string(const simpledb::file::BlockId& blk, int offset, const std::string& val, bool ok_to_log) {
    d_concurrency_manager->x_lock(blk);
    auto buffer = d_my_buffers->getBuffer(blk);
    int lsn = -1;
    if (ok_to_log) {
        lsn = d_recovery_manager->set_string(*buffer, offset, val);
    }
    auto& page = buffer->contents();
    page.setString(offset, val);
    buffer->set_modified(true, lsn);
}

int Transaction::available_buffers() {
    return d_buffer_manager->available();
}

int Transaction::size(const std::string& filename) {
    return d_file_manager->length(filename);
}

simpledb::file::BlockId Transaction::append(const std::string& filename) {
    return d_file_manager->append(filename);
}

int Transaction::block_size() {
    return d_file_manager->blockSize();
}

int Transaction::next_txnum() {
    static int next_txnum = 0;
    static std::mutex s_txnum_mutex;
    std::lock_guard<std::mutex> lock(s_txnum_mutex);
    return next_txnum++;
}

} // namespace simpledb::tx 