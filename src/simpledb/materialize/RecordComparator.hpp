#pragma once

#include <vector>
#include <string>

#include "simpledb/query/scan.hpp"

namespace simpledb::materialize {

class RecordComparator {
public:
    RecordComparator(const std::vector<std::string>& fields);

    // FIXME: should be const parameters
    int compare(simpledb::query::Scan& s1, simpledb::query::Scan& s2) const;

private:
    std::vector<std::string> d_fields;

};

} // namespace simpledb::materialize