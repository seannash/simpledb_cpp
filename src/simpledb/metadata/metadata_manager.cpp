#include "simpledb/metadata/metadata_manager.hpp"
#include <stdexcept>

namespace simpledb::metadata {

MetadataManager::MetadataManager(bool is_new, std::shared_ptr<simpledb::tx::Transaction> tx) {
    d_table_manager = std::make_shared<TableManager>(is_new, tx);
    d_view_manager = std::make_shared<ViewManager>(is_new, tx);
    d_stat_manager = std::make_shared<StatManager>(is_new, tx);
    d_index_manager = std::make_shared<IndexManager>(is_new, tx);
}

void MetadataManager::create_table(
       std::string_view table_name,
       std::shared_ptr<simpledb::record::Schema> schema,
       std::shared_ptr<simpledb::tx::Transaction> tx) {
    d_table_manager->create_table(table_name, schema, tx);
}

std::shared_ptr<simpledb::record::Layout> MetadataManager::get_layout(
        std::string_view table_name,
        std::shared_ptr<simpledb::tx::Transaction> tx) {
    return d_table_manager->get_layout(table_name, tx);
}

void MetadataManager::create_view(
        std::string_view view_name,
        std::string_view view_def,
        std::shared_ptr<simpledb::tx::Transaction> tx) {
    d_view_manager->create_view(view_name, view_def, tx);
}

std::string MetadataManager::get_view_def(
        std::string_view view_name,
        std::shared_ptr<simpledb::tx::Transaction> tx) {
    return d_view_manager->get_view_def(view_name, tx);
}

void MetadataManager::create_index(
        std::string_view index_name,
        std::string_view table_name,
        std::string_view field_name,
        std::shared_ptr<simpledb::tx::Transaction> tx) {
    d_index_manager->create_index(index_name, table_name, field_name, tx);
}

std::map<std::string, IndexInfo> MetadataManager::get_index_info(
        std::string_view table_name,
        std::shared_ptr<simpledb::tx::Transaction> tx) {
    return d_index_manager->get_index_info(table_name, tx);
}

StatInfo MetadataManager::get_stat_info(
        std::string_view table_name,
        std::shared_ptr<simpledb::record::Layout> layout,
        std::shared_ptr<simpledb::tx::Transaction> tx) {
    return d_stat_manager->get_stat_info(table_name, layout, tx);
}

} // namespace simpledb::metadata
