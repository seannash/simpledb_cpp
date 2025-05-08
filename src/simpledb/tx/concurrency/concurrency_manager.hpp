#pragma once

#include "simpledb/file/BlockId.hpp"
#include "simpledb/tx/concurrency/lock_table.hpp"
#include <unordered_map>
#include <string>

namespace simpledb::tx::concurrency {

class ConcurrencyManager {
public:
    ConcurrencyManager();
    ~ConcurrencyManager();

    void s_lock(const simpledb::file::BlockId& blk);
    void x_lock(const simpledb::file::BlockId& blk);
    void release();

private:
    LockTable d_lock_table;
    std::unordered_map<simpledb::file::BlockId, std::string> d_locks;

    bool has_x_lock(const simpledb::file::BlockId& blk);
};

} // namespace simpledb::tx::concurrency
