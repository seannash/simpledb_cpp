#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/metadata/index_info.hpp"

namespace simpledb::index::planner {

class IndexSelectPlan : public simpledb::plan::Plan {
public:
    IndexSelectPlan(std::shared_ptr<simpledb::plan::Plan> p, std::shared_ptr<simpledb::metadata::IndexInfo> ii, const simpledb::query::Constant& val);

    std::shared_ptr<simpledb::query::Scan> open() override;
    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view field) const override;
    record::Schema& schema() const override;

private:
    std::shared_ptr<simpledb::plan::Plan> d_plan;
    std::shared_ptr<simpledb::metadata::IndexInfo> d_ii;
    simpledb::query::Constant d_val;
};

} // namespace simpledb::index::planner