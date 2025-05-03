#include "simpledb/plan/basic_query_planner.hpp"
#include "simpledb/plan/table_plan.hpp"
#include "simpledb/plan/select_plan.hpp"
#include "simpledb/plan/project_plan.hpp"
#include "simpledb/plan/product_plan.hpp"
#include "simpledb/parser/parser.hpp"

namespace simpledb::plan {

BasicQueryPlanner::BasicQueryPlanner(std::shared_ptr<metadata::MetadataManager> metadata_manager)
    : d_metadata_manager(metadata_manager)
{
}

std::shared_ptr<Plan> BasicQueryPlanner::create_plan(
    const parse::QueryData& data,
    std::shared_ptr<tx::Transaction> tx)
{
    std::vector<std::shared_ptr<Plan>> plans;
    for (const auto& table_name : data.table_list()) {
        const auto viewdef = d_metadata_manager->get_view_def(table_name, tx);
        if (!viewdef.empty()) {
            simpledb::parse::Parser parser{viewdef};
            auto query_data = parser.query();
            auto plan = create_plan(query_data, tx);
            plans.push_back(plan);
        } else {
            plans.push_back(std::make_shared<TablePlan>(tx, table_name, d_metadata_manager));
        }
    }

    // Step 2: Create the product of all table plans
    std::shared_ptr<Plan> p = plans[0];
    for (size_t i = 1; i < plans.size(); i++) {
        p = std::make_shared<ProductPlan>(p, plans[i]);
    }

    // Step 3: Add a select plan for the predicate
    p = std::make_shared<SelectPlan>(p, data.predicate());

    // Step 4: Project on the field names
    return std::make_shared<ProjectPlan>(p, data.select_list());
}

} // namespace simpledb::plan 