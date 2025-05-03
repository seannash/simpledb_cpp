#pragma once

#include <string>
#include <vector>

namespace simpledb::parse {

class CreateIndexData {
public:
    CreateIndexData(std::string index_name, std::string table_name, std::string field_name);
    ~CreateIndexData() = default;

    const std::string& index_name() const;
    const std::string& table_name() const;
    const std::string& field_name() const;

private:
    std::string d_index_name;
    std::string d_table_name;
    std::string d_field_name;
};

} // namespace simpledb::parse 