#include "simpledb/buffer/buffer.hpp"

namespace simpledb::buffer {

Buffer::Buffer(std::shared_ptr<simpledb::file::FileManager> file_manager, 
              std::shared_ptr<simpledb::log::LogManager> log_manager)
    : d_file_manager(file_manager)
    , d_log_manager(log_manager)
    , d_page(file_manager->blockSize())
    , d_pins(0)
    , d_modifying_tx(-1)
    , d_modifying_lsn(-1)
    , d_block_id(simpledb::file::BlockId("",0))
{
}

Buffer::~Buffer() {
}

simpledb::file::Page& Buffer::contents() {
    return d_page;
}

simpledb::file::BlockId Buffer::block() {
    return d_block_id;
}

bool Buffer::is_pinned() {
    return d_pins > 0;
}

void Buffer::set_modified(int txnum, int lsn) {
    d_modifying_tx = txnum;
    if (lsn >= 0) {
        d_modifying_lsn = lsn;
    }
}

int Buffer::modifing_tx() {
    return d_modifying_tx;
}

void Buffer::assign_to_block(const simpledb::file::BlockId& blk) {
    flush();
    d_block_id = blk;
    d_file_manager->read(d_block_id, &d_page);
    d_pins = 0;
}

void Buffer::flush() {
    if (d_modifying_tx >= 0) {
        d_log_manager->flush(d_modifying_lsn);
        d_file_manager->write(d_block_id, &d_page);
        d_modifying_tx = -1;
    }
}

void Buffer::pin() {
    d_pins++;
}

void Buffer::unpin() {
    d_pins--;
}

} // namespace simpledb::buffer
