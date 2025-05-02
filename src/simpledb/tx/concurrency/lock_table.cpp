#include "simpledb/tx/concurrency/lock_table.hpp"
#include <stdexcept>
#include <thread>
#include <chrono>

namespace simpledb::tx::concurrency {

namespace {
constexpr int MAX_WAITING_TIME = 10;
}

LockTable::LockTable() {}

void LockTable::s_lock(simpledb::file::BlockId blk) {
    std::unique_lock<std::mutex> lock(d_mutex);
    //lock.lock();
    try {
        auto start_time = std::chrono::steady_clock::now();
        while (has_x_lock(blk) && !waiting_too_long(start_time)) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            lock.lock();
        }
        if (has_x_lock(blk))
            throw std::runtime_error("LockTable::s_lock: Has x lock");
        d_locks[blk] += 1;
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("LockTable::s_lock: " + std::string(e.what()));
    }
}

void LockTable::x_lock(simpledb::file::BlockId blk) {
    std::unique_lock<std::mutex> lock(d_mutex);
    try {
        auto start_time = std::chrono::steady_clock::now();
        while (has_other_s_lock(blk) && !waiting_too_long(start_time)) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            lock.lock();
        }
        if (has_other_s_lock(blk))
            throw std::runtime_error("LockTable::x_lock: Has other s lock");
        d_locks[blk] = -1;
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("LockTable::x_lock: " + std::string(e.what()));
    }
}

void LockTable::unlock(simpledb::file::BlockId blk) {
    std::lock_guard<std::mutex> lock(d_mutex);
    int val = d_locks[blk];
    if (val > 1) {
        d_locks[blk] -= 1;
    } else {
        d_locks.erase(blk);
    }
}

bool LockTable::has_x_lock(simpledb::file::BlockId& blk) {
    return d_locks[blk] < 0;
}

bool LockTable::has_other_s_lock(simpledb::file::BlockId& blk) {
    return d_locks[blk] > 1;
}

bool LockTable::waiting_too_long(std::chrono::steady_clock::time_point start_time) {
    return std::chrono::steady_clock::now() - start_time > std::chrono::seconds(MAX_WAITING_TIME);
}


} // namespace simpledb::tx::concurrency
