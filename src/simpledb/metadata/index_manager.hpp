#pragma once

#include "simpledb/metadata/table_manager.hpp"
#include "simpledb/metadata/stat_manager.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/metadata/index_info.hpp"
#include "simpledb/record/layout.hpp"

#include <memory>
#include <string_view>
#include <map>
namespace simpledb::metadata {

class IndexManager {
public:
    IndexManager(bool is_new,
                std::shared_ptr<simpledb::metadata::TableManager> table_manager,
                std::shared_ptr<simpledb::metadata::StatManager> stat_manager,
                std::shared_ptr<simpledb::tx::Transaction> tx);
    ~IndexManager() = default;

    void create_index(std::string_view iname,
                      std::string_view tname,
                      std::string_view fname,
                      std::shared_ptr<simpledb::tx::Transaction> tx);

    std::map<std::string, IndexInfo> get_index_info(std::string_view tname, std::shared_ptr<simpledb::tx::Transaction> tx);

private:
    std::shared_ptr<simpledb::metadata::TableManager> d_table_manager;
    std::shared_ptr<simpledb::metadata::StatManager> d_stat_manager;
    std::shared_ptr<simpledb::record::Layout> d_layout;
};

} // namespace simpledb::metadata
