#pragma once

#include <unordered_map>
#include <vector>
#include "simpledb/file/block_id.hpp"
#include "simpledb/buffer/buffer.hpp"
#include "simpledb/buffer/buffer_manager.hpp"
#include <memory>
namespace simpledb::tx {

class BufferList {
public:
    explicit BufferList(std::shared_ptr<simpledb::buffer::BufferManager> bm);

    std::shared_ptr<simpledb::buffer::Buffer> getBuffer(const file::BlockId& blk);
    void pin(const file::BlockId& blk);
    void unpin(const file::BlockId& blk);
    void unpinAll();

private:
    std::unordered_map<file::BlockId, std::shared_ptr<simpledb::buffer::Buffer>> d_buffers;
    std::vector<file::BlockId> d_pins;
    std::shared_ptr<simpledb::buffer::BufferManager> d_bm;
};

} // namespace simpledb::tx