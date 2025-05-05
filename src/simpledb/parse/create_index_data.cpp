#include "simpledb/parse/create_index_data.hpp"

namespace simpledb::parse {

CreateIndexData::CreateIndexData(std::string index_name, std::string table_name, std::string field_name)
    : d_index_name(std::move(index_name))
    , d_table_name(std::move(table_name))
    , d_field_name(std::move(field_name))
{}

const std::string& CreateIndexData::index_name() const {
    return d_index_name;
}

const std::string& CreateIndexData::table_name() const {
    return d_table_name;
}

const std::string& CreateIndexData::field_name() const {
    return d_field_name;
}

} // namespace simpledb::parse 