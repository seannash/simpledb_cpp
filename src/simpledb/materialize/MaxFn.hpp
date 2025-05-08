#pragma once

#include "simpledb/materialize/AggregateFn.hpp"
#include "simpledb/query/constant.hpp"

namespace simpledb::materialize {

class MaxFn : public AggregateFn {
public:
    explicit MaxFn(std::string_view fieldname);

    simpledb::query::Constant processFirst(simpledb::query::Scan& s) override;
    simpledb::query::Constant processNext(simpledb::query::Scan& s) override;
    std::string fieldName() const override;
    simpledb::query::Constant value() const override;

private:
    std::string d_fldname;
    simpledb::query::Constant d_val;
};

} // namespace simpledb::materialize 