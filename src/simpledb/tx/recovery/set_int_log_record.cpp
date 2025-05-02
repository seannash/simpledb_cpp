#include "simpledb/tx/recovery/set_int_log_record.hpp"
#include <stdexcept>
#include <sstream>
#include <span>
#include <cstring>
#include <vector>

#include "simpledb/file/buffer_wrapper.hpp"
#include "simpledb/log/log_manager.hpp"
namespace simpledb::tx::recovery {

namespace {

constexpr int POS_TXNUM = 4;
constexpr int POS_FILENAME = 8;
constexpr int POS_OFFSET = 12;
constexpr int POS_VAL = 16;

} // namespace
SetIntLogRecord::SetIntLogRecord(std::span<char> bytes)
    : d_txnum(-1)
    , d_blocknum(-1)
    , d_offset(-1)
    , d_val(0)
    , d_blk("", 0)
{
    if (bytes.size() < sizeof(int) * 4) {
        throw std::runtime_error("SetInt log record is too short");
    }
    simpledb::file::BufferWrapper bw(bytes);
    d_txnum = bw.getInt(POS_TXNUM);
    std::string filename = bw.getString(POS_FILENAME);
    int bpos = POS_FILENAME + bw.maxLength(filename);
    d_blocknum = bw.getInt(bpos);
    d_blk = simpledb::file::BlockId(filename, d_blocknum);
    int opos = bpos + sizeof(int);
    d_offset = bw.getInt(opos);
    int vpos = opos + sizeof(int);
    d_val = bw.getInt(vpos);
}

int SetIntLogRecord::op() const {
    return SET_INT;
}

int SetIntLogRecord::txnum() const {
    return d_txnum;
}

void SetIntLogRecord::undo(Transaction& tx) {
    tx.pin(d_blk);
    tx.set_int(d_blk, d_offset, d_val, false); // Do not log this
    tx.unpin(d_blk);
}

std::string SetIntLogRecord::to_string() const {
    std::stringstream ss;
    ss << "<SET_INT " << d_txnum << " " << d_blocknum << " " << d_offset << " " << d_val << ">";
    return ss.str();
}

int SetIntLogRecord::write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum, simpledb::file::BlockId blk, int offset, int val) {
    int tpos = 0;
    int fpos = tpos + sizeof(int);
    int bpos = fpos + blk.fileName().size() + sizeof(int);
    int opos = bpos + sizeof(int);
    int vpos = opos + sizeof(int);
    int size = vpos + sizeof(int);
    std::vector<char> buf(size, 0);
    simpledb::file::BufferWrapper bw(buf);
    bw.setInt(0, SET_INT);
    bw.setInt(tpos, txnum);
    bw.setString(fpos, blk.fileName());
    bw.setInt(bpos, blk.number());
    bw.setInt(opos, offset);
    bw.setInt(vpos, val);
    return lm->append(buf);
}

} // namespace simpledb::tx::recovery 