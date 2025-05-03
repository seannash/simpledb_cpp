#pragma once

#include <string>
#include "simpledb/parser/query_data.hpp"

namespace simpledb::parse {

class CreateViewData {
public:
    CreateViewData(std::string view_name, QueryData view_def);
    ~CreateViewData() = default;

    const std::string& view_name() const;
    const std::string& view_def() const;

private:
    std::string d_view_name;
    std::string d_view_def;
};

} // namespace simpledb::parse 