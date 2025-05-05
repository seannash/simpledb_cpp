#include "simpledb/index/btree/BTreeDir.hpp"
#include <optional>

namespace simpledb::index::btree {

BTreeDir::BTreeDir(std::shared_ptr<simpledb::tx::Transaction> tx,
                   simpledb::file::BlockId blk,
                   std::shared_ptr<simpledb::record::Layout> layout)
: d_tx(std::move(tx))
, d_layout(std::move(layout))
, d_contents(std::make_unique<BTPage>(d_tx, blk, d_layout))
, d_filename(blk.fileName())
{
}

void BTreeDir::close()
{
    d_contents->close();
}

int BTreeDir::search(simpledb::query::Constant searchkey)
{
    auto childblk = findChildBlock(searchkey);
    while (d_contents->getFlag() > 0) {
        d_contents->close();
        d_contents = std::make_unique<BTPage>(d_tx, childblk, d_layout);
        childblk = findChildBlock(searchkey);
    }
    return childblk.number();
}

void BTreeDir::makeNewRoot(const DirEntry& e)
{
    auto firstval = d_contents->getDataVal(0);
    int level = d_contents->getFlag();
    auto newblk = d_contents->split(0, level); // transfer all the records
    DirEntry oldroot(firstval, newblk.number());
    insertEntry(oldroot);
    insertEntry(e);
    d_contents->setFlag(level + 1);
}

std::optional<DirEntry> BTreeDir::insert(const DirEntry& e)
{
    if (d_contents->getFlag() == 0) {
        return insertEntry(e);
    }
    auto childblk = findChildBlock(e.dataVal());
    BTreeDir child(d_tx, childblk, d_layout);
    auto myentry = child.insert(e);
    child.close();
    return myentry ? insertEntry(*myentry) : std::nullopt;
}

std::optional<DirEntry> BTreeDir::insertEntry(const DirEntry& e)
{
    int newslot = 1 + d_contents->findSlotBefore(e.dataVal());
    d_contents->insertDir(newslot, e.dataVal(), e.blockNumber());
    if (!d_contents->isFull()) {
        return std::nullopt;
    }
    // else page is full, so split it
    int level = d_contents->getFlag();
    int splitpos = d_contents->getNumRecs() / 2;
    auto splitval = d_contents->getDataVal(splitpos);
    auto newblk = d_contents->split(splitpos, level);
    return DirEntry(splitval, newblk.number());
}

simpledb::file::BlockId BTreeDir::findChildBlock(const simpledb::query::Constant& searchkey)
{
    int slot = d_contents->findSlotBefore(searchkey);
    if (d_contents->getDataVal(slot + 1) == searchkey) {
        slot++;
    }
    int blknum = d_contents->getChildNum(slot);
    return simpledb::file::BlockId(d_filename, blknum);
}

} // namespace simpledb::index::btree
