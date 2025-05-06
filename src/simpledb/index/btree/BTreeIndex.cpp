#include "simpledb/index/btree/BTreeIndex.hpp"
#include "simpledb/index/btree/BTPage.hpp"
#include "simpledb/index/btree/BTreeDir.hpp"
namespace simpledb::index::btree {

BTreeIndex::BTreeIndex(std::shared_ptr<simpledb::tx::Transaction> tx,
                       const std::string& idxname,
                       std::shared_ptr<simpledb::record::Layout> leafLayout)
    : d_tx(std::move(tx))
    , d_dirLayout(nullptr)
    , d_leafLayout(leafLayout)
    , d_leaftbl(idxname + "leaf")
    , d_root(simpledb::file::BlockId(d_leaftbl, 0))
    , d_leaf(nullptr)
    , d_rootblk(simpledb::file::BlockId(idxname + "dir", 0))
{
    
    if (d_tx->size(d_leaftbl) == 0) {
        auto blk = d_tx->append(d_leaftbl);
        BTPage node{d_tx, blk, d_leafLayout};
        node.format(blk, -1);
    }
    simpledb::record::Schema dirsch {};
    dirsch.add("block", d_leafLayout->schema());
    dirsch.add("dataval", d_leafLayout->schema());
    std::string dirtbl = idxname + "dir";
    d_dirLayout = std::make_shared<simpledb::record::Layout>(dirsch);
    if (d_tx->size(dirtbl) == 0) {
        d_tx->append(dirtbl);
        BTPage node{d_tx, d_rootblk, d_dirLayout};
        node.format(d_rootblk, -1);
        auto fldtype = dirsch.get_type("dataval");
        simpledb::query::Constant minval = (fldtype == jdbc::ColumnType::INT)
            ? simpledb::query::Constant(std::numeric_limits<int>::min())
            : simpledb::query::Constant("");
        node.insertDir(0, minval, 0);
        node.close();
    }
}

void BTreeIndex::beforeFirst(const simpledb::query::Constant& dataval)
{
    close();
    BTreeDir root{d_tx, d_rootblk, d_dirLayout};
    int blknum = root.search(dataval);
    root.close();;
    simpledb::file::BlockId leafblk{d_leaftbl, blknum};
    d_leaf = std::make_unique<BTreeLeaf>(d_tx, leafblk, d_leafLayout, dataval);
}

bool BTreeIndex::next()
{
    return d_leaf->next();
}

simpledb::record::RID BTreeIndex::getDataRid()
{
    return d_leaf->getDataRid();
}

void BTreeIndex::insert(const simpledb::query::Constant& key, const simpledb::record::RID& rid)
{
    beforeFirst(key);
    auto e = d_leaf->insert(rid);
    if (!e.has_value()) {
        return;
    }
    BTreeDir root(d_tx, d_rootblk, d_dirLayout);
    auto e2 = root.insert(e.value());
    if (e2.has_value()) {
        root.makeNewRoot(e2.value());
    root.close();        
    }
}

void BTreeIndex::delete_row(const simpledb::query::Constant& key, const simpledb::record::RID& rid)
{
    beforeFirst(key);
    d_leaf->delete_record(rid);
    d_leaf->close();
}

void BTreeIndex::close()
{
    if (d_leaf) {
        d_leaf->close();
    }
}

} // namespace simpledb::index::btree 