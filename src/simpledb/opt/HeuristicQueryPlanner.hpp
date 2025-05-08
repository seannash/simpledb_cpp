#pragma once

#include "simpledb/plan/query_planner.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/parse/query_data.hpp"
#include "simpledb/plan/plan.hpp"
#include "simpledb/plan/planner.hpp"
#include "simpledb/plan/project_plan.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/opt/TablePlanner.hpp"

#include <memory>
#include <vector>
#include <string>

namespace simpledb::opt {


class HeuristicQueryPlanner : public simpledb::plan::QueryPlanner {
public:

    explicit HeuristicQueryPlanner(std::shared_ptr<simpledb::metadata::MetadataManager> mdm);


    std::shared_ptr<simpledb::plan::Plan> create_plan(
        const simpledb::parse::QueryData& data,
        std::shared_ptr<simpledb::tx::Transaction> tx) override;

    void set_planner(std::shared_ptr<simpledb::plan::Planner> p);

private:
    std::shared_ptr<simpledb::plan::Plan> get_lowest_select_plan();
    std::shared_ptr<simpledb::plan::Plan> get_lowest_join_plan(
        std::shared_ptr<simpledb::plan::Plan> current);
    std::shared_ptr<simpledb::plan::Plan> get_lowest_product_plan(
        std::shared_ptr<simpledb::plan::Plan> current);

    std::vector<std::shared_ptr<simpledb::opt::TablePlanner>> d_tableplanners;
    std::shared_ptr<simpledb::metadata::MetadataManager> d_mdm;
};

} // namespace simpledb::opt