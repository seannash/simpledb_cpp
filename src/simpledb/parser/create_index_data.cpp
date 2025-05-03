#include "simpledb/parser/create_index_data.hpp"

namespace simpledb::parse {

CreateIndexData::CreateIndexData(std::string index_name, std::string table_name, std::vector<std::string> fields)
    : d_index_name(std::move(index_name))
    , d_table_name(std::move(table_name))
    , d_fields(std::move(fields))
{}

const std::string& CreateIndexData::index_name() const {
    return d_index_name;
}

const std::string& CreateIndexData::table_name() const {
    return d_table_name;
}

const std::vector<std::string>& CreateIndexData::fields() const {
    return d_fields;
}

} // namespace simpledb::parse 