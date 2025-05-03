#include "simpledb/query/product_scan.hpp"
#include <stdexcept>

namespace simpledb::query {

ProductScan::ProductScan(std::shared_ptr<Scan> left_scan, std::shared_ptr<Scan> right_scan)
    : d_left_scan(left_scan)
    , d_right_scan(right_scan)
{}

void ProductScan::before_first() {
    d_left_scan->before_first();
    d_left_scan->next();
    d_right_scan->before_first();
}

bool ProductScan::next() {
    if (d_right_scan->next()) {
        return true;
    } else {
        d_right_scan->before_first();
        return d_right_scan->next() && d_left_scan->next();
    }
}

int ProductScan::get_int(std::string_view field) {
    if (d_left_scan->has_field(field)) {
        return d_left_scan->get_int(field);
    }
    return d_right_scan->get_int(field);
}

std::string ProductScan::get_string(std::string_view field) {
    if (d_left_scan->has_field(field)) {
        return d_left_scan->get_string(field);
    }
    return d_right_scan->get_string(field);
}

simpledb::query::Constant ProductScan::get_val(std::string_view field) {
    if (d_left_scan->has_field(field)) {
        return d_left_scan->get_val(field);
    }
    return d_right_scan->get_val(field);
}

bool ProductScan::has_field(std::string_view field) {
    return d_left_scan->has_field(field) || d_right_scan->has_field(field);
}

void ProductScan::close() {
    d_left_scan->close();
    d_right_scan->close();
}

} // namespace simpledb::query
