#include "simpledb/parser/delete_data.hpp"

namespace simpledb::parse {

DeleteData::DeleteData(std::string table_name, std::shared_ptr<simpledb::query::Predicate> predicate)
    : d_table_name(std::move(table_name))
    , d_predicate(std::move(predicate))
{}

const std::string& DeleteData::table_name() const {
    return d_table_name;
}

const std::shared_ptr<simpledb::query::Predicate>& DeleteData::predicate() const {
    return d_predicate;
}

} // namespace simpledb::parse 