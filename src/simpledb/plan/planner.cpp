#include "simpledb/plan/planner.hpp"

namespace simpledb::plan {

Planner::Planner(std::shared_ptr<QueryPlanner> qplanner, std::shared_ptr<UpdatePlanner> uplanner)
    : d_qplanner(std::move(qplanner))
    , d_uplanner(std::move(uplanner))
{
}

std::shared_ptr<Plan> Planner::createQueryPlan(const std::string& qry, std::shared_ptr<tx::Transaction> tx) {
    parse::Parser parser(qry);
    auto data = parser.query();
    return d_qplanner->create_plan(data, tx);
}

int Planner::executeUpdate(const std::string& cmd, std::shared_ptr<tx::Transaction> tx) {
    parse::Parser parser(cmd);
    auto data = parser.parse_update_command();
    
    // FIXME: Is this the best way?
    return std::visit([this, &tx](const auto& cmd) {
        using T = std::decay_t<decltype(cmd)>;
        if constexpr (std::is_same_v<T, parse::InsertData>) {
            return d_uplanner->executeInsert(cmd, tx);
        } else if constexpr (std::is_same_v<T, parse::DeleteData>) {
            return d_uplanner->executeDelete(cmd, tx);
        } else if constexpr (std::is_same_v<T, parse::ModifyData>) {
            return d_uplanner->executeModify(cmd, tx);
        } else if constexpr (std::is_same_v<T, parse::CreateTableData>) {
            return d_uplanner->executeCreateTable(cmd, tx);
        } else if constexpr (std::is_same_v<T, parse::CreateViewData>) {
            return d_uplanner->executeCreateView(cmd, tx);
        } else if constexpr (std::is_same_v<T, parse::CreateIndexData>) {
            return d_uplanner->executeCreateIndex(cmd, tx);
        }
        return 0;
    }, data);
}

} // namespace simpledb::plan