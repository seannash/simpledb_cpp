#include "simpledb/materialize/MaxFn.hpp"

namespace simpledb::materialize {

MaxFn::MaxFn(std::string_view fieldname)
    : d_fldname(fieldname)
    , d_val() {}

simpledb::query::Constant MaxFn::processFirst(simpledb::query::Scan& s) {
    d_val = s.get_val(d_fldname);
    return d_val;
}

simpledb::query::Constant MaxFn::processNext(simpledb::query::Scan& s) {
    auto newval = s.get_val(d_fldname);
    if (newval > d_val) {
        d_val = newval;
    }
    return d_val;
}

std::string MaxFn::fieldName() const {
    return "maxof" + std::string(d_fldname);
}

simpledb::query::Constant MaxFn::value() const {
    return d_val;
}

} // namespace simpledb::materialize 