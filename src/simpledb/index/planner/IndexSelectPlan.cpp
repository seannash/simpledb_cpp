#include "simpledb/index/planner/IndexSelectPlan.hpp"
#include "simpledb/index/Index.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/index/planner/IndexSelectScan.hpp"

namespace simpledb::index::planner {

IndexSelectPlan::IndexSelectPlan(std::shared_ptr<simpledb::plan::Plan> p,
                                std::shared_ptr<simpledb::metadata::IndexInfo> ii,
                                const simpledb::query::Constant& val)
    : d_plan(std::move(p))
    , d_ii(std::move(ii))
    , d_val(val)
{
}

std::shared_ptr<simpledb::query::Scan> IndexSelectPlan::open()
{
    auto ts = std::dynamic_pointer_cast<simpledb::record::TableScan>(d_plan->open());
    auto idx = d_ii->open();
    return std::make_shared<simpledb::index::planner::IndexSelectScan>(ts, idx, d_val);
}

int IndexSelectPlan::blocks_accessed() const
{
    return d_ii->blocks_accessed() + records_output();
}

int IndexSelectPlan::records_output() const
{
    return d_ii->records_output();
}

int IndexSelectPlan::distinct_values(std::string_view field) const
{
    return d_ii->distinct_values(field);
}

simpledb::record::Schema& IndexSelectPlan::schema() const
{
    return d_plan->schema();
}

} // namespace simpledb::index::planner 