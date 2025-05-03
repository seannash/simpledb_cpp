#include "simpledb/plan/project_plan.hpp"
#include "simpledb/query/project_scan.hpp"

namespace simpledb::plan {

ProjectPlan::ProjectPlan(std::shared_ptr<Plan> p, std::vector<std::string> field_list)
    : d_plan(p)
    , d_field_list(std::move(field_list))
{
    // Create a new schema containing only the specified fields
    for (const auto& field_name : d_field_list) {
        d_schema.add(field_name, d_plan->schema()); // FIXME:Should schema be a shared_ptr?
    }
}

ProjectPlan::~ProjectPlan() = default;

std::shared_ptr<query::Scan> ProjectPlan::open() {
    std::shared_ptr<query::Scan> s = d_plan->open();
    return std::make_shared<query::ProjectScan>(s, d_field_list);
}

int ProjectPlan::blocks_accessed() const {
    return d_plan->blocks_accessed();
}

int ProjectPlan::records_output() const {
    return d_plan->records_output();
}

int ProjectPlan::distinct_values(std::string_view field) const {
    return d_plan->distinct_values(field);
}

record::Schema& ProjectPlan::schema() const {
    return const_cast<record::Schema&>(d_schema);
}

} // namespace simpledb::plan 