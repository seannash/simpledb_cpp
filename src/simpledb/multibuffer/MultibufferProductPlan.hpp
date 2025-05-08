#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/materialize/TempTable.hpp"

#include <memory>

namespace simpledb::multibuffer {

class MultibufferProductPlan : public simpledb::plan::Plan {
public:

    MultibufferProductPlan(std::shared_ptr<simpledb::tx::Transaction> tx,
                          std::shared_ptr<simpledb::plan::Plan> lhs,
                          std::shared_ptr<simpledb::plan::Plan> rhs);

    std::shared_ptr<simpledb::query::Scan> open() override;

    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view field) const override;
    const simpledb::record::Schema& schema() const override;

private:
    simpledb::materialize::TempTable copy_records_from(std::shared_ptr<simpledb::plan::Plan> p);

    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::plan::Plan> d_lhs;
    std::shared_ptr<simpledb::plan::Plan> d_rhs;
    simpledb::record::Schema d_schema;
};

} // namespace simpledb::multibuffer