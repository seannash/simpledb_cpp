#pragma once

#include "simpledb/query/constant.hpp"
#include "simpledb/materialize/AggregateFn.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace simpledb::materialize {

class GroupValue {
public:
    GroupValue() = default;
    GroupValue(simpledb::query::Scan& s, const std::vector<std::string>& fields);

    simpledb::query::Constant getVal(const std::string& fldname);

    bool operator==(const GroupValue& other) const;
    bool operator!=(const GroupValue& other) const;

    int hashcode() const;

private:
    std::unordered_map<std::string, simpledb::query::Constant> d_vals;

};

} // namespace simpledb::materialize