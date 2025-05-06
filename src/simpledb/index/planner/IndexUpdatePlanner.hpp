#pragma once

#include <memory>
#include <string>

#include "simpledb/parse/parser.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/plan/update_planner.hpp"

namespace simpledb::index::planner {


class IndexUpdatePlanner : public simpledb::plan::UpdatePlanner {
public:

    explicit IndexUpdatePlanner(std::shared_ptr<simpledb::metadata::MetadataManager> mdm);

    int executeInsert(const simpledb::parse::InsertData& data, std::shared_ptr<simpledb::tx::Transaction> tx) override;
    int executeDelete(const simpledb::parse::DeleteData& data, std::shared_ptr<simpledb::tx::Transaction> tx) override;
    int executeModify(const simpledb::parse::ModifyData& data, std::shared_ptr<simpledb::tx::Transaction> tx) override;

    int executeCreateTable(const simpledb::parse::CreateTableData& data, std::shared_ptr<simpledb::tx::Transaction> tx) override;
    int executeCreateView(const simpledb::parse::CreateViewData& data, std::shared_ptr<simpledb::tx::Transaction> tx) override;
    int executeCreateIndex(const simpledb::parse::CreateIndexData& data, std::shared_ptr<simpledb::tx::Transaction> tx) override;

private:
    std::shared_ptr<simpledb::metadata::MetadataManager> d_mdm;
};

} // namespace simpledb::index::planner 