#pragma once

#include "simpledb/query/term.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/plan/plan.hpp"

#include <memory>
#include <string>

namespace simpledb::query {

class Predicate {
public:
    Predicate();
    Predicate(std::shared_ptr<Term> term);
    ~Predicate() = default;

    void conjoin_with(std::shared_ptr<Predicate> pred);
    bool is_satisfied(simpledb::query::Scan& s) const;
    int reduction_factor(const simpledb::plan::Plan& plan) const;
    std::shared_ptr<Predicate> select_sub_predicates(const simpledb::record::Schema& schema) const;
    std::shared_ptr<Predicate> join_sub_predicates(const simpledb::record::Schema& schema1, const simpledb::record::Schema& schema2) const;
    Constant equates_with_constant(std::string_view field) const;
    Constant equates_with_field(std::string_view field) const;
    
    std::string to_string() const;

private:
    std::vector<std::shared_ptr<Term>> d_terms;
};

std::ostream& operator<<(std::ostream& os, const Predicate& p);

} // namespace simpledb::query