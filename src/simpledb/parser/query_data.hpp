#pragma once

#include <string>
#include <vector>
#include <memory>
#include "simpledb/query/predicate.hpp"

namespace simpledb::parse {

class QueryData {
public:
    QueryData(std::vector<std::string> select_list, std::vector<std::string> table_list, std::shared_ptr<simpledb::query::Predicate> predicate);
    ~QueryData() = default;

    const std::vector<std::string>& select_list() const;
    const std::vector<std::string>& table_list() const;
    const std::shared_ptr<simpledb::query::Predicate>& predicate() const;

    std::string to_string() const;

private:
    std::vector<std::string> d_select_list;
    std::vector<std::string> d_table_list;
    std::shared_ptr<simpledb::query::Predicate> d_predicate;
};

} // namespace simpledb::parse 