#pragma once

#include <memory>
#include <string>

#include "simpledb/index/Index.hpp"
#include "simpledb/file/block_id.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/record/rid.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/file/block_id.hpp"
#include "simpledb/index/btree/BTPage.hpp"
#include "simpledb/index/btree/BTreeLeaf.hpp"
namespace simpledb::index::btree {

class BTreeIndex : public simpledb::index::Index {
public:
    BTreeIndex(std::shared_ptr<simpledb::tx::Transaction> tx, const std::string& idxname, std::shared_ptr<simpledb::record::Layout> leafLayout);

    void beforeFirst(const simpledb::query::Constant& dataval) override;
    bool next() override;
    simpledb::record::RID getDataRid() override;
    void insert(const simpledb::query::Constant& key, const simpledb::record::RID& rid) override;
    void delete_row(const simpledb::query::Constant& key, const simpledb::record::RID& rid) override;
    void close() override;
    
private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Layout> d_dirLayout;
    std::shared_ptr<simpledb::record::Layout> d_leafLayout;
    std::string d_leaftbl;
    simpledb::file::BlockId d_root;
    std::unique_ptr<BTreeLeaf> d_leaf;
    simpledb::file::BlockId d_rootblk;
};

} // namespace simpledb::index::btree