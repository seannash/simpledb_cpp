#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"

namespace simpledb::materialize {

class MaterializePlan : public simpledb::plan::Plan {
public:
    MaterializePlan(std::shared_ptr<simpledb::tx::Transaction> tx, 
                        std::shared_ptr<simpledb::plan::Plan> srcplan);

    std::shared_ptr<simpledb::query::Scan> open() override;
    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view fldname) const override;

    const simpledb::record::Schema& schema() const override;
    
private:
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::plan::Plan> d_srcplan;

};
} // namespace simpledb::materialize
