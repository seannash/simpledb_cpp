#include "log_record_builder.hpp"

#include <stdexcept>

#include "checkpoint_log_record.hpp"
#include "start_log_record.hpp"
#include "commit_log_record.hpp"
#include "rollback_log_record.hpp"
#include "set_int_log_record.hpp"
#include "set_string_log_record.hpp"


namespace simpledb::tx::recovery {

std::unique_ptr<LogRecord> LogRecordBuilder::build(std::span<char> bytes) {
    if (bytes.size() < sizeof(int)) {
        throw std::runtime_error("Log record is too short");
    }

    int op = *reinterpret_cast<const int*>(bytes.data());
    switch (op) {
        case LogRecord::CHECKPOINT:
            return std::make_unique<CheckpointLogRecord>();
        case LogRecord::START:
            return std::make_unique<StartLogRecord>(bytes);
        case LogRecord::COMMIT:
            return std::make_unique<CommitLogRecord>(bytes);
        case LogRecord::ROLLBACK:
            return std::make_unique<RollbackLogRecord>(bytes);
        case LogRecord::SET_INT:
            return std::make_unique<SetIntLogRecord>(bytes);
        case LogRecord::SET_STRING:
            return std::make_unique<SetStringLogRecord>(bytes);
        default:
            throw std::runtime_error("Invalid log record operation");
    }
}

} // namespace simpledb::tx::recovery