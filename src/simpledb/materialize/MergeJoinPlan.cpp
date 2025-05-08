#include "simpledb/materialize/MergeJoinPlan.hpp"
#include "simpledb/materialize/SortPlan.hpp"
#include "simpledb/materialize/MergeJoinScan.hpp"
#include <algorithm>

namespace simpledb::materialize {

MergeJoinPlan::MergeJoinPlan(std::shared_ptr<simpledb::tx::Transaction> tx,
                           std::shared_ptr<simpledb::plan::Plan> p1,
                           std::shared_ptr<simpledb::plan::Plan> p2,
                           std::string fldname1,
                           std::string fldname2)
    : d_fldname1(std::move(fldname1))
    , d_fldname2(std::move(fldname2))
{
    std::vector<std::string> sortlist1{d_fldname1};
    d_p1 = std::make_shared<SortPlan>(p1, sortlist1, tx);
    std::vector<std::string> sortlist2{d_fldname2};
    d_p2 = std::make_shared<SortPlan>(p2, sortlist2, tx);
    d_sch = d_p1->schema();
    d_sch.add_all(d_p2->schema());
}

std::shared_ptr<simpledb::query::Scan> MergeJoinPlan::open() {
    auto s1 = d_p1->open();
    auto s2 = std::dynamic_pointer_cast<SortScan>(d_p2->open());
    return std::make_shared<MergeJoinScan>(std::move(s1), std::move(s2), d_fldname1, d_fldname2);
}

int MergeJoinPlan::blocks_accessed() const {
    return d_p1->blocks_accessed() + d_p2->blocks_accessed();
}

int MergeJoinPlan::records_output() const {
    int maxvals = std::max(d_p1->distinct_values(d_fldname1),
                          d_p2->distinct_values(d_fldname2));
    return (d_p1->records_output() * d_p2->records_output()) / maxvals;
}

int MergeJoinPlan::distinct_values(std::string_view fldname) const {
    if (d_p1->schema().has_field(fldname)) {
        return d_p1->distinct_values(fldname);
    } else {
        return d_p2->distinct_values(fldname);
    }
}

const simpledb::record::Schema& MergeJoinPlan::schema() const {
    return d_sch;
}

} // namespace simpledb::materialize 