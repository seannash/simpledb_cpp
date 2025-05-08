#pragma once

#include "simpledb/tx/recovery/log_record.hpp"
#include "simpledb/log/LogMgr.hpp"
#include "simpledb/file/BlockId.hpp"
#include <span>

namespace simpledb::tx::recovery {

class StartLogRecord : public LogRecord {
public:
    virtual ~StartLogRecord() noexcept override = default;
    explicit StartLogRecord(std::span<char> bytes);
    int op() const override;
    int txnum() const override;
    void undo(Transaction& tx) override;
    std::string to_string() const override;
    static int write_to_log(std::shared_ptr<simpledb::log::LogMgr> lm, int txnum);
private:
    int d_txnum;
};

} // namespace simpledb::tx::recovery 