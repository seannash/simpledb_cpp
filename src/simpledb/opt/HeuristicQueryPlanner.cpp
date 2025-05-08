#include "simpledb/opt/HeuristicQueryPlanner.hpp"
#include "simpledb/opt/TablePlanner.hpp"
#include <algorithm>

namespace simpledb::opt {

HeuristicQueryPlanner::HeuristicQueryPlanner(
    std::shared_ptr<simpledb::metadata::MetadataManager> mdm)
    : d_mdm(std::move(mdm))
{
}

std::shared_ptr<simpledb::plan::Plan> HeuristicQueryPlanner::create_plan(
      const simpledb::parse::QueryData& data,
      std::shared_ptr<simpledb::tx::Transaction> tx) {
    
    // Step 1: Create a TablePlanner object for each mentioned table
    // FIXME: Should predicate be a pointer?
    for (const auto& tblname : data.table_list()) {
        d_tableplanners.push_back(std::make_shared<TablePlanner>(
            tblname, *data.predicate(), tx, d_mdm));
    }
    
    // Step 2: Choose the lowest-size plan to begin the join order
    auto currentplan = get_lowest_select_plan();
    
    // Step 3: Repeatedly add a plan to the join order
    while (!d_tableplanners.empty()) {
        auto p = get_lowest_join_plan(currentplan);
        if (p) {
            currentplan = p;
        } else {  // no applicable join
            currentplan = get_lowest_product_plan(currentplan);
        }
    }
    
    // Step 4: Project on the field names and return
    return std::make_shared<simpledb::plan::ProjectPlan>(currentplan, data.select_list());
}

std::shared_ptr<simpledb::plan::Plan> HeuristicQueryPlanner::get_lowest_select_plan() {
    std::shared_ptr<TablePlanner> besttp;
    std::shared_ptr<simpledb::plan::Plan> bestplan;
    
    for (const auto& tp : d_tableplanners) {
        auto plan = tp->make_select_plan();
        if (!bestplan || plan->records_output() < bestplan->records_output()) {
            besttp = tp;
            bestplan = plan;
        }
    }
    
    if (besttp) {
        d_tableplanners.erase(
            std::remove(d_tableplanners.begin(), d_tableplanners.end(), besttp),
            d_tableplanners.end());
    }
    
    return bestplan;
}

std::shared_ptr<simpledb::plan::Plan> HeuristicQueryPlanner::get_lowest_join_plan(
    std::shared_ptr<simpledb::plan::Plan> current) {
    std::shared_ptr<TablePlanner> besttp;
    std::shared_ptr<simpledb::plan::Plan> bestplan;
    
    for (const auto& tp : d_tableplanners) {
        auto plan = tp->make_join_plan(current);
        if (plan && (!bestplan || plan->records_output() < bestplan->records_output())) {
            besttp = tp;
            bestplan = plan;
        }
    }
    
    if (besttp) {
        d_tableplanners.erase(
            std::remove(d_tableplanners.begin(), d_tableplanners.end(), besttp),
            d_tableplanners.end());
    }
    
    return bestplan;
}

std::shared_ptr<simpledb::plan::Plan> HeuristicQueryPlanner::get_lowest_product_plan(
    std::shared_ptr<simpledb::plan::Plan> current) {
    std::shared_ptr<TablePlanner> besttp;
    std::shared_ptr<simpledb::plan::Plan> bestplan;
    
    for (const auto& tp : d_tableplanners) {
        auto plan = tp->make_product_plan(current);
        if (!bestplan || plan->records_output() < bestplan->records_output()) {
            besttp = tp;
            bestplan = plan;
        }
    }
    
    if (besttp) {
        d_tableplanners.erase(
            std::remove(d_tableplanners.begin(), d_tableplanners.end(), besttp),
            d_tableplanners.end());
    }
    
    return bestplan;
}

void HeuristicQueryPlanner::set_planner(std::shared_ptr<simpledb::plan::Planner> p) {
    // for use in planning views, which
    // for simplicity this code doesn't do.
}

} // namespace simpledb::opt 