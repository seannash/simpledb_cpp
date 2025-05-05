#include "simpledb/parse/create_table_data.hpp"

namespace simpledb::parse {

CreateTableData::CreateTableData(std::string table_name, simpledb::record::Schema new_schema)
    : d_table_name(std::move(table_name))
    , d_new_schema(std::move(new_schema))
{}

const std::string& CreateTableData::table_name() const {
    return d_table_name;
}

const simpledb::record::Schema& CreateTableData::new_schema() const {
    return d_new_schema;
}

} // namespace simpledb::parse 