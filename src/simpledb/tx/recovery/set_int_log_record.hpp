#pragma once

#include "simpledb/tx/recovery/log_record.hpp"
#include <span>
#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/block_id.hpp"

namespace simpledb::tx::recovery {

class  SetIntLogRecord : public LogRecord {
public:
    virtual ~SetIntLogRecord() noexcept override = default;
    explicit SetIntLogRecord(std::span<char> bytes);
    int op() const override;
    int txnum() const override;
    void undo(int txnum) override;
    std::string to_string() const override;
    static int write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum, simpledb::file::BlockId blk, int offset, std::span<char> val);

private:
    int d_txnum;
    int d_blocknum;
    int d_offset;
    int d_val;
    simpledb::file::BlockId d_blk;
};

} // namespace simpledb::tx::recovery 