#include "simpledb/materialize/RecordComparator.hpp"

namespace simpledb::materialize {

RecordComparator::RecordComparator(const std::vector<std::string>& fields)
: d_fields(fields)
{}

// FIXME: should be const parameters
int RecordComparator::compare(simpledb::query::Scan& s1, simpledb::query::Scan& s2) const {
    for (const auto& field : d_fields) {
        auto val1 = s1.get_val(field);
        auto val2 = s2.get_val(field);
        int result = val1 < val2;
        if (!result) {
            return result;
        }
    }
    return 0;
}

} // namespace simpledb::materialize 