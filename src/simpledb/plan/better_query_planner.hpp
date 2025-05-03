#pragma once

#include "simpledb/plan/query_planner.hpp"
#include "simpledb/metadata/metadata_manager.hpp"

#include <memory>

namespace simpledb::plan {

class BetterQueryPlanner: public QueryPlanner {
public:
    BetterQueryPlanner(std::shared_ptr<metadata::MetadataManager> metadata_manager);
    ~BetterQueryPlanner() = default;

    std::shared_ptr<Plan> create_plan(
        const parse::QueryData& data,
        std::shared_ptr<tx::Transaction> tx) override;

private:
    std::shared_ptr<metadata::MetadataManager> d_metadata_manager;
};

} // namespace simpledb::plan 