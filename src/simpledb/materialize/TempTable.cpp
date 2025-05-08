#include "simpledb/materialize/TempTable.hpp"
#include "simpledb/record/table_scan.hpp"
#include "simpledb/record/layout.hpp"
#include <sstream>
#include <mutex>

namespace simpledb::materialize {

std::mutex TempTable::s_mutex;
static int next_table_num = 0;

TempTable::TempTable(std::shared_ptr<simpledb::tx::Transaction> tx, 
                    std::string tblname,
                    std::shared_ptr<simpledb::record::Schema> schema)
: d_tx_(std::move(tx))
, d_tblname(std::move(tblname))
, d_layout(std::make_shared<simpledb::record::Layout>(*schema))
{}

std::shared_ptr<simpledb::record::TableScan> TempTable::open() const {
    return std::make_shared<simpledb::record::TableScan>(d_tx_, d_tblname, d_layout);
}

std::string_view TempTable::tableName() const {
    return d_tblname;
}

std::shared_ptr<simpledb::record::Layout> TempTable::getLayout() const {
    return d_layout;
}

std::string TempTable::nextTableName() {
    std::lock_guard<std::mutex> lock(s_mutex);
    std::stringstream ss;
    ss << "temp" << next_table_num++;
    return ss.str();
}

} // namespace simpledb::materialize 