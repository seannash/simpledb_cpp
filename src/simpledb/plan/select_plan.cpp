#include "simpledb/plan/select_plan.hpp"
#include "simpledb/query/select_scan.hpp"

#include <string>
#include <string_view>
#include <memory>

namespace simpledb::plan {

SelectPlan::SelectPlan(std::shared_ptr<Plan> p, std::shared_ptr<query::Predicate> pred)
    : d_plan(p)
    , d_pred(pred)
{
}

SelectPlan::~SelectPlan() = default;

std::shared_ptr<query::Scan> SelectPlan::open() {
    std::shared_ptr<query::Scan> s = d_plan->open();
    return std::make_shared<query::SelectScan>(s, d_pred);
}

int SelectPlan::blocks_accessed() const {
    return d_plan->blocks_accessed();
}

int SelectPlan::records_output() const {
    return d_plan->records_output() / d_pred->reduction_factor(*d_plan.get());
}

int SelectPlan::distinct_values(std::string_view field) const {
    if (!d_pred->equates_with_constant(field).is_null()) {
        return 1;
    } else if (!d_pred->equates_with_field(field).is_null()) {
        std::string_view other_field = d_pred->equates_with_field(field).as_string();
        return std::min(d_plan->distinct_values(field), d_plan->distinct_values(other_field));
    } else {
        return d_plan->distinct_values(field);
    }
}

const record::Schema& SelectPlan::schema() const {
    return d_plan->schema();
}

} // namespace simpledb::plan 