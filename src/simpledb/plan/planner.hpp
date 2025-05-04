#pragma once

#include "simpledb/plan/query_planner.hpp"
#include "simpledb/plan/update_planner.hpp"
#include "simpledb/parser/parser.hpp"
#include "simpledb/parser/query_data.hpp"
#include "simpledb/parser/insert_data.hpp"
#include "simpledb/parser/delete_data.hpp"
#include "simpledb/parser/modify_data.hpp"
#include "simpledb/parser/create_table_data.hpp"
#include "simpledb/parser/create_view_data.hpp"
#include "simpledb/parser/create_index_data.hpp"
#include "simpledb/tx/transaction.hpp"

#include <memory>
#include <string>
#include <variant>

namespace simpledb::plan {

class Planner {
public:
    Planner(std::shared_ptr<QueryPlanner> qplanner, std::shared_ptr<UpdatePlanner> uplanner);
    ~Planner() = default;

    std::shared_ptr<Plan> createQueryPlan(const std::string& qry, std::shared_ptr<tx::Transaction> tx);
    int executeUpdate(const std::string& cmd, std::shared_ptr<tx::Transaction> tx);

private:
    std::shared_ptr<QueryPlanner> d_qplanner;
    std::shared_ptr<UpdatePlanner> d_uplanner;

};

} // namespace simpledb::plan 