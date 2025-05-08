#include "simpledb/query/constant.hpp"
#include <stdexcept>
#include <functional>
namespace simpledb::query {

Constant::Constant()
: d_is_null(true)
, d_type()
, d_val(nullptr) {
}


Constant::Constant(int val)
: d_is_null(false)
, d_type(::jdbc::ColumnType::INT)
, d_val(val) {
}

Constant::Constant(std::string val)
: d_is_null(false)
, d_type(::jdbc::ColumnType::STRING)
, d_val(std::move(val)) {
}

bool Constant::isNull() const {
    return d_is_null;
}

int Constant::asInt() const {
    if (!d_is_null && d_type != ::jdbc::ColumnType::INT) {
        throw std::runtime_error("Constant is not an integer");
    }
    return std::any_cast<int>(d_val);
}

std::string Constant::asString() const {
    if (!d_is_null && d_type != ::jdbc::ColumnType::STRING) {
        throw std::runtime_error("Constant is not a string");
    }
    return std::any_cast<std::string>(d_val);
}

std::string Constant::toString() const {
    if (d_is_null) {
        return "null";
    }
    if (d_type == ::jdbc::ColumnType::INT) {
        return std::to_string(asInt());
    } else {
        return asString();
    }
}

std::strong_ordering Constant::operator<=>(const Constant& other) const {
    if (d_is_null || other.d_is_null || d_type != other.d_type) {
        throw std::runtime_error("Cannot compare constants of different types");
    }
    if (d_type == ::jdbc::ColumnType::INT) {
        return as_int() <=> other.as_int();
    } else {
        return as_string() <=> other.as_string();
    }
}

bool Constant::operator==(const Constant& other) const {
    return *this <=> other == 0;
}

int Constant::hashCode() const {
switch (d_type) {
        case ::jdbc::ColumnType::INT:
            return std::hash<int>{}(as_int());
        case ::jdbc::ColumnType::STRING:
            return std::hash<std::string>{}(as_string());
        default:
            throw std::runtime_error("Unsupported constant type");
    }
}

std::ostream& operator<<(std::ostream& os, const Constant& c) {
    os << c.to_string();
    return os;
}


} // namespace simpledb::query
