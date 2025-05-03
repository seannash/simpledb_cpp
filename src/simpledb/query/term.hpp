#pragma once

#include "simpledb/query/expression.hpp"
#include "simpledb/query/scan.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/query/constant.hpp"
#include "simpledb/plan/plan.hpp"

namespace simpledb::query {

class Term {
public:
    Term(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    bool is_satisfied(simpledb::query::Scan& s) const;
    bool applies_to(const simpledb::record::Schema& sch) const;
    int reduction_factor(const simpledb::plan::Plan& plan) const;
    Constant equates_with_constant(std::string_view field) const;
    std::string equates_with_field(std::string_view field) const;
    std::string to_string() const;

private:
    std::shared_ptr<Expression> d_lhs;
    std::shared_ptr<Expression> d_rhs;
};

std::ostream& operator<<(std::ostream& os, const Term& t);

} // namespace simpledb::query