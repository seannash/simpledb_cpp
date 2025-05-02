#pragma once

#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/block_id.hpp"
#include "simpledb/tx/recovery/log_record.hpp"
#include <span>

namespace simpledb::tx::recovery {

class   CheckpointLogRecord : public LogRecord {
public:
    virtual ~CheckpointLogRecord() noexcept override = default;
    CheckpointLogRecord();

    int op() const override;
    int txnum() const override;
    void undo(int txnum) override;

    static int write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum, simpledb::file::BlockId blk, int offset, std::span<char> val);

    std::string to_string() const override;

private:
    int d_txnum;
};

} // namespace simpledb::tx::recovery 