#pragma once

#include "simpledb/log/LogMgr.hpp"
#include "simpledb/buffer/BufferMgr.hpp"
#include "simpledb/file/FileMgr.hpp"
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
    Transaction(std::shared_ptr<simpledb::file::FileMgr> file_manager, std::shared_ptr<simpledb::log::LogMgr> log_manager, std::shared_ptr<simpledb::buffer::BufferMgr> buffer_manager);
    ~Transaction();

    void commit();
    void rollback();
    void recover();

    void pin(const simpledb::file::BlockId& blk);
    void unpin(const simpledb::file::BlockId& blk);

    int getInt(const simpledb::file::BlockId& blk, int offset);
    std::string getString(const simpledb::file::BlockId& blk, int offset);

    void setInt(const simpledb::file::BlockId& blk, int offset, int val, bool ok_to_log);
    void setString(const simpledb::file::BlockId& blk, int offset, const std::string& val, bool ok_to_log);

    int availableBuffers();
    int size(const std::string& filename);
    simpledb::file::BlockId append(const std::string& filename);
    int blockSize();
private:
    std::shared_ptr<simpledb::file::FileMgr> d_fileMgr;
    std::shared_ptr<simpledb::log::LogMgr> d_logMgr;
    std::shared_ptr<simpledb::buffer::BufferMgr> d_bufferMgr;
    int d_txnum;

    std::shared_ptr<simpledb::tx::concurrency::ConcurrencyManager> d_concurrency_manager;
    std::shared_ptr<recovery::RecoveryManager> d_recovery_manager;
    std::shared_ptr<BufferList> d_my_buffers;
    static int next_txnum();
};

} // namespace simpledb::tx