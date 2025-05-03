#pragma once

#include "simpledb/query/constant.hpp"
#include "simpledb/record/schema.hpp"
#include "simpledb/query/scan.hpp"

#include <string>
#include <memory>

namespace simpledb::query {

class Expression {
public:

    Expression(Constant val);
    Expression(std::string field_name);
    ~Expression() = default;
    
    bool is_field_name() const;
    bool is_constant() const;
    
    std::string as_field_name() const;
    Constant as_constant() const;
    
    Constant evaluate(simpledb::query::Scan& scan) const;
    bool applies_to(const simpledb::record::Schema& schema) const;
    std::string to_string() const;

private:
    bool d_is_field_name;
    Constant d_val;
    std::string d_field_name;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);

} // namespace simpledb::query
