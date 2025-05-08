#pragma once

#include "simpledb/file/BlockId.hpp"

#include <chrono>
#include <mutex>
#include <unordered_map>

namespace simpledb::tx::concurrency {

class LockTable {
public:
    LockTable();
    ~LockTable() = default;

    void s_lock(simpledb::file::BlockId blk);
    void x_lock(simpledb::file::BlockId blk);
    void unlock(simpledb::file::BlockId blk);

private:
    std::mutex d_mutex;
    std::unordered_map<simpledb::file::BlockId, int> d_locks;

    bool has_x_lock(simpledb::file::BlockId& blk);
    bool has_other_s_lock(simpledb::file::BlockId& blk);

    bool waiting_too_long(std::chrono::steady_clock::time_point start_time);

};

} // namespace simpledb::tx::concurrency