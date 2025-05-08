#pragma once

#include <span>
#include <iostream>


#include "simpledb/log/LogMgr.hpp"
#include "simpledb/file/BlockId.hpp"
#include "simpledb/tx/recovery/log_record.hpp"

namespace simpledb::tx::recovery {

class   CheckpointLogRecord : public LogRecord {
public:
    virtual ~CheckpointLogRecord() noexcept override = default;
    CheckpointLogRecord();

    int op() const override;
    int txnum() const override;
    void undo(Transaction& tx) override;

    static int write_to_log(std::shared_ptr<simpledb::log::LogMgr> lm, int txnum);

    std::string to_string() const override;

private:
    int d_txnum;
};

} // namespace simpledb::tx::recovery 