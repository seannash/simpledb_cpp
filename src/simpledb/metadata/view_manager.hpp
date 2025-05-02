#pragma once

#include "simpledb/tx/transaction.hpp"
#include "simpledb/metadata/table_manager.hpp"
#include "simpledb/record/schema.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace simpledb::metadata {

class ViewManager {
public:
    ViewManager(bool is_new, std::shared_ptr<simpledb::metadata::TableManager> tbl_manager, std::shared_ptr<simpledb::tx::Transaction> tx);
    ~ViewManager() = default;
    
    void create_view(std::string_view view_name, std::string_view vdef, std::shared_ptr<simpledb::tx::Transaction> tx);
    std::string get_view_def(std::string_view view_name, std::shared_ptr<simpledb::tx::Transaction> tx);

private:
    std::shared_ptr<simpledb::metadata::TableManager> d_tbl_manager;
       
};

} // namespace simpledb::metadata

