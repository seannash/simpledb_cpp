#pragma once

#include <string>
#include "simpledb/parser/query_data.hpp"

namespace simpledb::parse {

class CreateViewData {
public:
    CreateViewData(std::string view_name, QueryData query_data);
    ~CreateViewData() = default;

    const std::string& view_name() const;
    const QueryData& query_data() const;

private:
    std::string d_view_name;
    QueryData d_query_data;
};

} // namespace simpledb::parse 