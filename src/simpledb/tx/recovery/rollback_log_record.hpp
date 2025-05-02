#pragma once

#include "simpledb/tx/recovery/log_record.hpp"
#include <span>
#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/block_id.hpp"

namespace simpledb::tx::recovery {

class RollbackLogRecord : public LogRecord {
public:
    virtual ~RollbackLogRecord() noexcept override = default;
    explicit RollbackLogRecord(std::span<char> bytes);
    int op() const override;
    int txnum() const override;
    void undo(int txnum) override;
    std::string to_string() const override;
    static int write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum, simpledb::file::BlockId blk, int offset, std::span<char> val);

private:
    int d_txnum;
};

} // namespace simpledb::tx::recovery 