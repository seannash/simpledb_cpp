#pragma once

#include "simpledb/tx/recovery/log_record.hpp"
#include <span>

namespace simpledb::tx::recovery {

class LogRecordBuilder {
public:

    static std::unique_ptr<LogRecord> build(std::span<char> bytes);
};

} // namespace simpledb::tx::recovery