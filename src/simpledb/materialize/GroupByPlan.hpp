#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/materialize/AggregateFn.hpp"
#include "simpledb/tx/transaction.hpp"

#include <vector>
#include <memory>
#include <string_view>

namespace simpledb::materialize {

class GroupByPlan : public simpledb::plan::Plan {
public:
    GroupByPlan(std::shared_ptr<simpledb::tx::Transaction> tx,
                std::shared_ptr<simpledb::plan::Plan> p,
                std::vector<std::string> groupfields,
                std::vector<std::shared_ptr<AggregateFn>> aggfns);

    std::shared_ptr<simpledb::query::Scan> open() override;
    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view field) const override;
    const simpledb::record::Schema& schema() const override;

private:
    std::shared_ptr<simpledb::plan::Plan> d_plan;
    std::vector<std::string> d_groupfields;
    std::vector<std::shared_ptr<AggregateFn>> d_aggfns;
    simpledb::record::Schema d_schema;
};

} // namespace simpledb::materialize
