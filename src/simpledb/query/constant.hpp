#pragma once

#include "simpledb/record/field_type.hpp"

#include <any>
#include <string>
#include <compare>

namespace simpledb::query {

class Constant {
public:
    Constant(int val);
    Constant(std::string val);

    int as_int() const;
    std::string as_string() const;

    std::strong_ordering operator<=>(const Constant& other) const;
    bool operator==(const Constant& other) const;

    std::string to_string() const;
    
private:
    simpledb::record::FieldType d_type;
    std::any d_val;
};

std::ostream& operator<<(std::ostream& os, const Constant& c);

} // namespace simpledb::query
