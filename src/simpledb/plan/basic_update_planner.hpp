
#pragma once

#include "simpledb/plan/update_planner.hpp"

namespace simpledb::plan {

class BasicUpdatePlanner: public UpdatePlanner {
public:
    BasicUpdatePlanner(std::shared_ptr<metadata::MetadataManager> metadata_manager);
    ~BasicUpdatePlanner() = default;

    int executeInsert(const parse::InsertData& data, std::shared_ptr<tx::Transaction> tx) override;
    int executeDelete(const parse::DeleteData& data, std::shared_ptr<tx::Transaction> tx) override;
    int executeModify(const parse::ModifyData& data, std::shared_ptr<tx::Transaction> tx) override;
    int executeCreateTable(const parse::CreateTableData& data, std::shared_ptr<tx::Transaction> tx) override;
    int executeCreateView(const parse::CreateViewData& data, std::shared_ptr<tx::Transaction> tx) override;
    int executeCreateIndex(const parse::CreateIndexData& data, std::shared_ptr<tx::Transaction> tx) override;

private:
    std::shared_ptr<metadata::MetadataManager> d_metadata_manager;
};

} // namespace simpledb::plan