#pragma once

#include "simpledb/metadata/table_manager.hpp"
#include "simpledb/tx/transaction.hpp"
#include "simpledb/metadata/stat_info.hpp"
#include "simpledb/record/layout.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <mutex>

namespace simpledb::metadata {


class StatManager {
public:
    StatManager(std::shared_ptr<simpledb::metadata::TableManager> tbl_manager, std::shared_ptr<simpledb::tx::Transaction> tx);
    ~StatManager() = default;

    StatInfo get_stat_info(std::string tblname, std::shared_ptr<simpledb::record::Layout> layout, std::shared_ptr<simpledb::tx::Transaction> tx);
    
private:
    std::mutex d_mutex;
    std::shared_ptr<simpledb::metadata::TableManager> d_tbl_manager;
    std::unordered_map<std::string, StatInfo> d_stats;
    int d_num_calls;

    void refresh_stats(std::shared_ptr<simpledb::tx::Transaction> tx);
    StatInfo calculate_stats(std::string tblname, std::shared_ptr<simpledb::record::Layout> layout, std::shared_ptr<simpledb::tx::Transaction> tx);

};
} // namespace simpledb::metadata