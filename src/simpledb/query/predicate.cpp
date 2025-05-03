#include "simpledb/query/predicate.hpp"
#include <stdexcept>
#include "simpledb/record/schema.hpp"
#include <sstream>
namespace simpledb::query {

Predicate::Predicate() = default;

Predicate::Predicate(std::shared_ptr<Term> term)
: d_terms({term}) {
}

void Predicate::conjoin_with(std::shared_ptr<Predicate> pred) {
    d_terms.insert(d_terms.end(), pred->d_terms.begin(), pred->d_terms.end());
}

bool Predicate::is_satisfied(simpledb::query::Scan& s) const {
    for (const auto& term : d_terms) {
        if (!term->is_satisfied(s)) {
            return false;
        }
    }
    return true;
}

int Predicate::reduction_factor(Plan& plan) const {
    int factor = 1;
    for (const auto& term : d_terms) {
        factor *= term->reduction_factor(plan);
    }
    return factor;
}

std::shared_ptr<Predicate> Predicate::select_sub_predicates(const simpledb::record::Schema& schema) const {
    Predicate result;
    for (const auto& term : d_terms) {
        if (term->applies_to(schema)) {
            result.d_terms.push_back(term);
        }
    }
    if (result.d_terms.empty()) {
        return std::make_shared<Predicate>();
    }
    return std::make_shared<Predicate>(result);
}

std::shared_ptr<Predicate> Predicate::join_sub_predicates(
    const simpledb::record::Schema& schema1,
    const simpledb::record::Schema& schema2) const
{
    std::shared_ptr<Predicate> result = std::make_shared<Predicate>();
    simpledb::record::Schema new_schema {};
    new_schema.add_all(schema1);
    new_schema.add_all(schema2);

    for (const auto& term : d_terms) {
        if (!term->applies_to(schema1) && !term->applies_to(schema2) && term->applies_to(new_schema)) {
            result->d_terms.push_back(term);
        }
    }
    if (result->d_terms.empty()) {
        return result; // TODO Return nullptr like book?
    }
    return result;
}

Constant Predicate::equates_with_constant(std::string_view field) const {
    for (const auto& term : d_terms) {
        Constant c = term->equates_with_constant(field);
        if (!c.is_null()) {
            return c;
        }
    }
    return Constant();
}

Constant Predicate::equates_with_field(std::string_view field) const {
    for (const auto& term : d_terms) {
        Constant c = term->equates_with_field(field);
        if (!c.is_null()) {
            return c;
        }
    }
    return Constant();
}

std::string Predicate::to_string() const {
    if (d_terms.empty()) {
        return "";
    }
    std::stringstream ss {};
    ss << "(";
    for (const auto& term : d_terms) {
        ss << term->to_string() << " ";
    }
    ss << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Predicate& p) {
    os << p.to_string();
    return os;
}

} // namespace simpledb::query
