#include "simpledb/parser/create_view_data.hpp"

namespace simpledb::parse {

CreateViewData::CreateViewData(std::string view_name, QueryData view_def)
    : d_view_name(std::move(view_name))
    , d_view_def(view_def.to_string())
{}

const std::string& CreateViewData::view_name() const {
    return d_view_name;
}

const std::string& CreateViewData::view_def() const {
    return d_view_def;
}

} // namespace simpledb::parse 