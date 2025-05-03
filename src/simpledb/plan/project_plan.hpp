#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"

#include <string>
#include <vector>
#include <memory>

namespace simpledb::plan {

class ProjectPlan: public Plan {

public:
    ProjectPlan(std::shared_ptr<Plan> p, std::vector<std::string> field_list);
    ~ProjectPlan() override;

    std::shared_ptr<query::Scan> open() override;

    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view field) const override;
    record::Schema& schema() const override;

private:
    std::shared_ptr<Plan> d_plan;
    std::vector<std::string> d_field_list;
    record::Schema d_schema;
};

} // namespace simpledb::plan