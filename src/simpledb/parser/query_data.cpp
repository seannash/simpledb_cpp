#include "simpledb/parser/query_data.hpp"

namespace simpledb::parse {

QueryData::QueryData(std::vector<std::string> select_list, std::vector<std::string> table_list, std::shared_ptr<simpledb::query::Predicate> predicate)
    : d_select_list(std::move(select_list))
    , d_table_list(std::move(table_list))
    , d_predicate(std::move(predicate))
{}

const std::vector<std::string>& QueryData::select_list() const {
    return d_select_list;
}

const std::vector<std::string>& QueryData::table_list() const {
    return d_table_list;
}

const std::shared_ptr<simpledb::query::Predicate>& QueryData::predicate() const {
    return d_predicate;
}

} // namespace simpledb::parse 