#include "simpledb/index/planner/IndexJoinPlan.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/index/query/IndexJoinScan.hpp"

namespace simpledb::index::planner {

IndexJoinPlan::IndexJoinPlan(std::shared_ptr<simpledb::plan::Plan> p1,
                            std::shared_ptr<simpledb::plan::Plan> p2,
                            std::shared_ptr<simpledb::metadata::IndexInfo> ii,
                            std::string joinfield)
: d_p1(std::move(p1))
, d_p2(std::move(p2))
, d_ii(std::move(ii))
, d_joinfield(std::move(joinfield))
{
    d_sch.add_all(d_p1->schema());
    d_sch.add_all(d_p2->schema());
}

std::shared_ptr<simpledb::query::Scan> IndexJoinPlan::open()
{
    auto s = d_p1->open();
    // throws an exception if p2 is not a tableplan
    auto ts = std::dynamic_pointer_cast<simpledb::record::TableScan>(d_p2->open());
    auto idx = d_ii->open();
    return std::make_shared<simpledb::index::query::IndexJoinScan>(s, idx, d_joinfield, ts);
}

int IndexJoinPlan::blocks_accessed() const
{
    return d_p1->blocks_accessed() 
        + (d_p1->records_output() * d_ii->blocks_accessed())
        + records_output();
}

int IndexJoinPlan::records_output() const
{
    return d_p1->records_output() * d_ii->records_output();
}

int IndexJoinPlan::distinct_values(std::string_view fldname) const
{
    if (d_p1->schema().has_field(fldname)) {
        return d_p1->distinct_values(fldname);
    } else {
        return d_p2->distinct_values(fldname);
    }
}

const simpledb::record::Schema& IndexJoinPlan::schema() const
{
    return d_sch;
}

} // namespace simpledb::index::planner 