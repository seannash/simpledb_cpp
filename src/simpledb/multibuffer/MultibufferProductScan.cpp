#include "simpledb/multibuffer/MultibufferProductScan.hpp"
#include "simpledb/multibuffer/BufferNeeds.hpp"
#include "simpledb/multibuffer/ChunkScan.hpp"

namespace simpledb::multibuffer {

MultibufferProductScan::MultibufferProductScan(
      std::shared_ptr<simpledb::tx::Transaction> tx,
      std::shared_ptr<simpledb::query::Scan> lhsscan,
      std::string tblname,
      simpledb::record::Layout layout)
: d_tx(std::move(tx))
, d_lhsscan(std::move(lhsscan))
, d_filename(tblname + ".tbl")
, d_layout(std::move(layout))
{
    d_filesize = d_tx->size(d_filename);
    int available = d_tx->available_buffers();
    d_chunksize = BufferNeeds::bestFactor(available, d_filesize);
    before_first();
}

void MultibufferProductScan::close() {
    d_prodscan->close();
}

void MultibufferProductScan::before_first() {
    d_nextblknum = 0;
    use_next_chunk();
}

bool MultibufferProductScan::next() {
    while (!d_prodscan->next()) {
        if (!use_next_chunk()) {
            return false;
        }
    }
    return true;
}

int MultibufferProductScan::get_int(std::string_view field) {
    return d_prodscan->get_int(field);
}

std::string MultibufferProductScan::get_string(std::string_view field) {
    return d_prodscan->get_string(field);
}

simpledb::query::Constant MultibufferProductScan::get_val(std::string_view field) {
    return d_prodscan->get_val(field);
}

bool MultibufferProductScan::has_field(std::string_view field) {
    return d_prodscan->has_field(field);
}

bool MultibufferProductScan::use_next_chunk() {
    if (d_rhsscan) {
        d_rhsscan->close();
    }
    if (d_nextblknum >= d_filesize) {
        return false;
    }
    int end = d_nextblknum + d_chunksize - 1;
    if (end >= d_filesize) {
        end = d_filesize - 1;
    }
    d_rhsscan = std::make_shared<ChunkScan>(d_tx, d_filename, d_layout, d_nextblknum, end);
    d_lhsscan->before_first();
    d_prodscan = std::make_shared<simpledb::query::ProductScan>(d_lhsscan, d_rhsscan);
    d_nextblknum = end + 1;
    return true;
}

} // namespace simpledb::multibuffer 