#include "simpledb/tx/concurrency/concurrency_manager.hpp"
#include <stdexcept>

namespace simpledb::tx::concurrency {

ConcurrencyManager::ConcurrencyManager() = default;

ConcurrencyManager::~ConcurrencyManager() {
    release();
}

void ConcurrencyManager::s_lock(const simpledb::file::BlockId& blk) {
    if (d_locks.find(blk) == d_locks.end()) {
        d_lock_table.s_lock(blk);
        d_locks[blk] = "S";
    }
}

void ConcurrencyManager::x_lock(const simpledb::file::BlockId& blk) {
    if (!has_x_lock(blk)) {
        s_lock(blk);  // Upgrade to X lock by first getting S lock
        d_lock_table.x_lock(blk);
        d_locks[blk] = "X";
    }
}

void ConcurrencyManager::release() {
    for (const auto& [blk, _] : d_locks) {
        d_lock_table.unlock(blk);
    }
    d_locks.clear();
}

bool ConcurrencyManager::has_x_lock(const simpledb::file::BlockId& blk) {
    auto it = d_locks.find(blk);
    return it != d_locks.end() && it->second == "X";
}

} // namespace simpledb::tx::concurrency
