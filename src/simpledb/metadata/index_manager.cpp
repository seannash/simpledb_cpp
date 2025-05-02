#include "simpledb/metadata/index_manager.hpp"
#include <stdexcept>
#include "simpledb/record/table_scan.hpp"
namespace simpledb::metadata {

IndexManager::IndexManager(bool is_new,
                         std::shared_ptr<simpledb::metadata::TableManager> table_manager,
                         std::shared_ptr<simpledb::metadata::StatManager> stat_manager,
                         std::shared_ptr<simpledb::tx::Transaction> tx)
: d_table_manager(table_manager)
, d_stat_manager(stat_manager) {
    if (is_new) {
        auto schema = std::make_shared<simpledb::record::Schema>();
        schema->add_string_field("indexname", TableManager::MAX_NAME_LEN);
        schema->add_string_field("tablename", TableManager::MAX_NAME_LEN);
        schema->add_string_field("fieldname", TableManager::MAX_NAME_LEN);
        d_table_manager->create_table("idxcat", schema, tx);
    }
    d_layout = d_table_manager->get_layout("idxcat", tx);
}

void IndexManager::create_index(std::string_view iname,
                              std::string_view tname,
                              std::string_view fname,
                              std::shared_ptr<simpledb::tx::Transaction> tx) {
    simpledb::record::TableScan ts(tx, "idxcat", d_layout);
    ts.insert();
    ts.set_string("indexname", iname);
    ts.set_string("tablename", tname);
    ts.set_string("fieldname", fname);
    ts.close();
}

std::map<std::string, IndexInfo> IndexManager::get_index_info(std::string_view tname,
                                     std::shared_ptr<simpledb::tx::Transaction> tx) {
    std::map<std::string, IndexInfo> result;
    simpledb::record::TableScan ts(tx, "idxcat", d_layout);
    while (ts.next()) {
        if (ts.get_string("tablename") == tname) {
            std::string  indexname = ts.get_string("indexname");
            std::string fieldname = ts.get_string("fieldname");
            auto layout = d_table_manager->get_layout(tname, tx);
            StatInfo stat_info = d_stat_manager->get_stat_info(std::string(tname), layout, tx);
            // FIXME: Converting a reference to a shared_ptr is stuff of nightmares
            auto table_schema = std::make_shared<simpledb::record::Schema>(layout->schema());
            IndexInfo index_info(indexname,
                                 fieldname,
                                 table_schema,
                                 tx,
                                 stat_info);
            result.emplace(fieldname, index_info);
        }
    }
    ts.close();
    return result;
}

} // namespace simpledb::metadata
