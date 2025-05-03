#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace simpledb::plan {

class ProductPlan: public Plan {
public:
    ProductPlan(std::shared_ptr<Plan> p1, std::shared_ptr<Plan> p2);
    ~ProductPlan();

    std::shared_ptr<query::Scan> open() override;
    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view field) const override;
    record::Schema& schema() const override;

private:
    std::shared_ptr<Plan> d_plan1;
    std::shared_ptr<Plan> d_plan2;
    record::Schema d_schema;
};

} // namespace simpledb::plan