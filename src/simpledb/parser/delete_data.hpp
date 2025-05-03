#pragma once

#include <string>
#include <memory>
#include "simpledb/query/predicate.hpp"

namespace simpledb::parse {

class DeleteData {
public:
    DeleteData(std::string table_name, std::shared_ptr<simpledb::query::Predicate> predicate);
    ~DeleteData() = default;

    const std::string& table_name() const;
    const std::shared_ptr<simpledb::query::Predicate>& predicate() const;

private:
    std::string d_table_name;
    std::shared_ptr<simpledb::query::Predicate> d_predicate;
};

} // namespace simpledb::parse 