#include "simpledb/query/select_scan.hpp"
#include <stdexcept>

namespace simpledb::query {

SelectScan::SelectScan(std::shared_ptr<Scan> scan, std::shared_ptr<Predicate> pred)
: d_scan(scan)
, d_predicate(pred) {
}

void SelectScan::before_first() {
    d_scan->before_first();
}

bool SelectScan::next() {
    while (d_scan->next()) {
        if (d_predicate->is_satisfied(*d_scan)) {
            return true;
        }
    }
    return false;
}

int SelectScan::get_int(std::string_view field) {
    return d_scan->get_int(field);
}

std::string SelectScan::get_string(std::string_view field) {
    return d_scan->get_string(field);
}

simpledb::query::Constant SelectScan::get_val(std::string_view field) {
    return d_scan->get_val(field);
}

bool SelectScan::has_field(std::string_view field) {
    return d_scan->has_field(field);
}

void SelectScan::close() {
    d_scan->close();
}

void SelectScan::set_int(std::string_view field, int val) {
    auto us = std::dynamic_pointer_cast<UpdateScan>(d_scan);
    if (us) {
        us->set_int(field, val);
    } else {
        throw std::runtime_error("Underlying scan is not an UpdateScan");
    }
}

void SelectScan::set_string(std::string_view field, std::string_view val) {
    auto us = std::dynamic_pointer_cast<UpdateScan>(d_scan);
    if (us) {
        us->set_string(field, val);
    } else {
        throw std::runtime_error("Underlying scan is not an UpdateScan");
    }
}

void SelectScan::set_val(std::string_view field, simpledb::query::Constant val) {
    auto us = std::dynamic_pointer_cast<UpdateScan>(d_scan);
    if (us) {
        us->set_val(field, val);
    } else {
        throw std::runtime_error("Underlying scan is not an UpdateScan");
    }
}

void SelectScan::insert() {
    auto us = std::dynamic_pointer_cast<UpdateScan>(d_scan);
    if (us) {
        us->insert();
    } else {
        throw std::runtime_error("Underlying scan is not an UpdateScan");
    }
}

void SelectScan::delete_record() {
    auto us = std::dynamic_pointer_cast<UpdateScan>(d_scan);
    if (us) {
        us->delete_record();
    } else {
        throw std::runtime_error("Underlying scan is not an UpdateScan");
    }
}

simpledb::record::RID SelectScan::current_rid() {
    auto us = std::dynamic_pointer_cast<UpdateScan>(d_scan);
    if (us) {
        return us->current_rid();
    } else {
        throw std::runtime_error("Underlying scan is not an UpdateScan");
    }
}

void SelectScan::move_to_rid(simpledb::record::RID rid) {
    auto us = std::dynamic_pointer_cast<UpdateScan>(d_scan);
    if (us) {
        us->move_to_rid(rid);
    } else {
        throw std::runtime_error("Underlying scan is not an UpdateScan");
    }
}

} // namespace simpledb::query
