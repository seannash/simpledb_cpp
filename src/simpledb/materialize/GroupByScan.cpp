#include "simpledb/materialize/GroupByScan.hpp"
#include <stdexcept>
#include <algorithm>

namespace simpledb::materialize {

GroupByScan::GroupByScan(std::shared_ptr<simpledb::query::Scan> s,
                        std::vector<std::string> groupfields,
                        std::vector<std::shared_ptr<AggregateFn>> aggfns)
    : d_scan(std::move(s))
    , d_groupfields(std::move(groupfields))
    , d_aggfns(std::move(aggfns))
    , d_more_groups(false)
    , d_groupval()
{
    before_first();
}

void GroupByScan::before_first() {
    d_scan->before_first();
    d_more_groups = d_scan->next();
}

bool GroupByScan::next() {
    if (!d_more_groups) {
        return false;
    }
    for (const auto& fn : d_aggfns) {
        fn->processFirst(*d_scan);
    }
    d_groupval = GroupValue(*d_scan, d_groupfields);
    while (d_more_groups = d_scan->next() ) {
        GroupValue gv {*d_scan, d_groupfields};
        if (d_groupval != gv) {
            break;
        }
        for (const auto& fn : d_aggfns) {
            fn->processNext(*d_scan);
        }
    }
    return true;
}

int GroupByScan::get_int(std::string_view field) {
    return get_val(field).as_int();
}

std::string GroupByScan::get_string(std::string_view field) {
    return get_val(field).as_string();
}

simpledb::query::Constant GroupByScan::get_val(std::string_view field) {
    if (std::find(d_groupfields.begin(), d_groupfields.end(), field) != d_groupfields.end()) {
        return d_groupval.getVal(std::string(field));
    }
    for (const auto& fn : d_aggfns) {
        if (fn->fieldName() == field) {
            return fn->value();
        }
    }
    throw std::runtime_error("Field not found: " + std::string(field));
}

bool GroupByScan::has_field(std::string_view field) {
    if (std::find(d_groupfields.begin(), d_groupfields.end(), field) != d_groupfields.end()) {
        return true;
    }
    for (const auto& fn : d_aggfns) {
        if (fn->fieldName() == field) {
            return true;
        }
    }
    return false;
}

void GroupByScan::close() {
    d_scan->close();
}

} // namespace simpledb::materialize 