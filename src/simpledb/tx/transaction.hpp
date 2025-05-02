#pragma once

#include "simpledb/log/log_manager.hpp"
#include "simpledb/buffer/buffer_manager.hpp"
#include "simpledb/file/file_manager.hpp"
#include "simpledb/tx/concurrency/concurrency_manager.hpp"
#include "simpledb/tx/recovery/recovery_manager.hpp"
#include "simpledb/tx/bufferlist.hpp"
#include <mutex>
#include <memory>
#include <string>

namespace recovery {
class RecoveryManager;
}

namespace simpledb::tx {

class Transaction {
public:
    Transaction(std::shared_ptr<simpledb::file::FileManager> file_manager, std::shared_ptr<simpledb::log::LogManager> log_manager, std::shared_ptr<simpledb::buffer::BufferManager> buffer_manager);
    ~Transaction();

    void commit();
    void rollback();
    void recover();

    void pin(const simpledb::file::BlockId& blk);
    void unpin(const simpledb::file::BlockId& blk);

    int get_int(const simpledb::file::BlockId& blk, int offset);
    std::string get_string(const simpledb::file::BlockId& blk, int offset);

    void set_int(const simpledb::file::BlockId& blk, int offset, int val, bool ok_to_log);
    void set_string(const simpledb::file::BlockId& blk, int offset, const std::string& val, bool ok_to_log);

    int available_buffers();
    int size(const std::string& filename);
    simpledb::file::BlockId append(const std::string& filename);
    int block_size();
private:
    std::shared_ptr<simpledb::file::FileManager> d_file_manager;
    std::shared_ptr<simpledb::log::LogManager> d_log_manager;
    std::shared_ptr<simpledb::buffer::BufferManager> d_buffer_manager;
    int d_txnum;

    std::shared_ptr<simpledb::tx::concurrency::ConcurrencyManager> d_concurrency_manager;
    std::shared_ptr<recovery::RecoveryManager> d_recovery_manager;
    std::shared_ptr<BufferList> d_my_buffers;
    static int next_txnum();
};

} // namespace simpledb::tx