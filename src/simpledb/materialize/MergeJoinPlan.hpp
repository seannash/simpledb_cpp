#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/tx/transaction.hpp"

#include <memory>
#include <string>
#include <vector>

namespace simpledb::materialize {


class MergeJoinPlan : public simpledb::plan::Plan {
public:

    MergeJoinPlan(std::shared_ptr<simpledb::tx::Transaction> tx,
                 std::shared_ptr<simpledb::plan::Plan> p1,
                 std::shared_ptr<simpledb::plan::Plan> p2,
                 std::string fldname1,
                 std::string fldname2);


    std::shared_ptr<simpledb::query::Scan> open() override;
    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view fldname) const override;
    const simpledb::record::Schema& schema() const override;

private:
    std::shared_ptr<simpledb::plan::Plan> d_p1;
    std::shared_ptr<simpledb::plan::Plan> d_p2;
    std::string d_fldname1;
    std::string d_fldname2;
    simpledb::record::Schema d_sch;
};

} // namespace simpledb::materialize
