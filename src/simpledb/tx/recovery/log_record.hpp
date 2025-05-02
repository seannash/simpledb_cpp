#pragma once

#include <span>
#include <memory>

#include "simpledb/tx/transaction.hpp"

namespace simpledb::tx::recovery {

class LogRecord {
public:
    enum {
        CHECKPOINT = 0,
        START,
        COMMIT,
        ROLLBACK,
        SET_INT,
        SET_STRING
    };

    virtual ~LogRecord() = default;

    virtual int op() const = 0;
    virtual int txnum() const = 0;
    virtual void undo(Transaction& tx) = 0;

    virtual std::string to_string() const = 0;

};

} // namespace simpledb::tx::recovery