#include "simpledb/parser/create_view_data.hpp"

namespace simpledb::parse {

CreateViewData::CreateViewData(std::string view_name, QueryData query_data)
    : d_view_name(std::move(view_name))
    , d_query_data(std::move(query_data))
{}

const std::string& CreateViewData::view_name() const {
    return d_view_name;
}

const QueryData& CreateViewData::query_data() const {
    return d_query_data;
}

} // namespace simpledb::parse 