#pragma once

#include "simpledb/record/rid.hpp"
#include "simpledb/query/constant.hpp"

namespace simpledb::index {

class Index {
public:
    virtual ~Index() = default;

    virtual void beforeFirst(const simpledb::query::Constant& dataval) = 0;
    virtual bool next() = 0;
    virtual simpledb::record::RID getDataRid() = 0;
    virtual void insert(const simpledb::query::Constant& dataval, const simpledb::record::RID& rid) = 0;
    virtual void delete_row(const simpledb::query::Constant& dataval, const simpledb::record::RID& rid) = 0;
    virtual void close() = 0;


};

} // namespace simpledb::index