#include "simpledb/tx/transaction.hpp"

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
    //d_my_buffer = d_buffer_manager->new_buffer();
}

Transaction::~Transaction() {
    // TODO: Implement cleanup
}

void Transaction::commit() {
    // TODO: Implement commit
}

void Transaction::rollback() {
    // TODO: Implement rollback
}

void Transaction::recover() {
    // TODO: Implement recovery
}

void Transaction::pin(const simpledb::file::BlockId& blk) {
    // TODO: Implement pin
}

void Transaction::unpin(const simpledb::file::BlockId& blk) {
    // TODO: Implement unpin
}

int Transaction::get_int(const simpledb::file::BlockId& blk, int offset) {
    // TODO: Implement get_int
    return 0;
}

std::string Transaction::get_string(const simpledb::file::BlockId& blk, int offset) {
    // TODO: Implement get_string
    return "";
}

void Transaction::set_int(const simpledb::file::BlockId& blk, int offset, int val, bool ok_to_log) {
    // TODO: Implement set_int
}

void Transaction::set_string(const simpledb::file::BlockId& blk, int offset, const std::string& val, bool ok_to_log) {
    // TODO: Implement set_string
}

int Transaction::available_buffers() {
    // TODO: Implement available_buffers
    return 0;
}

int Transaction::size(const std::string& filename) {
    // TODO: Implement size
    return 0;
}

simpledb::file::BlockId Transaction::append(const std::string& filename) {
    // TODO: Implement append
    return simpledb::file::BlockId(filename, 0);
}

int Transaction::block_size() {
    // TODO: Implement block_size
    return 0;
}

int Transaction::next_txnum() {
    static int next_txnum = 0;
    static std::mutex s_txnum_mutex;
    std::lock_guard<std::mutex> lock(s_txnum_mutex);
    return next_txnum++;
}

} // namespace simpledb::tx 