#include "simpledb/buffer/buffer_manager.hpp"
#include <algorithm>
#include <thread>
#include <chrono>

namespace simpledb::buffer {

namespace {

const int MAX_TIME = 10000;

}

BufferManager::BufferManager(std::shared_ptr<simpledb::file::FileManager> file_manager,
                           std::shared_ptr<simpledb::log::LogManager> log_manager,
                           int num_buffers)
    : d_file_manager(file_manager)
    , d_log_manager(log_manager)
    , d_num_buffers(num_buffers)
    , d_buffer_pool()
    , d_num_available(num_buffers)
{
    for (int i = 0; i < num_buffers; i++) {
        d_buffer_pool.push_back(std::make_shared<Buffer>(file_manager, log_manager));
    }
}

BufferManager::~BufferManager() {

}

int BufferManager::available() {
    return d_num_available;
}

void BufferManager::flush_all(int txnum) {
    for (auto& buffer : d_buffer_pool) {
        if (buffer->modifing_tx() == txnum) {
            buffer->flush();
        }
    }
}

std::shared_ptr<Buffer> BufferManager::pin(const simpledb::file::BlockId& blk) {
    // TODO add back in retry logic
    auto buffer = try_to_pin(blk);
    if (!buffer) {
        throw std::runtime_error("BufferManager::pin: Failed to pin block");
    }
    return buffer;
}

void BufferManager::unpin(Buffer& buffer) {
    buffer.unpin();
    if (!buffer.is_pinned()) {
        d_num_available++;
        // notify all waiting threads
    }
}

std::shared_ptr<Buffer> BufferManager::try_to_pin(const simpledb::file::BlockId& blk) {
    auto buffer = find_existing_buffer(blk);
    if (!buffer) {
        buffer = choose_unpinned_buffer();
        if (!buffer) {
            return nullptr;
        }
        buffer->assign_to_block(blk);
    }
    if (!buffer->is_pinned()) {
        d_num_available--;
    }
    buffer->pin();
    return buffer;
}

std::shared_ptr<Buffer> BufferManager::find_existing_buffer(const simpledb::file::BlockId& blk) {
    for (auto& buffer : d_buffer_pool) {
        if (buffer->block() == blk) {
            return buffer;
        }
    }
    return nullptr;
}

std::shared_ptr<Buffer> BufferManager::choose_unpinned_buffer() {
    for (auto& buffer : d_buffer_pool) {
        if (!buffer->is_pinned()) {
            return buffer;
        }
    }
    return nullptr;
}

} // namespace simpledb::buffer
