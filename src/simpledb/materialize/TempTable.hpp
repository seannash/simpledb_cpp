#pragma once

#include <memory>

#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/record/layout.hpp"

namespace simpledb::materialize {

class TempTable {
public:
    TempTable(std::shared_ptr<simpledb::tx::Transaction> tx, 
            std::shared_ptr<simpledb::record::Schema> schema);

    std::shared_ptr<simpledb::record::TableScan> open() const;

    std::string_view tableName() const;

    std::shared_ptr<simpledb::record::Layout> getLayout() const;

    static std::string nextTableName();

private:
    static std::mutex s_mutex;
    std::shared_ptr<simpledb::tx::Transaction> d_tx_;
    std::string d_tblname;
    std::shared_ptr<simpledb::record::Layout> d_layout;
};

}  // namespace simpledb::materialize