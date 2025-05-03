#include "simpledb/parser/create_table_data.hpp"

namespace simpledb::parse {

CreateTableData::CreateTableData(std::string table_name, simpledb::record::Schema schema)
    : d_table_name(std::move(table_name))
    , d_schema(std::move(schema))
{}

const std::string& CreateTableData::table_name() const {
    return d_table_name;
}

const simpledb::record::Schema& CreateTableData::schema() const {
    return d_schema;
}

} // namespace simpledb::parse 