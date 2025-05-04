#include "simpledb/tx/recovery/commit_log_record.hpp"

#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/block_id.hpp"

#include <stdexcept>
#include <sstream>
#include <vector>
#include <span>
#include <cstring>

namespace simpledb::tx::recovery {

namespace {

constexpr int POS_TXNUM = 4;

} // namespace

CommitLogRecord::CommitLogRecord(std::span<char> bytes)
: d_txnum(-1) {
    if (bytes.size() < sizeof(int) * 2) {
        throw std::runtime_error("Commit log record is too short");
    }
    //d_txnum = *reinterpret_cast<const int*>(bytes.data() + POS_TXNUM);
    std::memcpy(&d_txnum, bytes.data() + POS_TXNUM, sizeof(int));
}

int CommitLogRecord::op() const {
    return COMMIT;
}

int CommitLogRecord::txnum() const {
    return d_txnum;
}

void CommitLogRecord::undo(Transaction& tx) {
}

std::string CommitLogRecord::to_string() const {
    std::stringstream ss;
    ss << "<COMMIT " << d_txnum << ">";
    return ss.str();
}

int CommitLogRecord::write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum) {
    std::vector<char> buf(sizeof(int)*2, 0);
    *reinterpret_cast<int*>(buf.data()) = COMMIT;
    std::memcpy(buf.data() + sizeof(int), &txnum, sizeof(int));
    return lm->append(buf);
}

} // namespace simpledb::tx::recovery 