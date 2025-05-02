#include "simpledb/record/table_scan.hpp"
#include <stdexcept>

namespace simpledb::record {

TableScan::TableScan(std::shared_ptr<simpledb::tx::Transaction> tx,
                     std::string_view table_name,
                     Layout layout)
    : d_tx(tx)
    , d_table_name(table_name)
    , d_layout(layout) {
}

void TableScan::close() {
}

bool TableScan::has_field(std::string_view field) {
    return false;
}

void TableScan::before_first() {
}

bool TableScan::next() {
    return false;
}

void TableScan::move_to_rid(RID rid) {
}

void TableScan::insert() {
}

int TableScan::get_int(std::string_view field) {
    return 0;
}

std::string TableScan::get_string(std::string_view field) {
    return "";
}

void TableScan::set_int(std::string_view field, int value) {
}

void TableScan::set_string(std::string_view field, std::string_view value) {
}

RID TableScan::current_rid() {
    return RID(0, 0);
}

void TableScan::delete_record() {
}

} // namespace simpledb::record
