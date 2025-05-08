#pragma once

#include "simpledb/tx/recovery/log_record.hpp"
#include <span>
#include "simpledb/log/LogMgr.hpp"
#include "simpledb/file/BlockId.hpp"

namespace simpledb::tx::recovery {

class RollbackLogRecord : public LogRecord {
public:
    virtual ~RollbackLogRecord() noexcept override = default;
    explicit RollbackLogRecord(std::span<char> bytes);
    int op() const override;
    int txnum() const override;
    void undo(Transaction& tx) override;
    std::string to_string() const override;
    static int write_to_log(std::shared_ptr<simpledb::log::LogMgr> lm, int txnum);

private:
    int d_txnum;
};

} // namespace simpledb::tx::recovery 