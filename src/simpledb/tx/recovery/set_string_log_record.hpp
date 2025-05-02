#pragma once

#include "simpledb/tx/recovery/log_record.hpp"
#include <span>
#include <string>
#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/block_id.hpp"

namespace simpledb::tx::recovery {

class SetStringLogRecord : public LogRecord {
public:
    virtual ~SetStringLogRecord() noexcept override = default;
    explicit SetStringLogRecord(std::span<char> bytes);
    int op() const override;
    int txnum() const override;
    void undo(int txnum) override;
    std::string to_string() const override;
    static int write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum, simpledb::file::BlockId blk, int offset, std::string_view val);

private:
    int d_txnum;
    simpledb::file::BlockId d_blk;
    int d_offset;
    std::string d_val;
};

} // namespace simpledb::tx::recovery 