#pragma once

#include <memory>
#include <string>

#include "simpledb/file/block_id.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/index/btree/BTPage.hpp"
#include "simpledb/index/btree/DirEntry.hpp"

namespace simpledb::index::btree {


class BTreeDir {
public:

    BTreeDir(std::shared_ptr<simpledb::tx::Transaction> tx,
             simpledb::file::BlockId blk,
             std::shared_ptr<simpledb::record::Layout> layout);


    void close();
    int search(simpledb::query::Constant searchkey);
    void makeNewRoot(const DirEntry& e);
    std::optional<DirEntry> insert(const DirEntry& e);

private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Layout> d_layout;
    std::unique_ptr<BTPage> d_contents;
    std::string d_filename;

    std::optional<DirEntry> insertEntry(const DirEntry& e);
    simpledb::file::BlockId findChildBlock(const simpledb::query::Constant& searchkey);
};

} // namespace simpledb::index::btree 