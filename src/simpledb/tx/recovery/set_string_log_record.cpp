#include "simpledb/tx/recovery/set_string_log_record.hpp"
#include <stdexcept>
#include <sstream>
#include <span>
#include <cstring>
#include <vector>

namespace simpledb::tx::recovery {

SetStringLogRecord::SetStringLogRecord(std::span<char> bytes)
    : d_txnum(-1)
    , d_blk("", 0)
    , d_offset(-1)
{
    if (bytes.size() < sizeof(int) * 3) {
        throw std::runtime_error("SetString log record is too short");
    }
    simpledb::file::BufferWrapper bw(bytes);
    int tpos = sizeof(int);
    d_txnum = bw.getInt(tpos);
    int fpos = tpos + sizeof(int);
    std::string filename = bw.getString(fpos);
    int bpos = fpos + bw.maxLength(filename);
    int blknum = bw.getInt(bpos);
    d_blk = simpledb::file::BlockId(filename, blknum);
    int opus = bpos + sizeof(int);
    d_offset = bw.getInt(opus);
    int vpos = opus + sizeof(int);
    d_val = bw.getString(vpos);
}

int SetStringLogRecord::op() const {
    return SET_STRING;
}

int SetStringLogRecord::txnum() const {
    return d_txnum;
}

void SetStringLogRecord::undo(int txnum) {
}

std::string SetStringLogRecord::to_string() const {
    std::stringstream ss;
    ss << "<SET_STRING " << d_txnum << " " << d_blk << " " << d_offset << " " << d_val << ">";
    return ss.str();
}

int SetStringLogRecord::write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum, simpledb::file::BlockId blk, int offset, std::string_view val) {
    int tpos = sizeof(int);
    int fpos = tpos + sizeof(int);
    int bpos = fpos + blk.fileName().size()+sizeof(int);
    int opos = bpos + sizeof(int);
    int vpos = opos + sizeof(int);
    int size = vpos + val.size() +sizeof(int);
    
    std::vector<char> buf(size, 0);
    simpledb::file::BufferWrapper bw(buf);
    bw.setInt(0, SET_STRING);
    bw.setInt(tpos, txnum);
    bw.setString(fpos, blk.fileName());
    bw.setInt(bpos, blk.number());
    bw.setInt(opos, offset);
    bw.setString(vpos, val);
    return lm->append(buf);
}

} // namespace simpledb::tx::recovery 