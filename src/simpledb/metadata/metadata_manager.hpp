#pragma once

#include "simpledb/record/schema.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/metadata/table_manager.hpp"
#include "simpledb/metadata/stat_manager.hpp"
#include "simpledb/metadata/index_manager.hpp"
#include "simpledb/metadata/view_manager.hpp"
#include <memory>
#include <string_view>

namespace simpledb::metadata {

class MetadataManager {
public:
    MetadataManager(bool is_new, std::shared_ptr<simpledb::tx::Transaction> tx);

    ~MetadataManager() = default;

    void create_table(
        std::string_view table_name,
        std::shared_ptr<simpledb::record::Schema> schema,
        std::shared_ptr<simpledb::tx::Transaction> tx);

    std::shared_ptr<simpledb::record::Layout> get_layout(
        std::string_view table_name,
        std::shared_ptr<simpledb::tx::Transaction> tx);
    
    void create_view(
        std::string_view view_name,
        std::string_view view_def,
        std::shared_ptr<simpledb::tx::Transaction> tx);

    std::string get_view_def(
        std::string_view view_name,
        std::shared_ptr<simpledb::tx::Transaction> tx);

    void create_index(
        std::string_view index_name,
        std::string_view table_name,
        std::string_view field_name,
        std::shared_ptr<simpledb::tx::Transaction> tx);

    std::map<std::string, IndexInfo> get_index_info(
        std::string_view table_name,
        std::shared_ptr<simpledb::tx::Transaction> tx);

    StatInfo get_stat_info(
        std::string_view table_name,
        std::shared_ptr<simpledb::record::Layout> layout,
        std::shared_ptr<simpledb::tx::Transaction> tx);

private:
    std::shared_ptr<simpledb::metadata::TableManager> d_table_manager;
    std::shared_ptr<simpledb::metadata::ViewManager> d_view_manager;
    std::shared_ptr<simpledb::metadata::StatManager> d_stat_manager;
    std::shared_ptr<simpledb::metadata::IndexManager> d_index_manager;
};
} // namespace simpledb::metadata