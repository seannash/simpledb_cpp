#pragma once

#include "simpledb/buffer/buffer.hpp"
#include "simpledb/file/file_manager.hpp"
#include "simpledb/log/log_manager.hpp"

#include <memory>

namespace simpledb::buffer {

class BufferManager {
public:
    BufferManager(std::shared_ptr<simpledb::file::FileManager> file_manager, std::shared_ptr<simpledb::log::LogManager> log_manager, int num_buffers);
    ~BufferManager();
    std::shared_ptr<Buffer> pin(const simpledb::file::BlockId& blk);
    void unpin(Buffer& buffer);

    int available();
    void flush_all(int txnum);

private:
    std::shared_ptr<simpledb::file::FileManager> d_file_manager;
    std::shared_ptr<simpledb::log::LogManager> d_log_manager;
    int d_num_buffers;
    std::vector<std::shared_ptr<Buffer>> d_buffer_pool;
    int d_num_available;

    std::shared_ptr<Buffer> choose_unpinned_buffer();
    std::shared_ptr<Buffer> find_existing_buffer(const simpledb::file::BlockId& blk);
    std::shared_ptr<Buffer> try_to_pin(const simpledb::file::BlockId& blk);
};
} // namespace simpledb::buffer

