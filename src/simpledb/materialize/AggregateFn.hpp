#pragma once

#include "simpledb/query/scan.hpp"
#include "simpledb/query/constant.hpp"

#include <string>

namespace simpledb::materialize {

class AggregateFn {
public:

    virtual ~AggregateFn() = default;

    virtual simpledb::query::Constant processFirst(simpledb::query::Scan& s) = 0;
    virtual simpledb::query::Constant processNext(simpledb::query::Scan& s) = 0;
    virtual std::string fieldName() const = 0;
    virtual simpledb::query::Constant value() const = 0;
};

} // namespace simpledb::materialize