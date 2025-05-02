#include "simpledb/metadata/table_manager.hpp"
#include <stdexcept>

namespace simpledb::metadata {

TableManager::TableManager(bool is_new, std::shared_ptr<simpledb::tx::Transaction> tx)
    : d_tx(tx) {
}

void TableManager::create_table(std::string_view table_name,
                              std::shared_ptr<simpledb::record::Schema> schema,
                              std::shared_ptr<simpledb::tx::Transaction> tx) {
}

std::shared_ptr<simpledb::record::Layout> TableManager::get_layout(
    std::string_view table_name,
    std::shared_ptr<simpledb::tx::Transaction> tx) const {
    return nullptr;
}

} // namespace simpledb::metadata
