#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/materialize/RecordComparator.hpp"
#include "simpledb/materialize/TempTable.hpp"
#include <vector>
#include <memory>
#include <string_view>


namespace simpledb::materialize {

class SortPlan : public simpledb::plan::Plan {
public:
    SortPlan(std::shared_ptr<simpledb::plan::Plan> plan,
            std::vector<std::string> sortFields,
            std::shared_ptr<simpledb::tx::Transaction> tx);

    std::shared_ptr<simpledb::query::Scan> open() override;

    int blocks_accessed() const override;
    int records_output() const override;
    int distinct_values(std::string_view fldname) const override;
    const simpledb::record::Schema& schema() const override;

private:
    std::shared_ptr<simpledb::plan::Plan> d_plan;
    std::shared_ptr<simpledb::tx::Transaction> d_tx;
    std::shared_ptr<simpledb::record::Schema> d_schema;
    RecordComparator d_comp;

    std::vector<std::shared_ptr<TempTable>> splitIntoRuns(std::shared_ptr<simpledb::query::Scan> src);
    std::vector<std::shared_ptr<TempTable>> doAllMergeIntervals(std::vector<std::shared_ptr<TempTable>>& runs);
    std::shared_ptr<TempTable> mergeTwoRuns(std::shared_ptr<TempTable> t1, std::shared_ptr<TempTable> t2);
    bool copy(simpledb::query::Scan& src, simpledb::query::UpdateScan& dest);

};
}