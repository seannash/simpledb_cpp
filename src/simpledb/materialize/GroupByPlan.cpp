#include "simpledb/materialize/GroupByPlan.hpp"
#include "simpledb/materialize/GroupByScan.hpp"

#include <algorithm>
namespace simpledb::materialize {

GroupByPlan::GroupByPlan(std::shared_ptr<simpledb::tx::Transaction> tx,
                        std::shared_ptr<simpledb::plan::Plan> p,
                        std::vector<std::string> groupfields,
                        std::vector<std::shared_ptr<AggregateFn>> aggfns)
    : d_plan(std::move(p))
    , d_groupfields(std::move(groupfields))
    , d_aggfns(std::move(aggfns))
{
   for (const auto& fldname : d_groupfields) {
      d_schema.add(fldname, d_plan->schema());
   }
   for (const auto& fn : d_aggfns) {
      d_schema.add_int_field(fn->fieldName());
   }
}

std::shared_ptr<simpledb::query::Scan> GroupByPlan::open() {
    auto s = d_plan->open();
    return std::make_shared<GroupByScan>(s, d_groupfields, d_aggfns);
}

int GroupByPlan::blocks_accessed() const {
    return d_plan->blocks_accessed();
}

int GroupByPlan::records_output() const {
    int numgroups = 1;
    for (const auto& fldname : d_groupfields) {
        numgroups *= d_plan->distinct_values(fldname);
    }
    return numgroups;
}

int GroupByPlan::distinct_values(std::string_view field) const {
    if (std::find(d_groupfields.begin(), d_groupfields.end(), field) != d_groupfields.end()) {
        return d_plan->distinct_values(field);
    }
    return records_output();
}

const simpledb::record::Schema& GroupByPlan::schema() const {
    return d_schema;
}

} // namespace simpledb::materialize 