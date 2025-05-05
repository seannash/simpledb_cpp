#pragma once

#include <string>
#include <vector>
#include "simpledb/query/constant.hpp"
#include "simpledb/query/expression.hpp"
#include "simpledb/query/predicate.hpp"

namespace simpledb::parse {

class ModifyData {
public:
    ModifyData(std::string table_name,
               std::string target_field,
               std::shared_ptr<simpledb::query::Expression> new_value,
               std::shared_ptr<simpledb::query::Predicate> predicate);
    ~ModifyData() = default;

    const std::string& table_name() const;
    std::string target_field() const;
    std::shared_ptr<simpledb::query::Expression> new_value() const;
    const std::shared_ptr<simpledb::query::Predicate> predicate() const;

private:
    std::string d_table_name;
    std::string d_target_field;
    std::shared_ptr<simpledb::query::Expression> d_new_value;
    std::shared_ptr<simpledb::query::Predicate> d_predicate;
};

} // namespace simpledb::parse 