#include "simpledb/materialize/GroupValue.hpp"

namespace simpledb::materialize {

GroupValue::GroupValue(simpledb::query::Scan& s, const std::vector<std::string>& fields) {
    for (const auto& field : fields) {
        d_vals[field] = s.get_val(field);
    }
}

simpledb::query::Constant GroupValue::getVal(const std::string& fldname) {
    auto it = d_vals.find(fldname);
    if (it == d_vals.end()) {
        return simpledb::query::Constant();
    }
    return it->second;
}

bool GroupValue::operator==(const GroupValue& other) const {
    if (d_vals.size() != other.d_vals.size()) {
        return false;
    }
    for (const auto& [field, val] : d_vals) {
        auto it = other.d_vals.find(field);
        if (it == other.d_vals.end() || val != it->second) {
            return false;
        }
    }
    return true;
}

bool GroupValue::operator!=(const GroupValue& other) const {
    return !(*this == other);
}

int GroupValue::hashcode() const {
    int hashval = 0;
    for (const auto& [_, c] : d_vals) {
        hashval += c.hashCode();
    }
    return hashval;
}

} // namespace simpledb::materialize 