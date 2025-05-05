#pragma once

#include <memory>
#include <optional>
#include <string>

#include "simpledb/file/block_id.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/rid.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/index/btree/BTPage.hpp"
#include "simpledb/index/btree/DirEntry.hpp"

namespace simpledb::index::btree {

class BTreeLeaf {
public:

    BTreeLeaf(std::shared_ptr<simpledb::tx::Transaction> tx,
              simpledb::file::BlockId blk,
              std::shared_ptr<simpledb::record::Layout> layout,
              simpledb::query::Constant searchkey);


    void close();


    bool next();
    simpledb::record::RID getDataRid() const;
    void delete_record(const simpledb::record::RID& datarid);
    std::optional<DirEntry> insert(const simpledb::record::RID& datarid);

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Layout> d_layout;
    simpledb::query::Constant d_searchkey;
    std::unique_ptr<BTPage> d_contents;
    int d_currentslot;
    std::string d_filename;

    bool tryOverflow();
};

} // namespace simpledb::index::btree