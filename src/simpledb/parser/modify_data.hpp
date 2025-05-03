#pragma once

#include <string>
#include <vector>
#include "simpledb/query/constant.hpp"

namespace simpledb::parse {

class ModifyData {
public:
    ModifyData(std::string table_name, std::vector<std::string> fields, std::vector<simpledb::query::Constant> values);
    ~ModifyData() = default;

    const std::string& table_name() const;
    const std::vector<std::string>& fields() const;
    const std::vector<simpledb::query::Constant>& values() const;

private:
    std::string d_table_name;
    std::vector<std::string> d_fields;
    std::vector<simpledb::query::Constant> d_values;
};

} // namespace simpledb::parse 