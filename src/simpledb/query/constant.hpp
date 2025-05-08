#pragma once

#include "jdbc/ColumnType.hpp"

#include <any>
#include <string>
#include <compare>

namespace simpledb::query {

class Constant {
public:
    Constant();
    Constant(int val);
    Constant(std::string val);

    bool isNull() const;

    int asInt() const;
    std::string asString() const;

    std::strong_ordering operator<=>(const Constant& other) const;
    bool operator==(const Constant& other) const;

    std::string toString() const;


    int hashCode() const;
    
private:
    bool d_is_null;
    ::jdbc::ColumnType d_type;
    std::any d_val;
};

std::ostream& operator<<(std::ostream& os, const Constant& c);

} // namespace simpledb::query
