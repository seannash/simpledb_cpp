#pragma once

#include "simpledb/plan/plan.hpp"
#include "simpledb/parser/query_data.hpp"
#include "simpledb/tx/transaction.hpp"

#include <memory>

namespace simpledb::plan {

/**
 * The interface implemented by planners for the SQL select statement.
 */
class QueryPlanner {
public:
    virtual ~QueryPlanner() = default;  

    virtual std::shared_ptr<Plan> create_plan(
        const parse::QueryData& data,
        std::shared_ptr<tx::Transaction> tx) = 0;
};

} // namespace simpledb::plan