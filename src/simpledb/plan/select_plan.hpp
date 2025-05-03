#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/query/predicate.hpp"
#include "simpledb/query/scan.hpp"

namespace simpledb::plan {

class SelectPlan: public Plan {
public:
    SelectPlan(std::shared_ptr<Plan> p, std::shared_ptr<query::Predicate> pred);
    ~SelectPlan() override;

    std::shared_ptr<query::Scan> open() override;
    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view field) const override;
    record::Schema& schema() const override;

private:
    std::shared_ptr<Plan> d_plan;
    std::shared_ptr<query::Predicate> d_pred;
};

} // namespace simpledb::plan