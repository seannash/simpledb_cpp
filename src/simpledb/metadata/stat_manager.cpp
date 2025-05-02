#include "simpledb/metadata/stat_manager.hpp"
#include "simpledb/record/table_scan.hpp"
#include <stdexcept>

namespace simpledb::metadata {

StatManager::StatManager(std::shared_ptr<simpledb::metadata::TableManager> tbl_manager,
                        std::shared_ptr<simpledb::tx::Transaction> tx)
: d_mutex()
, d_tbl_manager(tbl_manager)
, d_stats()
, d_num_calls(0) {
    refresh_stats(tx);
}

StatInfo StatManager::get_stat_info(std::string tblname,
                              std::shared_ptr<simpledb::record::Layout> layout,
                              std::shared_ptr<simpledb::tx::Transaction> tx) {
    std::lock_guard<std::mutex> lock(d_mutex);
    d_num_calls++;
    if (d_num_calls > 100) {
        refresh_stats(tx);
    }
    auto it = d_stats.find(tblname);
    if (it == d_stats.end()) {
        auto si = calculate_stats(tblname, layout, tx);
        d_stats.emplace(tblname, si);
        return si;
    }
    return it->second;
}

void StatManager::refresh_stats(std::shared_ptr<simpledb::tx::Transaction> tx) {
    d_stats.clear();
    d_num_calls = 0;
    auto tcat_layout = d_tbl_manager->get_layout("tblcat", tx);
    simpledb::record::TableScan tcat(tx, "tblcat", tcat_layout);
    while (tcat.next()) {
        std::string tblname = tcat.get_string("tblname");
        auto layout = d_tbl_manager->get_layout(tblname, tx);
        simpledb::record::TableScan tbl(tx, tblname, layout);
        while (tbl.next()) {
            std::string fldname = tbl.get_string("fldname");
            auto layout = d_tbl_manager->get_layout(tblname, tx);
            StatInfo si = calculate_stats(tblname, layout, tx);
            d_stats.emplace(tblname, si);
        }
    }
    tcat.close();
}

StatInfo StatManager::calculate_stats(std::string tblname,
                                      std::shared_ptr<simpledb::record::Layout> layout,
                                      std::shared_ptr<simpledb::tx::Transaction> tx) {
    int num_blocks = 0;
    int num_records = 0;
    simpledb::record::TableScan ts(tx, tblname, layout);
    while (ts.next()) {
        num_blocks = ts.current_rid().block_number() + 1;
        num_records++;
    }
    ts.close();
    return StatInfo(num_blocks, num_records);
}

} // namespace simpledb::metadata
