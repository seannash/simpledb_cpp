#include "simpledb/parser/modify_data.hpp"

namespace simpledb::parse {

ModifyData::ModifyData(std::string table_name, std::vector<std::string> fields, std::vector<simpledb::query::Constant> values)
    : d_table_name(std::move(table_name))
    , d_fields(std::move(fields))
    , d_values(std::move(values))
{}

const std::string& ModifyData::table_name() const {
    return d_table_name;
}

const std::vector<std::string>& ModifyData::fields() const {
    return d_fields;
}

const std::vector<simpledb::query::Constant>& ModifyData::values() const {
    return d_values;
}

} // namespace simpledb::parse 