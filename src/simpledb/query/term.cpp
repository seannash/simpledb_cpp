#include "simpledb/query/term.hpp"
#include <stdexcept>
#include <limits>
#include <algorithm>

namespace simpledb::query {

Term::Term(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
    : d_lhs(std::move(lhs))
    , d_rhs(std::move(rhs)) {
}

bool Term::is_satisfied(simpledb::query::Scan& s) const {
    Constant lhs_val = d_lhs->evaluate(s);
    Constant rhs_val = d_rhs->evaluate(s);
    return lhs_val == rhs_val;
}

bool Term::applies_to(const simpledb::record::Schema& sch) const {
    return d_lhs->applies_to(sch) && d_rhs->applies_to(sch);
}

int Term::reduction_factor(const simpledb::plan::Plan& plan) const {
    std::string lhs_name {};
    std::string rhs_name {};
    if (d_lhs->is_field_name() && d_rhs->is_field_name()) {
        lhs_name = d_lhs->as_field_name();
        rhs_name = d_rhs->as_field_name();
        return std::max(plan.distinct_values(lhs_name), plan.distinct_values(rhs_name));
    }
    if (d_lhs->is_field_name()) {
        lhs_name = d_lhs->as_field_name();
        return plan.distinct_values(lhs_name);  
    }
    if (d_rhs->is_field_name()) {
        rhs_name = d_rhs->as_field_name();
        return plan.distinct_values(rhs_name);
    }
    if (d_lhs->is_constant() && d_rhs->is_constant()) {
        return 1;
    }
    return std::numeric_limits<int>::max();
}

Constant Term::equates_with_constant(std::string_view field) const {
    if (d_lhs->is_field_name() && d_lhs->as_field_name() == field && !d_rhs->is_field_name()) {
        return d_rhs->as_constant();
    }
    if (d_rhs->is_field_name() && d_rhs->as_field_name() == field && !d_lhs->is_field_name()) {
        return d_lhs->as_constant();
    }
    throw std::runtime_error("Term does not equate field with constant");
}

std::string Term::equates_with_field(std::string_view field) const {
    if (d_lhs->is_field_name() && d_lhs->as_field_name() == field && d_rhs->is_field_name()) {
        return d_rhs->as_field_name();
    }
    if (d_rhs->is_field_name() && d_rhs->as_field_name() == field && d_lhs->is_field_name()) {
        return d_lhs->as_field_name();
    }
    return ""; // TODO How to represent null?
}

std::string Term::to_string() const {
    return d_lhs->to_string() + "=" + d_rhs->to_string();
}

std::ostream& operator<<(std::ostream& os, const Term& t) {
    os << t.to_string();
    return os;
}

} // namespace simpledb::query
