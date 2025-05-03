#include "simpledb/plan/product_plan.hpp"
#include "simpledb/query/product_scan.hpp"

namespace simpledb::plan {

ProductPlan::ProductPlan(std::shared_ptr<Plan> p1, std::shared_ptr<Plan> p2)
    : d_plan1(p1)
    , d_plan2(p2)
{
    d_schema.add_all(p1->schema());
    d_schema.add_all(p2->schema());
}

ProductPlan::~ProductPlan() = default;

std::shared_ptr<query::Scan> ProductPlan::open() {
    std::shared_ptr<query::Scan> s1 = d_plan1->open();
    std::shared_ptr<query::Scan> s2 = d_plan2->open();
    return std::make_shared<query::ProductScan>(s1, s2);
}

int ProductPlan::blocks_accessed() const {
    return d_plan1->blocks_accessed() + (d_plan1->records_output() * d_plan2->blocks_accessed());
}

int ProductPlan::records_output() const {
    return d_plan1->records_output() * d_plan2->records_output();
}

int ProductPlan::distinct_values(std::string_view field) const {
    if (d_plan1->schema().has_field(field)) {
        return d_plan1->distinct_values(field);
    } else {
        return d_plan2->distinct_values(field);
    }
}

record::Schema& ProductPlan::schema() const {
    return const_cast<record::Schema&>(d_schema);
}

} // namespace simpledb::plan 