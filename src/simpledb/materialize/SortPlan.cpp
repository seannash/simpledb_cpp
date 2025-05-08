#include "simpledb/materialize/SortPlan.hpp"
#include "simpledb/materialize/MaterializationPlan.hpp"
#include "simpledb/materialize/TempTable.hpp"
#include "simpledb/materialize/RecordComparator.hpp"
#include "simpledb/materialize/SortScan.hpp"
#include <algorithm>
#include <cmath>

namespace simpledb::materialize {

SortPlan::SortPlan(std::shared_ptr<simpledb::plan::Plan> plan,
                  std::vector<std::string> sortFields,
                  std::shared_ptr<simpledb::tx::Transaction> tx)
: d_plan(std::move(plan))
, d_tx(std::move(tx))
, d_schema(std::make_shared<simpledb::record::Schema>(d_plan->schema())) // FIXME: unneeded conversion
, d_comp(sortFields)
{}

std::shared_ptr<simpledb::query::Scan> SortPlan::open() {
    auto src = d_plan->open();
    auto runs = splitIntoRuns(src);
    src->close();
    while (runs.size() > 2) {
        runs = doAllMergeIntervals(runs);
    }
    return std::make_shared<SortScan>(runs, d_comp);
}

int SortPlan::blocks_accessed() const {
    MaterializationPlan mp{d_tx, d_plan};
    return mp.blocks_accessed();
}

int SortPlan::records_output() const {
    return d_plan->records_output();
}

int SortPlan::distinct_values(std::string_view fldname) const {
    return d_plan->distinct_values(fldname);
}

const simpledb::record::Schema& SortPlan::schema() const {
    return d_plan->schema();
}

std::vector<std::shared_ptr<TempTable>> SortPlan::splitIntoRuns(std::shared_ptr<simpledb::query::Scan> src) {
    std::vector<std::shared_ptr<TempTable>> temps;
    src->before_first();
    if (!src->next()) {
        return temps;
    }
    auto currenttemp = std::make_shared<TempTable>(d_tx, d_schema);
    temps.push_back(currenttemp);
    auto currentscan = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(currenttemp->open());
    while (copy(*src, *currentscan))
    // Create initial sorted runs
    while (src->next()) {
        if (d_comp.compare(*src, *currentscan) < 0) {
            currentscan->close();
            currenttemp = std::make_shared<TempTable>(d_tx, d_schema);
            temps.push_back(currenttemp);
            currentscan = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(currenttemp->open());
        }

    }
    currentscan->close();
    return temps;
}

std::vector<std::shared_ptr<TempTable>> SortPlan::doAllMergeIntervals(std::vector<std::shared_ptr<TempTable>>& runs) {
    std::vector<std::shared_ptr<TempTable>> result {};
    while(runs.size() > 1) {
        // FIXME: removeing front is not efficient
        auto p1 = runs.front();
        runs.erase(runs.begin());
        auto p2 = runs.front();
        runs.erase(runs.begin());
        result.push_back(mergeTwoRuns(p1, p2));
    }
    if (runs.size() == 1) {
        result.push_back(runs.front());
    }
    return result;
}

std::shared_ptr<TempTable> SortPlan::mergeTwoRuns(std::shared_ptr<TempTable> t1, std::shared_ptr<TempTable> t2) {
    auto src1 = t1->open();
    auto src2 = t2->open();
    auto result = std::make_shared<TempTable>(d_tx, d_schema);
    auto dest = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(result->open());
    
    bool hasmore1 = src1->next();
    bool hasmore2 = src2->next();
    
    while (hasmore1 && hasmore2) {
        if (d_comp.compare(*src1, *src2) < 0) {
            hasmore1 = copy(*src1, *dest);
        } else {
            hasmore2 = copy(*src2, *dest);
        };
    }
    if (hasmore1) {
        while (hasmore1) {
            hasmore1 = copy(*src1, *dest);
        }
    } else {
        while (hasmore2) {
            hasmore2 = copy(*src2, *dest);
        }
    }
    dest->close();
    src1->close();
    src2->close();
    return result;
}

bool SortPlan::copy(simpledb::query::Scan& src, simpledb::query::UpdateScan& dest) {
    dest.insert();
    for (const auto& fldname : d_schema->fields()) {
        dest.set_val(fldname, src.get_val(fldname));
    }
    return src.next();
}

} // namespace simpledb::materialize 