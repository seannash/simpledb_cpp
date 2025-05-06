#include "simpledb/index/hash/HashIndex.hpp"

namespace {
const int NUM_BUCKETS = 100;
}

namespace simpledb::index::hash {

HashIndex::HashIndex(std::shared_ptr<simpledb::tx::Transaction> tx, 
                    std::string_view index_name, 
                    std::shared_ptr<simpledb::record::Layout> index_layout)
    : d_tx(std::move(tx))
    , d_indexName(index_name)
    , d_indexLayout(std::move(index_layout))
    , d_searchKey()
    , d_ts(nullptr)
{}

void HashIndex::beforeFirst(const simpledb::query::Constant& dataval)
{
    close();
    d_searchKey = dataval;
    int bucket = d_searchKey.hashCode() % NUM_BUCKETS;
    std::string tableName = d_indexName + std::to_string(bucket);
    d_ts = std::make_unique<simpledb::record::TableScan>(d_tx, tableName, d_indexLayout);
}

bool HashIndex::next()
{
    while (d_ts->next()) {
        if (d_ts->get_val("dataval") == d_searchKey) {
            return true;
        }
    }
    return false;
}

simpledb::record::RID HashIndex::getDataRid()
{
    int blknum = d_ts->get_int("block");
    int id = d_ts->get_int("id");
    return simpledb::record::RID(blknum, id);
}

void HashIndex::insert(const simpledb::query::Constant& dataval, 
                      const simpledb::record::RID& rid)                   
{
    std::cout << "Inserting " << dataval.to_string() << " into " << d_indexName << std::endl;
    beforeFirst(dataval);
    d_ts->insert();
    d_ts->set_int("block", rid.block_number());
    d_ts->set_int("id", rid.slot());
    d_ts->set_val("dataval", dataval);
}

void HashIndex::delete_row(const simpledb::query::Constant& dataval, 
                           const simpledb::record::RID& rid)
{
    beforeFirst(dataval);
    while (next()) {
        if (getDataRid() == rid) {
            d_ts->delete_record();
            return;
        }
    }
}

void HashIndex::close()
{
    if (d_ts) {
        d_ts->close();
    }
}

int HashIndex::searchCost(int numBlocks, int rpb)
{
    return numBlocks / NUM_BUCKETS;
}

} // namespace simpledb::index::hash