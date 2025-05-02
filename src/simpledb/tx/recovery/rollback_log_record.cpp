#include "simpledb/tx/recovery/rollback_log_record.hpp"
#include <stdexcept>
#include <sstream>

namespace simpledb::tx::recovery {

RollbackLogRecord::RollbackLogRecord(std::span<char> bytes) : d_txnum(-1) {
    if (bytes.size() < sizeof(int) * 2) {
        throw std::runtime_error("Rollback log record is too short");
    }
    d_txnum = *reinterpret_cast<const int*>(bytes.data() + sizeof(int));
}

int RollbackLogRecord::op() const {
    return ROLLBACK;
}

int RollbackLogRecord::txnum() const {
    return d_txnum;
}

void RollbackLogRecord::undo(Transaction& tx) {
}

std::string RollbackLogRecord::to_string() const {
    std::stringstream ss;
    ss << "<ROLLBACK " << d_txnum << ">";
    return ss.str();
}

int RollbackLogRecord::write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum) {
    std::vector<char> buf(sizeof(int)*2, 0);
    *reinterpret_cast<int*>(buf.data()) = ROLLBACK;
    *reinterpret_cast<int*>(buf.data() + sizeof(int)) = txnum;
    return lm->append(buf);
}

} // namespace simpledb::tx::recovery 