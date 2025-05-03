#pragma once

#include <string>
#include <vector>
#include "simpledb/record/schema.hpp"

namespace simpledb::parse {

class CreateTableData {
public:
    CreateTableData(std::string table_name, simpledb::record::Schema schema);
    ~CreateTableData() = default;

    const std::string& table_name() const;
    const simpledb::record::Schema& schema() const;

private:
    std::string d_table_name;
    simpledb::record::Schema d_schema;
};

} // namespace simpledb::parse 