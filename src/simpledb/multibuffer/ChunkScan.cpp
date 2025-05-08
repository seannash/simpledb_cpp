#include "simpledb/multibuffer/ChunkScan.hpp"
#include "simpledb/file/block_id.hpp"
#include <stdexcept>

namespace simpledb::multibuffer {

ChunkScan::ChunkScan(std::shared_ptr<simpledb::tx::Transaction> tx,
                     std::string filename,
                     simpledb::record::Layout layout,
                     int startbnum,
                     int endbnum)
    : d_tx(std::move(tx))
    , d_filename(std::move(filename))
    , d_layout(std::move(layout))
    , d_startbnum(startbnum)
    , d_endbnum(endbnum)
{
    for (int i = startbnum; i <= endbnum; i++) {
        simpledb::file::BlockId blk(d_filename, i);
        d_buffs.push_back(std::make_shared<simpledb::record::RecordPage>(d_tx, blk, d_layout));
    }
    move_to_block(startbnum);
}

void ChunkScan::close() {
    for (size_t i = 0; i < d_buffs.size(); i++) {
        simpledb::file::BlockId blk(d_filename, d_startbnum + static_cast<int>(i));
        d_tx->unpin(blk);
    }
}

void ChunkScan::before_first() {
    move_to_block(d_startbnum);
}

bool ChunkScan::next() {
    d_currentslot = d_rp->next_after(d_currentslot);
    while (d_currentslot < 0) {
        if (d_currentbnum == d_endbnum) {
            return false;
        }
        move_to_block(d_rp->block().number() + 1);
        d_currentslot = d_rp->next_after(d_currentslot);
    }
    return true;
}

int ChunkScan::get_int(std::string_view field) {
    return d_rp->get_int(d_currentslot, std::string(field));
}

std::string ChunkScan::get_string(std::string_view field) {
    return d_rp->get_string(d_currentslot, std::string(field));
}

simpledb::query::Constant ChunkScan::get_val(std::string_view field) {
    if (d_layout.schema().get_type(std::string(field)) == jdbc::ColumnType::INT) {
        return simpledb::query::Constant(get_int(field));
    } else {
        return simpledb::query::Constant(get_string(field));
    }
}

bool ChunkScan::has_field(std::string_view field) {
    return d_layout.schema().has_field(std::string(field));
}

void ChunkScan::move_to_block(int blknum) {
    d_currentbnum = blknum;
    d_rp = d_buffs[d_currentbnum - d_startbnum];
    d_currentslot = -1;
}

} // namespace simpledb::multibuffer 