#pragma once

#include "simpledb/tx/recovery/log_record.hpp"
#include "simpledb/log/log_manager.hpp"
#include "simpledb/file/block_id.hpp"
#include <span>

namespace simpledb::tx::recovery {

class StartLogRecord : public LogRecord {
public:
    virtual ~StartLogRecord() noexcept override = default;
    explicit StartLogRecord(std::span<char> bytes);
    int op() const override;
    int txnum() const override;
    void undo(int txnum) override;
    std::string to_string() const override;
    static int write_to_log(std::shared_ptr<simpledb::log::LogManager> lm, int txnum, simpledb::file::BlockId blk, int offset, std::span<char> val);
private:
    int d_txnum;
};

} // namespace simpledb::tx::recovery 