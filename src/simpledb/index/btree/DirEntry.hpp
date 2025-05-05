#pragma once

#include "simpledb/query/constant.hpp"

namespace simpledb::index::btree {

class DirEntry {
public:

    DirEntry(simpledb::query::Constant dataval, int blocknum);


    const simpledb::query::Constant& dataVal() const ;

    int blockNumber() const;

private:
    simpledb::query::Constant d_dataval;
    int d_blocknum;
};

} // namespace simpledb::index::btree
