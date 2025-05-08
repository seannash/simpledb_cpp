#include "simpledb/materialize/MergeJoinScan.hpp"
#include <stdexcept>

namespace simpledb::materialize {

MergeJoinScan::MergeJoinScan(std::shared_ptr<simpledb::query::Scan> s1,
                           std::shared_ptr<SortScan> s2,
                           std::string fldname1,
                           std::string fldname2)
    : d_s1(std::move(s1))
    , d_s2(std::move(s2))
    , d_fldname1(std::move(fldname1))
    , d_fldname2(std::move(fldname2))
{
    before_first();
}

void MergeJoinScan::close() {
    d_s1->close();
    d_s2->close();
}

void MergeJoinScan::before_first() {
    d_s1->before_first();
    d_s2->before_first();
}

bool MergeJoinScan::next() {
    bool hasmore2 = d_s2->next();
    if (hasmore2 && d_s2->get_val(d_fldname2) == d_joinval) {
        return true;
    }

    bool hasmore1 = d_s1->next();
    if (hasmore1 && d_s1->get_val(d_fldname1) == d_joinval) {
        d_s2->restorePosition();
        return true;
    }

    while (hasmore1 && hasmore2) {
        auto v1 = d_s1->get_val(d_fldname1);
        auto v2 = d_s2->get_val(d_fldname2);
        if (v1 < v2) {
            hasmore1 = d_s1->next();
        } else if (v1 > v2) {
            hasmore2 = d_s2->next();
        } else {
            d_s2->savePosition();
            d_joinval = d_s2->get_val(d_fldname2);
            return true;
        }
    }
    return false;
}

int MergeJoinScan::get_int(std::string_view field) {
    if (d_s1->has_field(field)) {
        return d_s1->get_int(field);
    } else {
        return d_s2->get_int(field);
    }
}

std::string MergeJoinScan::get_string(std::string_view field) {
    if (d_s1->has_field(field)) {
        return d_s1->get_string(field);
    } else {
        return d_s2->get_string(field);
    }
}

simpledb::query::Constant MergeJoinScan::get_val(std::string_view field) {
    if (d_s1->has_field(field)) {
        return d_s1->get_val(field);
    } else {
        return d_s2->get_val(field);
    }
}

bool MergeJoinScan::has_field(std::string_view field) {
    return d_s1->has_field(field) || d_s2->has_field(field);
}

} // namespace simpledb::materialize 