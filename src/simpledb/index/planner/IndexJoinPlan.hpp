#pragma once

#include <memory>
#include <string>

#include "simpledb/plan/plan.hpp"
#include "simpledb/metadata/index_info.hpp"
#include "simpledb/record/schema.hpp"

namespace simpledb::index::planner {


class IndexJoinPlan : public simpledb::plan::Plan {
public:

    IndexJoinPlan(std::shared_ptr<simpledb::plan::Plan> p1,
                  std::shared_ptr<simpledb::plan::Plan> p2,
                  std::shared_ptr<simpledb::metadata::IndexInfo> ii,
                  std::string joinfield);


    std::shared_ptr<simpledb::query::Scan> open() override;
    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view fldname) const override;
    const simpledb::record::Schema& schema() const override;

private:
    std::shared_ptr<simpledb::plan::Plan> d_p1;
    std::shared_ptr<simpledb::plan::Plan> d_p2;
    std::shared_ptr<simpledb::metadata::IndexInfo> d_ii;
    std::string d_joinfield;
    simpledb::record::Schema d_sch;
};

} // namespace simpledb::index::planner