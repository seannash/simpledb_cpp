#pragma once

#include <string>
#include <vector>

namespace simpledb::parse {

class CreateIndexData {
public:
    CreateIndexData(std::string index_name, std::string table_name, std::vector<std::string> fields);
    ~CreateIndexData() = default;

    const std::string& index_name() const;
    const std::string& table_name() const;
    const std::vector<std::string>& fields() const;

private:
    std::string d_index_name;
    std::string d_table_name;
    std::vector<std::string> d_fields;
};

} // namespace simpledb::parse 