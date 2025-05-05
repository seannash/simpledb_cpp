#include "simpledb/parse/query_data.hpp"
#include <sstream>

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

std::string QueryData::to_string() const {
    std::stringstream ss {};
    ss << "SELECT ";
    auto field_iter = d_select_list.begin();
    ss << *field_iter;
    while (++field_iter != d_select_list.end()) {
        ss << ", " << *field_iter;
    }
    ss << " FROM ";
    auto table_iter = d_table_list.begin();
    ss << *table_iter;
    while (++table_iter != d_table_list.end()) {
        ss << ", " << *table_iter;
    }
    auto predicate_str = d_predicate->to_string();
    if (!predicate_str.empty()) {
        ss << "WHERE " << predicate_str;
    }
    return ss.str();
}
} // namespace simpledb::parse 