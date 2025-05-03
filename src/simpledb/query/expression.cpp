#include "simpledb/query/expession.hpp"
#include <stdexcept>

namespace simpledb::query {

Expression::Expression(Constant val)
    : d_is_field_name(false)
    , d_val(std::move(val))
    , d_field_name() {
}

Expression::Expression(std::string field_name)
    : d_is_field_name(true)
    , d_val(0)  // Default value, won't be used
    , d_field_name(std::move(field_name)) {
}

bool Expression::is_field_name() const {
    return d_is_field_name;
}

std::string Expression::as_field_name() const {
    if (!d_is_field_name) {
        throw std::runtime_error("Expression is not a field name");
    }
    return d_field_name;
}

Constant Expression::as_constant() const {
    if (d_is_field_name) {
        throw std::runtime_error("Expression is not a constant");
    }
    return d_val;
}

Constant Expression::evaluate(simpledb::query::Scan& scan) const {
    if (d_is_field_name) {
        if (!scan.has_field(d_field_name)) {
            throw std::runtime_error("Field not found: " + d_field_name);
        }
        return Constant(scan.get_val(d_field_name));
    }
    return d_val;
}

bool Expression::applies_to(const simpledb::record::Schema& schema) const {
    if (!d_is_field_name) {
        return true;
    }
    return schema.has_field(d_field_name);
}

std::string Expression::to_string() const {
    if (d_is_field_name) {
        return d_field_name;
    }
    return d_val.to_string();
}

std::ostream& operator<<(std::ostream& os, const Expression& e) {
    os << e.to_string();
    return os;
}

} // namespace simpledb::query
