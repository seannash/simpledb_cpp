#include "simpledb/index/btree/DirEntry.hpp"

namespace simpledb::index::btree {

DirEntry::DirEntry(simpledb::query::Constant dataval, int blocknum)
: d_dataval(std::move(dataval))
, d_blocknum(blocknum)
{}

const simpledb::query::Constant& DirEntry::dataVal() const
{
    return d_dataval;
}

int DirEntry::blockNumber() const
{
    return d_blocknum;
}

} // namespace simpledb::index::btree 