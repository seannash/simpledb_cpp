
#pragma once

#include "simpledb/plan/query_planner.hpp"
#include "simpledb/metadata/metadata_manager.hpp"
#include "simpledb/parse/insert_data.hpp"
#include "simpledb/parse/delete_data.hpp"
#include "simpledb/parse/modify_data.hpp"
#include "simpledb/parse/create_table_data.hpp"
#include "simpledb/parse/create_view_data.hpp"
#include "simpledb/parse/create_index_data.hpp"

#include <memory>

namespace simpledb::plan {

class UpdatePlanner {
public:
    virtual ~UpdatePlanner() = default;

    virtual int executeInsert(const simpledb::parse::InsertData& data, std::shared_ptr<simpledb::tx::Transaction> tx) = 0;
    virtual int executeDelete(const simpledb::parse::DeleteData& data, std::shared_ptr<simpledb::tx::Transaction> tx) = 0;
    virtual int executeModify(const simpledb::parse::ModifyData& data, std::shared_ptr<simpledb::tx::Transaction> tx) = 0;
    virtual int executeCreateTable(const simpledb::parse::CreateTableData& data, std::shared_ptr<simpledb::tx::Transaction> tx) = 0;
    virtual int executeCreateView(const simpledb::parse::CreateViewData& data, std::shared_ptr<simpledb::tx::Transaction> tx) = 0;
    virtual int executeCreateIndex(const simpledb::parse::CreateIndexData& data, std::shared_ptr<simpledb::tx::Transaction> tx) = 0;

};

} // namespace simpledb::plan