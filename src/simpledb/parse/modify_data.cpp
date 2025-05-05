#include "simpledb/parse/modify_data.hpp"

namespace simpledb::parse {

ModifyData::ModifyData(std::string table_name, std::string target_field, std::shared_ptr<simpledb::query::Expression> new_value, std::shared_ptr<simpledb::query::Predicate> predicate)
    : d_table_name(std::move(table_name))
    , d_target_field(std::move(target_field))
    , d_new_value(std::move(new_value))
    , d_predicate(std::move(predicate))
{}

const std::string& ModifyData::table_name() const {
    return d_table_name;
}

std::string ModifyData::target_field() const {
    return d_target_field;
}

std::shared_ptr<simpledb::query::Expression> ModifyData::new_value() const {
    return d_new_value;
}

const std::shared_ptr<simpledb::query::Predicate> ModifyData::predicate() const {
    return d_predicate;
}

} // namespace simpledb::parse 