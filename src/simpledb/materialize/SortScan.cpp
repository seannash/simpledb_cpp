#include "simpledb/materialize/SortScan.hpp"
#include <stdexcept>

namespace simpledb::materialize {

SortScan::SortScan(std::vector<std::shared_ptr<TempTable>> runs,
                  RecordComparator comp)
: d_s1(nullptr)
, d_s2(nullptr)
, d_currentscan(nullptr)
, d_comp(std::move(comp))
, d_hasmore1(false)
, d_hasmore2(false)
, d_savedpositions(2)
{
    d_s1 = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(runs.at(0)->open());
    d_hasmore1 = d_s1->next();
    if (runs.size() > 1) {
        d_s2 = std::dynamic_pointer_cast<simpledb::query::UpdateScan>(runs.at(1)->open());
        d_hasmore2 = d_s2->next();
    }
}

void SortScan::before_first() {
    d_s1->before_first();
    d_hasmore1 = d_s1->next();
    if (d_s2) {
        d_s2->before_first();
        d_hasmore2 = d_s2->next();
    }
}

bool SortScan::next() {
    if (d_currentscan == d_s1) {
        d_hasmore1 = d_s1->next();
    } else {
        d_hasmore2 = d_s2->next();
    }

    if (!d_hasmore1 && !d_hasmore2) {
        return false;
    } else if (d_hasmore1 && d_hasmore2) {
        if (d_comp.compare(*d_s1, *d_s2) < 0) {
            d_currentscan = d_s1;
        } else {
            d_currentscan = d_s2;
        }
    } else if (d_hasmore1) {
        d_currentscan = d_s1;
    } else {
        d_currentscan = d_s2;
    }
    return true;
}

int SortScan::get_int(std::string_view field) {
    return d_currentscan->get_int(field);
}

std::string SortScan::get_string(std::string_view field) {
    return d_currentscan->get_string(field);
}

simpledb::query::Constant SortScan::get_val(std::string_view field) {
    return d_currentscan->get_val(field);
}

bool SortScan::has_field(std::string_view field) {
    return d_currentscan->has_field(field);
}

void SortScan::close() {
    d_s1->close();
    if (d_s2) {
        d_s2->close();
    }
}

void SortScan::savePosition() {
    auto rid1 = d_s1->current_rid();
    auto rid2 = d_s2->current_rid();
    d_savedpositions[0] = rid1;
    d_savedpositions[1] = rid2;
}

void SortScan::restorePosition() {
    auto rid1 = d_savedpositions[0];
    auto rid2 = d_savedpositions[1];
    d_s1->move_to_rid(rid1);
    d_s2->move_to_rid(rid2);
}

} // namespace simpledb::materialize 