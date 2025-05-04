#include "simpledb/tx/recovery/start_log_record.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace simpledb::tx::recovery {

StartLogRecord::StartLogRecord(std::span<char> bytes) : d_txnum(-1) {
    if (bytes.size() < sizeof(int) * 2) {
        throw std::runtime_error("Start log record is too short");
    }
    d_txnum = *reinterpret_cast<const int*>(bytes.data() + sizeof(int));
}

int StartLogRecord::op() const {
    return START;
}

int StartLogRecord::txnum() const {
    return d_txnum;
}

void StartLogRecord::undo(Transaction& tx) {
}

std::string StartLogRecord::to_string() const {
    std::stringstream ss;
    ss << "<START " << d_txnum << ">";
    return ss.str();
}

int StartLogRecord::write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum) {
    std::vector<char> buf(sizeof(int)*2, 0);
    *reinterpret_cast<int*>(buf.data()) = START;
    *reinterpret_cast<int*>(buf.data() + sizeof(int)) = txnum;
    return lm->append(buf);
}

} // namespace simpledb::tx::recovery 