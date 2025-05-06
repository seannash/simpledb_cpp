#pragma once

#include <memory>
#include <string_view>

#include "simpledb/index/Index.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/rid.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/record/table_scan.hpp"

namespace simpledb::index::hash {

class HashIndex : public Index {
public:
    HashIndex(std::shared_ptr<simpledb::tx::Transaction> tx, std::string_view index_name, std::shared_ptr<simpledb::record::Layout> index_layout);
    ~HashIndex() = default;

    void beforeFirst(const simpledb::query::Constant& dataval) override;

    bool next() override;

    simpledb::record::RID getDataRid() override;

    void insert(const simpledb::query::Constant& dataval, const simpledb::record::RID& rid) override;
    void delete_row(const simpledb::query::Constant& dataval, const simpledb::record::RID& rid) override;

    void close() override;

    static int searchCost(int numBlocks, int rpb);

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::string d_indexName;
    std::shared_ptr<simpledb::record::Layout> d_indexLayout;
    simpledb::query::Constant d_searchKey;
    std::unique_ptr<simpledb::record::TableScan> d_ts;
};

} // namespace simpledb::index::hash