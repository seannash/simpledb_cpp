#include "simpledb/metadata/view_manager.hpp"
#include <stdexcept>

#include "simpledb/record/schema.hpp"
#include "simpledb/record/layout.hpp"
#include "simpledb/record/record_page.hpp"
#include "simpledb/record/schema.hpp"
#include "jdbc/column_types.hpp"
#include "simpledb/record/table_scan.hpp"
namespace simpledb::metadata {

namespace {
const std::string VIEWCAT_VIEWDEF_LEN = "viewdef_len";
const int MAX_VIEW_DEF = 100;
}

ViewManager::ViewManager(bool is_new, 
                        std::shared_ptr<simpledb::metadata::TableManager> tbl_manager,
                        std::shared_ptr<simpledb::tx::Transaction> tx)
: d_tbl_manager(tbl_manager)
{
    if (is_new) {
        auto schema = std::make_shared<simpledb::record::Schema>();
        schema->add_field("viewname", ::jdbc::ColumnType::STRING, TableManager::MAX_NAME_LEN);
        schema->add_field("viewdef", ::jdbc::ColumnType::STRING, MAX_VIEW_DEF);
        d_tbl_manager->create_table("viewcat", schema, tx);
    }
}

void ViewManager::create_view(std::string_view view_name, 
                            std::string_view vdef,
                            std::shared_ptr<simpledb::tx::Transaction> tx) {
    auto layout = d_tbl_manager->get_layout("viewcat", tx);
    simpledb::record::TableScan ts(tx, "viewcat", layout);
    ts.insert();
    ts.set_string("viewname", view_name);
    ts.set_string("viewdef", vdef);
    ts.close();
}

std::string ViewManager::get_view_def(std::string_view view_name,
                                    std::shared_ptr<simpledb::tx::Transaction> tx) {
    std::string result;
    auto layout = d_tbl_manager->get_layout("viewcat", tx);
    simpledb::record::TableScan ts(tx, "viewcat", layout);
    while (ts.next()) {
        if (ts.get_string("viewname") == view_name) {
            result = ts.get_string("viewdef");
            break;
        }
    }
    ts.close();
    return result;
}

} // namespace simpledb::metadata
