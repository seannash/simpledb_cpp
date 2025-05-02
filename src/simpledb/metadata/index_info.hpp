#pragma once

#include "simpledb/tx/transaction.hpp"
#include <string_view>
#include <memory>
#include "stat_info.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"

namespace simpledb::metadata {

// TODO: Placeholder for the index class
class Index {
public:
    Index(std::shared_ptr<simpledb::tx::Transaction> tx, std::string_view index_name, std::shared_ptr<simpledb::record::Layout> index_layout);
    ~Index() = default;

    static int search_cost(int x, int y);
};

class IndexInfo {
public:
    IndexInfo(std::string_view iname,
              std::string_view fname,
              std::shared_ptr<simpledb::record::Schema> table_schema,
              std::shared_ptr<simpledb::tx::Transaction> tx,
              StatInfo stat_info);
    ~IndexInfo() = default;

    std::unique_ptr<Index> open();

    int blocks_accessed() const;
    int records_output() const;
    int distinct_values(std::string_view fname) const;

private:
    std::string d_index_name;
    std::string d_field_name;
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Schema> d_table_schema;
    std::shared_ptr<simpledb::record::Layout> d_index_layout;
    StatInfo d_stat_info;

    std::shared_ptr<simpledb::record::Layout> create_index_layout();

};

} // namespace simpledb::metadata   