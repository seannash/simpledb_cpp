#include "simpledb/tx/bufferlist.hpp"
#include <algorithm>

namespace simpledb::tx {

BufferList::BufferList(std::shared_ptr<simpledb::buffer::BufferMgr> bm)
    : d_buffers(), d_pins(), d_bm(std::move(bm)) {
}

std::shared_ptr<simpledb::buffer::Buffer> BufferList::getBuffer(const file::BlockId& blk) {
    auto it = d_buffers.find(blk);
    return (it != d_buffers.end()) ? it->second : nullptr;
}

void BufferList::pin(const file::BlockId& blk) {
    auto buff = d_bm->pin(blk);
    d_buffers[blk] = buff;
    d_pins.push_back(blk);
}

void BufferList::unpin(const file::BlockId& blk) {
    auto it = d_buffers.find(blk);
    if (it != d_buffers.end()) {
        d_bm->unpin(*it->second);
        auto pin_it = std::find(d_pins.begin(), d_pins.end(), blk);
        if (pin_it != d_pins.end()) {
            d_pins.erase(pin_it);
        }
        if (std::find(d_pins.begin(), d_pins.end(), blk) == d_pins.end()) {
            d_buffers.erase(it);
        }
    }
}

void BufferList::unpinAll() {
    for (const auto& blk : d_pins) {
        auto it = d_buffers.find(blk);
        if (it != d_buffers.end()) {
            d_bm->unpin(*it->second);
        }
    }
    d_buffers.clear();
    d_pins.clear();
}

} // namespace simpledb::tx 