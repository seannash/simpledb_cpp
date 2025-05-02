#include "simpledb/tx/recovery/checkpoint_log_record.hpp"
#include <stdexcept>
#include <vector>

namespace simpledb::tx::recovery {

CheckpointLogRecord::CheckpointLogRecord() {}

int CheckpointLogRecord::op() const {
    return CHECKPOINT;
}

int CheckpointLogRecord::txnum() const {
    return -1;  // Checkpoint records don't have a transaction number
}

void CheckpointLogRecord::undo(Transaction& tx) {
}

int CheckpointLogRecord::write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum) {
    std::vector<char> data(sizeof(int), 0); 
    *reinterpret_cast<int*>(data.data()) = CHECKPOINT;
    *reinterpret_cast<int*>(data.data() + sizeof(int)) = txnum;
    std::cout << "<CHECKPOINT " << txnum << ">" << std::endl;
    return lm->append(data);
}

std::string CheckpointLogRecord::to_string() const {
    return "<CHECKPOINT>";
}

} // namespace simpledb::tx::recovery 