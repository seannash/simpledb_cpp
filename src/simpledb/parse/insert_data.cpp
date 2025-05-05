#include "simpledb/parse/insert_data.hpp"

namespace simpledb::parse {

InsertData::InsertData(std::string table_name, std::vector<std::string> fields, std::vector<simpledb::query::Constant> values)
: d_table_name(std::move(table_name))
, d_fields(std::move(fields))
, d_values(std::move(values))
{}

const std::string& InsertData::table_name() const {
    return d_table_name;
}

const std::vector<std::string>& InsertData::fields() const {
    return d_fields;
}

const std::vector<simpledb::query::Constant>& InsertData::values() const {
    return d_values;
}

} // namespace simpledb::parse
