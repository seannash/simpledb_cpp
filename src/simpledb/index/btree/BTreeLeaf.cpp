#include "simpledb/index/btree/BTreeLeaf.hpp"
#include <optional>

namespace simpledb::index::btree {

BTreeLeaf::BTreeLeaf(std::shared_ptr<simpledb::tx::Transaction> tx,
                     simpledb::file::BlockId blk,
                     std::shared_ptr<simpledb::record::Layout> layout,
                     simpledb::query::Constant searchkey)
    : d_tx(std::move(tx))
    , d_layout(std::move(layout))
    , d_searchkey(std::move(searchkey))
    , d_contents(std::make_unique<BTPage>(d_tx, blk, d_layout))
    , d_currentslot(d_contents->findSlotBefore(d_searchkey))
    , d_filename(blk.fileName())
{
}

void BTreeLeaf::close()
{
    d_contents->close();
}

bool BTreeLeaf::next()
{
    d_currentslot++;
    if (d_currentslot >= d_contents->getNumRecs()) {
        return tryOverflow();
    } else if (d_contents->getDataVal(d_currentslot) == d_searchkey) {
        return true;
    } else {
        return tryOverflow();
    }
}

simpledb::record::RID BTreeLeaf::getDataRid() const
{
    return d_contents->getDataRid(d_currentslot);
}

void BTreeLeaf::delete_record(const simpledb::record::RID& datarid)
{
    while (next()) {
        if (getDataRid() == datarid) {
            d_contents->delete_record(d_currentslot);
            return;
        }
    }
}

std::optional<DirEntry> BTreeLeaf::insert(const simpledb::record::RID& datarid)
{
    if (d_contents->getFlag() >= 0 && d_contents->getDataVal(0) > d_searchkey) {
        auto firstval = d_contents->getDataVal(0);
        auto newblk = d_contents->split(0, d_contents->getFlag());
        d_currentslot = 0;
        d_contents->setFlag(-1);
        d_contents->insertLeaf(d_currentslot, d_searchkey, datarid);
        return DirEntry(firstval, newblk.number());
    }

    d_currentslot++;
    d_contents->insertLeaf(d_currentslot, d_searchkey, datarid);
    if (!d_contents->isFull()) {
        return std::nullopt;
    }

    // else page is full, so split it
    auto firstkey = d_contents->getDataVal(0);
    auto lastkey = d_contents->getDataVal(d_contents->getNumRecs() - 1);
    if (lastkey == firstkey) {
        // create an overflow block to hold all but the first record
        auto newblk = d_contents->split(1, d_contents->getFlag());
        d_contents->setFlag(newblk.number());
        return std::nullopt;
    } else {
        int splitpos = d_contents->getNumRecs() / 2;
        auto splitkey = d_contents->getDataVal(splitpos);
        if (splitkey == firstkey) {
            // move right, looking for the next key
            while (d_contents->getDataVal(splitpos) == splitkey) {
                splitpos++;
            }
            splitkey = d_contents->getDataVal(splitpos);
        } else {
            // move left, looking for first entry having that key
            while (d_contents->getDataVal(splitpos - 1) == splitkey) {
                splitpos--;
            }
        }
        auto newblk = d_contents->split(splitpos, -1);
        return DirEntry(splitkey, newblk.number());
    }
}

bool BTreeLeaf::tryOverflow()
{
    auto firstkey = d_contents->getDataVal(0);
    int flag = d_contents->getFlag();
    if (d_searchkey != firstkey || flag < 0) {
        return false;
    }
    d_contents->close();
    simpledb::file::BlockId nextblk(d_filename, flag);
    d_contents = std::make_unique<BTPage>(d_tx, nextblk, d_layout);
    d_currentslot = 0;
    return true;
}

} // namespace simpledb::index::btree 